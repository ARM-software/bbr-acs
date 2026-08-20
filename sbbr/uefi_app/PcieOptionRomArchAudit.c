/** @file

  Copyright (c) 2026, Arm Ltd. All rights reserved.<BR>

  This program and the accompanying materials are licensed and made available
  under the terms and conditions of the BSD License which accompanies this
  distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
/*++

Module Name:
  PcieOptionRomArchAudit.c

Abstract:
  PCIe option ROM architecture audit UEFI application source.

--*/

#include <Uefi.h>

#include <IndustryStandard/Pci22.h>
#include <IndustryStandard/Pci23.h>
#include <IndustryStandard/PeImage.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/ShellLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/ComponentName.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/PciIo.h>

//#define PCI_CODE_TYPE_EFI_IMAGE        0x03
#define EFI_PCI_CAPABILITY_ID_PCIEXP   0x10
#define IS_LAST_ROM_IMAGE(_pcir)       (((_pcir)->Indicator & 0x80) != 0)

#define MAX_RFC4646_LANGUAGE_LEN       32
#define AUDIT_LOG_PATH                 L"\\acs_results_template\\acs_results\\uefi_dump\\PcieOptionRomArchAudit.log"
#define AUDIT_LOG_FALLBACK_PATH        L"PcieOptionRomArchAudit.log"

typedef struct {
  BOOLEAN  HasLegacyX86;
  BOOLEAN  HasIa32Uefi;
  BOOLEAN  HasX64Uefi;
  BOOLEAN  HasArm64Uefi;
  BOOLEAN  HasEbcUefi;
  BOOLEAN  HasOtherUefi;
  UINTN    ImageCount;
  UINTN    EfiImageCount;
  UINTN    DriverImageCount;
} ROM_AUDIT;

typedef struct {
  UINTN  ScannedHandles;
  UINTN  PcieDevices;
  UINTN  DevicesWithRom;
  UINTN  Hits;
  UINTN  DevicesWithLegacyX86;
  UINTN  DevicesWithIa32Uefi;
  UINTN  DevicesWithX64Uefi;
  UINTN  DevicesWithArm64Uefi;
  UINTN  DevicesWithEbcUefi;
  UINTN  DevicesWithOtherUefi;
  UINTN  RomParseErrors;
  UINTN  InspectionErrors;
} RUN_STATS;

typedef struct {
  UINTN   Segment;
  UINTN   Bus;
  UINTN   Device;
  UINTN   Function;
  UINT16  VendorId;
  UINT16  DeviceId;
  CHAR16  DeviceName[64];
} NON_COMPLIANT_DEVICE;

STATIC SHELL_FILE_HANDLE  mLogFile = NULL;

/*++

Routine Description:

  Opens the ACS result log for the PCIe option ROM audit.

Arguments:

  None.

Returns:

  EFI_SUCCESS           - The log file was opened.
  Error status           - The log file could not be opened.

--*/
STATIC
EFI_STATUS
OpenLogFile (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = ShellOpenFileByName (
             AUDIT_LOG_PATH,
             &mLogFile,
             EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,
             0
             );
  if (Status != EFI_NOT_FOUND) {
    return Status;
  }

  Print (
    L"Unable to open %s: %r. Using %s in the current directory.\n",
    AUDIT_LOG_PATH,
    Status,
    AUDIT_LOG_FALLBACK_PATH
    );

  return ShellOpenFileByName (
           AUDIT_LOG_FALLBACK_PATH,
           &mLogFile,
           EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,
           0
           );
}

/*++

Routine Description:

  Writes a formatted audit message to the ACS result log.

Arguments:

  Format  - Format string for the message.
  ...     - Format arguments.

Returns:

  None.

--*/
STATIC
VOID
AuditLog (
  IN CONST CHAR16  *Format,
  ...
  )
{
  VA_LIST  Marker;
  CHAR16   Buffer[1024];
  CHAR8    AsciiBuffer[1024];
  UINTN    Index;
  UINTN    Size;

  if (mLogFile == NULL) {
    return;
  }

  VA_START (Marker, Format);
  UnicodeVSPrint (Buffer, sizeof (Buffer), Format, Marker);
  VA_END (Marker);

  //
  // Write byte-oriented text so UEFI Shell's "cat" does not render the
  // UTF-16 NUL byte after every character as a dot.
  //
  Size = StrLen (Buffer);
  for (Index = 0; Index < Size; Index++) {
    AsciiBuffer[Index] = (Buffer[Index] <= 0x7f) ? (CHAR8)Buffer[Index] : '?';
  }

  ShellWriteFile (mLogFile, &Size, AsciiBuffer);
}

#define LOG(...)  AuditLog (__VA_ARGS__)


/*++

Routine Description:

  Returns a display name for a PCI vendor ID.

Arguments:

  VendorId  - PCI vendor ID.

Returns:

  Pointer to a display string.

--*/
STATIC
CONST CHAR16 *
VendorIdToStr (
  IN UINT16  VendorId
  )
{
  switch (VendorId) {
    case 0x1002: return L"AMD";
    case 0x10de: return L"NVIDIA";
    case 0x10ec: return L"Realtek";
    case 0x11ab: return L"Marvell";
    case 0x1234: return L"QEMU";
    case 0x13b5: return L"Arm";
    case 0x144d: return L"Samsung";
    case 0x14e4: return L"Broadcom";
    case 0x15ad: return L"VMware";
    case 0x15b3: return L"Mellanox";
    case 0x17cb: return L"Qualcomm";
    case 0x1af4: return L"Red Hat";
    case 0x8086: return L"Intel";
    default:     return L"Unknown vendor";
  }
}

/*++

Routine Description:

  Returns a display name for a PCI class code.

Arguments:

  BaseClass  - PCI base class code.
  SubClass   - PCI subclass code.
  ProgIf     - PCI programming interface code.

Returns:

  Pointer to a display string.

--*/
STATIC
CONST CHAR16 *
PciClassCodeToStr (
  IN UINT8  BaseClass,
  IN UINT8  SubClass,
  IN UINT8  ProgIf
  )
{
  switch (BaseClass) {
    case 0x01:
      switch (SubClass) {
        case 0x01: return L"IDE Controller";
        case 0x04: return L"RAID Controller";
        case 0x06: return (ProgIf == 0x01) ? L"SATA Controller (AHCI)" : L"SATA Controller";
        case 0x07: return L"Serial Attached SCSI Controller";
        case 0x08: return (ProgIf == 0x02) ? L"NVMe Controller" : L"Non-Volatile Memory Controller";
        default:   return L"Mass Storage Controller";
      }
    case 0x02:
      return (SubClass == 0x00) ? L"Ethernet Controller" : L"Network Controller";
    case 0x03:
      switch (SubClass) {
        case 0x00: return L"VGA Controller";
        case 0x01: return L"XGA Controller";
        case 0x02: return L"3D Controller";
        default:   return L"Display Controller";
      }
    case 0x04: return L"Multimedia Controller";
    case 0x05: return L"Memory Controller";
    case 0x06:
      switch (SubClass) {
        case 0x00: return L"Host Bridge";
        case 0x01: return L"ISA Bridge";
        case 0x04: return L"PCI-to-PCI Bridge";
        case 0x07: return L"CardBus Bridge";
        case 0x08: return L"RACEway Bridge";
        case 0x09: return L"Semi-Transparent PCI-to-PCI Bridge";
        case 0x0a: return L"InfiniBand-to-PCI Host Bridge";
        default:   return L"Bridge Device";
      }
    case 0x07: return (SubClass == 0x00) ? L"Serial Controller" : L"Communication Controller";
    case 0x08: return L"System Peripheral";
    case 0x09: return L"Input Device Controller";
    case 0x0a: return L"Docking Station";
    case 0x0b: return L"Processor";
    case 0x0c:
      switch (SubClass) {
        case 0x03: return L"USB Controller";
        case 0x05: return L"SMBus Controller";
        case 0x06: return L"InfiniBand Controller";
        case 0x07: return L"IPMI Interface";
        case 0x08: return L"SERCOS Interface";
        case 0x09: return L"CANbus Controller";
        default:   return L"Serial Bus Controller";
      }
    case 0x0d: return L"Wireless Controller";
    case 0x0e: return L"Intelligent I/O Controller";
    case 0x0f: return L"Satellite Communication Controller";
    case 0x10: return L"Encryption Controller";
    case 0x11: return L"Signal Processing Controller";
    case 0x12: return L"Processing Accelerator";
    case 0x13: return L"Non-Essential Instrumentation";
    default:     return L"Unclassified Device";
  }
}
STATIC
CONST CHAR16 *
MachineTypeToStr (
  IN UINT16  MachineType
  )
{
  switch (MachineType) {
    case IMAGE_FILE_MACHINE_I386:
      return L"IA32";
    case IMAGE_FILE_MACHINE_X64:
      return L"X64";
    case IMAGE_FILE_MACHINE_ARM64:
      return L"AARCH64";
    case IMAGE_FILE_MACHINE_EBC:
      return L"EBC";
#ifdef IMAGE_FILE_MACHINE_RISCV32
    case IMAGE_FILE_MACHINE_RISCV32:
      return L"RISCV32";
#endif
#ifdef IMAGE_FILE_MACHINE_RISCV64
    case IMAGE_FILE_MACHINE_RISCV64:
      return L"RISCV64";
#endif
#ifdef IMAGE_FILE_MACHINE_RISCV128
    case IMAGE_FILE_MACHINE_RISCV128:
      return L"RISCV128";
#endif
    default:
      return L"OTHER";
  }
}

/*++

Routine Description:

  Determines whether a PE/COFF machine type is recognized by the audit.

Arguments:

  MachineType  - Input parameter.

Returns:

  TRUE                  - The condition is met.
  FALSE                 - The condition is not met.

--*/
STATIC
BOOLEAN
IsKnownMachineType (
  IN UINT16  MachineType
  )
{
  switch (MachineType) {
    case IMAGE_FILE_MACHINE_I386:
    case IMAGE_FILE_MACHINE_X64:
    case IMAGE_FILE_MACHINE_ARM64:
    case IMAGE_FILE_MACHINE_EBC:
      return TRUE;
#ifdef IMAGE_FILE_MACHINE_RISCV32
    case IMAGE_FILE_MACHINE_RISCV32:
      return TRUE;
#endif
#ifdef IMAGE_FILE_MACHINE_RISCV64
    case IMAGE_FILE_MACHINE_RISCV64:
      return TRUE;
#endif
#ifdef IMAGE_FILE_MACHINE_RISCV128
    case IMAGE_FILE_MACHINE_RISCV128:
      return TRUE;
#endif
    default:
      return FALSE;
  }
}

/*++

Routine Description:

  Returns the display name for a PE/COFF subsystem.

Arguments:

  Subsystem  - Input parameter.

Returns:

  Pointer to a display string.

--*/
STATIC
CONST CHAR16 *
SubsystemToStr (
  IN UINT16  Subsystem
  )
{
  switch (Subsystem) {
    case EFI_IMAGE_SUBSYSTEM_EFI_APPLICATION:
      return L"APP";
    case EFI_IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
      return L"BS_DRIVER";
    case EFI_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
      return L"RT_DRIVER";
    default:
      return L"OTHER";
  }
}

/*++

Routine Description:

  Determines whether a PE/COFF subsystem represents a UEFI driver.

Arguments:

  Subsystem  - Input parameter.

Returns:

  TRUE                  - The condition is met.
  FALSE                 - The condition is not met.

--*/
STATIC
BOOLEAN
IsDriverSubsystem (
  IN UINT16  Subsystem
  )
{
  return (BOOLEAN)(
           (Subsystem == EFI_IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER) ||
           (Subsystem == EFI_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER)
           );
}

/*++

Routine Description:

  Determines whether a UTF-16 character is command-line whitespace.

Arguments:

  Ch  - Input parameter.

Returns:

  TRUE                  - The condition is met.
  FALSE                 - The condition is not met.

--*/
STATIC
BOOLEAN
IsWhiteSpaceChar (
  IN CHAR16  Ch
  )
{
  return (BOOLEAN)(
           (Ch == L' ')  ||
           (Ch == L'\t') ||
           (Ch == L'\r') ||
           (Ch == L'\n')
           );
}

/*++

Routine Description:

  Converts an uppercase UTF-16 ASCII character to lowercase.

Arguments:

  Ch  - Input parameter.

Returns:

  The lowercase UTF-16 ASCII character, or the original character when no conversion is needed.

--*/
STATIC
CHAR16
AsciiLowerChar16 (
  IN CHAR16  Ch
  )
{
  if ((Ch >= L'A') && (Ch <= L'Z')) {
    return (CHAR16)(Ch - L'A' + L'a');
  }

  return Ch;
}

/*++

Routine Description:

  Compares two UTF-16 strings without regard to ASCII case.

Arguments:

  Left  - Input parameter.
  Right  - Input parameter.

Returns:

  TRUE                  - The condition is met.
  FALSE                 - The condition is not met.

--*/
STATIC
BOOLEAN
StriEq (
  IN CONST CHAR16  *Left,
  IN CONST CHAR16  *Right
  )
{
  while ((*Left != L'\0') && (*Right != L'\0')) {
    if (AsciiLowerChar16 (*Left) != AsciiLowerChar16 (*Right)) {
      return FALSE;
    }

    Left++;
    Right++;
  }

  return (BOOLEAN)((*Left == L'\0') && (*Right == L'\0'));
}

/*++

Routine Description:

  Converts an uppercase ASCII character to lowercase.

Arguments:

  Ch  - Input parameter.

Returns:

  The lowercase ASCII character, or the original character when no conversion is needed.

--*/
STATIC
CHAR8
AsciiLowerChar8 (
  IN CHAR8  Ch
  )
{
  if ((Ch >= 'A') && (Ch <= 'Z')) {
    return (CHAR8)(Ch - 'A' + 'a');
  }

  return Ch;
}

/*++

Routine Description:

  Compares a fixed-length ASCII prefix without regard to case.

Arguments:

  Left  - Input parameter.
  Right  - Input parameter.
  Count  - Input parameter.

Returns:

  TRUE                  - The condition is met.
  FALSE                 - The condition is not met.

--*/
STATIC
BOOLEAN
AsciiPrefixInsensitive (
  IN CONST CHAR8  *Left,
  IN CONST CHAR8  *Right,
  IN UINTN        Count
  )
{
  UINTN  Index;

  for (Index = 0; Index < Count; Index++) {
    if ((Left[Index] == '\0') || (Right[Index] == '\0')) {
      return FALSE;
    }

    if (AsciiLowerChar8 (Left[Index]) != AsciiLowerChar8 (Right[Index])) {
      return FALSE;
    }
  }

  return TRUE;
}

/*++

Routine Description:

  Determines whether an ASCII character separates language tags.

Arguments:

  Ch  - Input parameter.

Returns:

  TRUE                  - The condition is met.
  FALSE                 - The condition is not met.

--*/
STATIC
BOOLEAN
IsLanguageSeparator (
  IN CHAR8  Ch
  )
{
  return (BOOLEAN)(
           (Ch == ';') ||
           (Ch == ',') ||
           (Ch == ' ') ||
           (Ch == '\t')
           );
}

/*++

Routine Description:

  Selects a supported English RFC 4646 language tag.

Arguments:

  SupportedLanguages  - Input parameter.
  LanguageBuffer  - Output parameter.
  LanguageBufferSize  - Input parameter.

Returns:

  TRUE                  - The condition is met.
  FALSE                 - The condition is not met.

--*/
STATIC
BOOLEAN
SelectEnglishRfc4646Language (
  IN  CONST CHAR8  *SupportedLanguages,
  OUT CHAR8        *LanguageBuffer,
  IN  UINTN        LanguageBufferSize
  )
{
  CONST CHAR8  *Start;
  CONST CHAR8  *Ptr;
  UINTN        Length;
  UINTN        Index;

  if ((SupportedLanguages == NULL) || (LanguageBuffer == NULL) || (LanguageBufferSize < 3)) {
    return FALSE;
  }

  Ptr = SupportedLanguages;
  while (*Ptr != '\0') {
    while ((*Ptr != '\0') && IsLanguageSeparator (*Ptr)) {
      Ptr++;
    }

    if (*Ptr == '\0') {
      break;
    }

    Start = Ptr;
    while ((*Ptr != '\0') && !IsLanguageSeparator (*Ptr)) {
      Ptr++;
    }

    Length = (UINTN)(Ptr - Start);
    if ((Length >= 2) && AsciiPrefixInsensitive (Start, "en", 2) &&
        ((Length == 2) || (Start[2] == '-')))
    {
      if (Length >= LanguageBufferSize) {
        Length = LanguageBufferSize - 1;
      }

      for (Index = 0; Index < Length; Index++) {
        LanguageBuffer[Index] = Start[Index];
      }

      LanguageBuffer[Length] = '\0';
      return TRUE;
    }
  }

  return FALSE;
}

/*++

Routine Description:

  Determines whether an ISO 639-2 language list supports English.

Arguments:

  SupportedLanguages  - Input parameter.

Returns:

  TRUE                  - The condition is met.
  FALSE                 - The condition is not met.

--*/
STATIC
BOOLEAN
SupportsEnglishIso639_2 (
  IN CONST CHAR8  *SupportedLanguages
  )
{
  CONST CHAR8  *Ptr;

  if (SupportedLanguages == NULL) {
    return FALSE;
  }

  for (Ptr = SupportedLanguages; (Ptr[0] != '\0') && (Ptr[1] != '\0') && (Ptr[2] != '\0'); Ptr += 3) {
    if (AsciiPrefixInsensitive (Ptr, "eng", 3)) {
      return TRUE;
    }
  }

  return FALSE;
}

/*++

Routine Description:

  Retrieves a null-terminated copy of the application load options.

Arguments:

  ImageHandle  - Input parameter.
  LoadOptionsText  - Output parameter.

Returns:

  EFI_SUCCESS           - The operation completed successfully.
  Error status           - The operation failed.

--*/
STATIC
EFI_STATUS
GetLoadOptionsCopy (
  IN  EFI_HANDLE  ImageHandle,
  OUT CHAR16      **LoadOptionsText
  )
{
  EFI_STATUS                 Status;
  EFI_LOADED_IMAGE_PROTOCOL  *LoadedImage;
  CHAR16                     *Buffer;
  UINTN                      CharCount;

  if (LoadOptionsText == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *LoadOptionsText = NULL;

  Status = gBS->HandleProtocol (
                  ImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID **)&LoadedImage
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((LoadedImage->LoadOptions == NULL) || (LoadedImage->LoadOptionsSize == 0)) {
    return EFI_NOT_FOUND;
  }

  CharCount = LoadedImage->LoadOptionsSize / sizeof (CHAR16);
  if ((LoadedImage->LoadOptionsSize % sizeof (CHAR16)) != 0) {
    CharCount++;
  }

  Buffer = AllocateZeroPool ((CharCount + 1) * sizeof (CHAR16));
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (Buffer, LoadedImage->LoadOptions, LoadedImage->LoadOptionsSize);
  Buffer[CharCount] = L'\0';

  *LoadOptionsText = Buffer;
  return EFI_SUCCESS;
}

/*++

Routine Description:

  Parses the application load options and selects operating modes.

Arguments:

  ImageHandle  - Input parameter.
  ReportMode  - Output parameter.
  HelpMode  - Output parameter.

Returns:

  None.

--*/
STATIC
VOID
ParseCommandLine (
  IN  EFI_HANDLE  ImageHandle,
  OUT BOOLEAN     *ReportMode,
  OUT BOOLEAN     *HelpMode
  )
{
  EFI_STATUS  Status;
  CHAR16      *LoadOptionsText;
  CHAR16      *Ptr;
  CHAR16      *Token;
  CHAR16      SavedChar;

  *ReportMode = FALSE;
  *HelpMode   = FALSE;

  Status = GetLoadOptionsCopy (ImageHandle, &LoadOptionsText);
  if (EFI_ERROR (Status)) {
    return;
  }

  Ptr = LoadOptionsText;
  while (*Ptr != L'\0') {
    while (IsWhiteSpaceChar (*Ptr)) {
      Ptr++;
    }

    if (*Ptr == L'\0') {
      break;
    }

    Token = Ptr;
    while ((*Ptr != L'\0') && !IsWhiteSpaceChar (*Ptr)) {
      Ptr++;
    }

    SavedChar = *Ptr;
    *Ptr      = L'\0';

    if (StriEq (Token, L"-r") ||
        StriEq (Token, L"--report") ||
        StriEq (Token, L"/report") ||
        StriEq (Token, L"report"))
    {
      *ReportMode = TRUE;
    } else if (StriEq (Token, L"-h") ||
               StriEq (Token, L"--help") ||
               StriEq (Token, L"/?") ||
               StriEq (Token, L"help"))
    {
      *HelpMode = TRUE;
    }

    *Ptr = SavedChar;
    if (SavedChar == L'\0') {
      break;
    }
  }

  FreePool (LoadOptionsText);
}

/*++

Routine Description:

  Prints the application usage information.

Arguments:

  None.

Returns:

  None.

--*/
STATIC
VOID
PrintUsage (
  VOID
  )
{
  LOG (L"Usage: PcieOptionRomArchAudit.efi [--report|-r]\n");
  LOG (L"\n");
  LOG (L"Default mode: scan live PCIe devices with option ROMs and report HITs.\n");
  LOG (L"Report mode : print a full report for every scanned PCIe device, whether it has\n");
  LOG (L"              an option ROM, and whether that ROM contains IA32, X64, AARCH64,\n");
  LOG (L"              EBC, or other UEFI driver images.\n");
}

/*++

Routine Description:

  Reads a PCI device standard configuration header.

Arguments:

  PciIo  - Input parameter.
  Pci  - Output parameter.

Returns:

  EFI_SUCCESS           - The operation completed successfully.
  Error status           - The operation failed.

--*/
STATIC
EFI_STATUS
ReadPciConfigHeader (
  IN  EFI_PCI_IO_PROTOCOL  *PciIo,
  OUT PCI_TYPE_GENERIC     *Pci
  )
{
  return PciIo->Pci.Read (
                 PciIo,
                 EfiPciIoWidthUint8,
                 0,
                 sizeof (*Pci),
                 Pci
                 );
}

/*++

Routine Description:

  Determines whether a PCI device exposes the PCI Express capability.

Arguments:

  PciIo  - Input parameter.
  Pci  - Input parameter.

Returns:

  TRUE                  - The condition is met.
  FALSE                 - The condition is not met.

--*/
STATIC
BOOLEAN
IsPciExpressDevice (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN PCI_TYPE_GENERIC     *Pci
  )
{
  EFI_STATUS              Status;
  EFI_PCI_CAPABILITY_HDR  CapHdr;
  UINT8                   Ptr;
  UINTN                   HopCount;

  if ((Pci->Device.Hdr.Status & EFI_PCI_STATUS_CAPABILITY) == 0) {
    return FALSE;
  }

  switch (Pci->Device.Hdr.HeaderType & HEADER_LAYOUT_CODE) {
    case HEADER_TYPE_DEVICE:
      Ptr = Pci->Device.Device.CapabilityPtr;
      break;
    case HEADER_TYPE_PCI_TO_PCI_BRIDGE:
      Ptr = Pci->Bridge.Bridge.CapabilityPtr;
      break;
    default:
      return FALSE;
  }

  Ptr = (UINT8)(Ptr & ~0x3U);
  for (HopCount = 0; (Ptr >= 0x40) && (HopCount < 64); HopCount++) {
    Status = PciIo->Pci.Read (
                     PciIo,
                     EfiPciIoWidthUint8,
                     Ptr,
                     sizeof (CapHdr),
                     &CapHdr
                     );
    if (EFI_ERROR (Status)) {
      return FALSE;
    }

    if (CapHdr.CapabilityID == EFI_PCI_CAPABILITY_ID_PCIEXP) {
      return TRUE;
    }

    if ((CapHdr.NextItemPtr == 0) || (CapHdr.NextItemPtr == Ptr)) {
      break;
    }

    Ptr = (UINT8)(CapHdr.NextItemPtr & ~0x3U);
  }

  return FALSE;
}

/*++

Routine Description:

  Converts a controller device path to displayable text.

Arguments:

  Handle  - Input parameter.

Returns:

  Allocated device path text, or NULL if it is unavailable.

--*/
STATIC
CHAR16 *
GetDevicePathText (
  IN EFI_HANDLE  Handle
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;

  DevicePath = DevicePathFromHandle (Handle);
  if (DevicePath == NULL) {
    return NULL;
  }

  return ConvertDevicePathToText (DevicePath, FALSE, TRUE);
}

/*++

Routine Description:

  Gets a controller name from a driver component-name protocol.

Arguments:

  DriverHandle  - Input parameter.
  ControllerHandle  - Input parameter.
  ControllerName  - Output parameter.

Returns:

  EFI_SUCCESS           - The operation completed successfully.
  Error status           - The operation failed.

--*/
STATIC
EFI_STATUS
TryGetControllerNameFromDriverHandle (
  IN  EFI_HANDLE  DriverHandle,
  IN  EFI_HANDLE  ControllerHandle,
  IN  EFI_HANDLE  ChildHandle,
  OUT CHAR16      **ControllerName
  )
{
  EFI_STATUS                   Status;
  EFI_COMPONENT_NAME2_PROTOCOL *ComponentName2;
  EFI_COMPONENT_NAME_PROTOCOL  *ComponentName;
  CHAR8                        Language[MAX_RFC4646_LANGUAGE_LEN];

  if (ControllerName == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *ControllerName = NULL;

  Status = gBS->HandleProtocol (
                  DriverHandle,
                  &gEfiComponentName2ProtocolGuid,
                  (VOID **)&ComponentName2
                  );
  if (!EFI_ERROR (Status) &&
      (ComponentName2 != NULL) &&
      (ComponentName2->GetControllerName != NULL) &&
      SelectEnglishRfc4646Language (ComponentName2->SupportedLanguages, Language, sizeof (Language)))
  {
    Status = ComponentName2->GetControllerName (
                               ComponentName2,
                               ControllerHandle,
                               ChildHandle,
                               Language,
                               ControllerName
                               );
    if (!EFI_ERROR (Status) && (*ControllerName != NULL)) {
      return EFI_SUCCESS;
    }
  }

  Status = gBS->HandleProtocol (
                  DriverHandle,
                  &gEfiComponentNameProtocolGuid,
                  (VOID **)&ComponentName
                  );
  if (!EFI_ERROR (Status) &&
      (ComponentName != NULL) &&
      (ComponentName->GetControllerName != NULL) &&
      SupportsEnglishIso639_2 (ComponentName->SupportedLanguages))
  {
    Status = ComponentName->GetControllerName (
                              ComponentName,
                              ControllerHandle,
                              ChildHandle,
                              "eng",
                              ControllerName
                              );
    if (!EFI_ERROR (Status) && (*ControllerName != NULL)) {
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/*++

Routine Description:

  Finds a human-readable name for a PCI controller.

Arguments:

  ControllerHandle  - Input parameter.
  ControllerName  - Output parameter.

Returns:

  EFI_SUCCESS           - The operation completed successfully.
  Error status           - The operation failed.

--*/
STATIC
EFI_STATUS
GetControllerName (
  IN  EFI_HANDLE  ControllerHandle,
  OUT CHAR16      **ControllerName
  )
{
  EFI_STATUS                           Status;
  EFI_OPEN_PROTOCOL_INFORMATION_ENTRY  *OpenInfo;
  UINTN                                EntryCount;
  UINTN                                Index;
  EFI_HANDLE                           *HandleBuffer;
  UINTN                                HandleCount;

  if (ControllerName == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *ControllerName = NULL;
  OpenInfo       = NULL;
  EntryCount     = 0;

  Status = gBS->OpenProtocolInformation (
                  ControllerHandle,
                  &gEfiPciIoProtocolGuid,
                  &OpenInfo,
                  &EntryCount
                  );
  if (!EFI_ERROR (Status) && (OpenInfo != NULL)) {
    for (Index = 0; Index < EntryCount; Index++) {
      if ((OpenInfo[Index].Attributes & EFI_OPEN_PROTOCOL_BY_DRIVER) == 0) {
        continue;
      }

      Status = TryGetControllerNameFromDriverHandle (
                 OpenInfo[Index].AgentHandle,
                 ControllerHandle,
                 NULL,
                 ControllerName
                 );
      if (!EFI_ERROR (Status) && (*ControllerName != NULL)) {
        FreePool (OpenInfo);
        return EFI_SUCCESS;
      }
    }

    for (Index = 0; Index < EntryCount; Index++) {
      if (((OpenInfo[Index].Attributes & EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER) == 0) ||
          (OpenInfo[Index].ControllerHandle == NULL))
      {
        continue;
      }

      Status = TryGetControllerNameFromDriverHandle (
                 OpenInfo[Index].AgentHandle,
                 ControllerHandle,
                 OpenInfo[Index].ControllerHandle,
                 ControllerName
                 );
      if (!EFI_ERROR (Status) && (*ControllerName != NULL)) {
        FreePool (OpenInfo);
        return EFI_SUCCESS;
      }
    }

    FreePool (OpenInfo);
  }

  HandleBuffer = NULL;
  HandleCount  = 0;
  Status       = gBS->LocateHandleBuffer (
                        ByProtocol,
                        &gEfiComponentName2ProtocolGuid,
                        NULL,
                        &HandleCount,
                        &HandleBuffer
                        );
  if (!EFI_ERROR (Status) && (HandleBuffer != NULL)) {
    for (Index = 0; Index < HandleCount; Index++) {
      Status = TryGetControllerNameFromDriverHandle (
                 HandleBuffer[Index],
                 ControllerHandle,
                 NULL,
                 ControllerName
                 );
      if (!EFI_ERROR (Status) && (*ControllerName != NULL)) {
        FreePool (HandleBuffer);
        return EFI_SUCCESS;
      }
    }

    FreePool (HandleBuffer);
  }

  HandleBuffer = NULL;
  HandleCount  = 0;
  Status       = gBS->LocateHandleBuffer (
                        ByProtocol,
                        &gEfiComponentNameProtocolGuid,
                        NULL,
                        &HandleCount,
                        &HandleBuffer
                        );
  if (!EFI_ERROR (Status) && (HandleBuffer != NULL)) {
    for (Index = 0; Index < HandleCount; Index++) {
      Status = TryGetControllerNameFromDriverHandle (
                 HandleBuffer[Index],
                 ControllerHandle,
                 NULL,
                 ControllerName
                 );
      if (!EFI_ERROR (Status) && (*ControllerName != NULL)) {
        FreePool (HandleBuffer);
        return EFI_SUCCESS;
      }
    }

    FreePool (HandleBuffer);
  }

  return EFI_NOT_FOUND;
}

/*++

Routine Description:

  Prints the device path and controller name for a PCI device.

Arguments:

  ControllerHandle  - Input parameter.
  ControllerName    - Optional output controller name.

Returns:

  None.

--*/
STATIC
VOID
PrintDeviceContext (
  IN  EFI_HANDLE  ControllerHandle,
  OUT CHAR16      **ControllerName OPTIONAL
  )
{
  CHAR16      *DevicePathText;
  CHAR16      *ResolvedControllerName;
  EFI_STATUS  Status;

  if (ControllerName != NULL) {
    *ControllerName = NULL;
  }

  DevicePathText = GetDevicePathText (ControllerHandle);
  if (DevicePathText != NULL) {
    LOG (L"    DevicePath = %s\n", DevicePathText);
    FreePool (DevicePathText);
  } else {
    LOG (L"    DevicePath = <unavailable>\n");
  }

  ResolvedControllerName = NULL;
  Status                 = GetControllerName (ControllerHandle, &ResolvedControllerName);
  if (!EFI_ERROR (Status) && (ResolvedControllerName != NULL)) {
    LOG (L"    DeviceName = %s\n", ResolvedControllerName);
    if (ControllerName != NULL) {
      *ControllerName = ResolvedControllerName;
    }
  } else {
    LOG (L"    DeviceName = <not found>\n");
  }
}

/*++

Routine Description:

  Parses an option ROM and classifies its legacy and UEFI images.

Arguments:

  Rom  - Input parameter.
  RomSize  - Input parameter.
  Audit  - Output parameter.

Returns:

  EFI_SUCCESS           - The operation completed successfully.
  Error status           - The operation failed.

--*/
STATIC
EFI_STATUS
AuditOptionRom (
  IN  UINT8      *Rom,
  IN  UINTN      RomSize,
  OUT ROM_AUDIT  *Audit
  )
{
  EFI_PCI_ROM_HEADER  RomHdr;
  PCI_DATA_STRUCTURE  *Pcir;
  UINT8               *RomPtr;
  UINT8               *RomEnd;
  UINTN               ImageSize;
  UINTN               Offset;
  UINTN               ImageIndex;
  UINT16              MachineType;
  UINT16              Subsystem;
  BOOLEAN             Compressed;
  BOOLEAN             LastImageFound;

  if ((Rom == NULL) || (Audit == NULL) || (RomSize < sizeof (PCI_EXPANSION_ROM_HEADER))) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (Audit, sizeof (*Audit));
  RomPtr     = Rom;
  RomEnd     = Rom + RomSize;
  ImageIndex     = 0;
  LastImageFound = FALSE;

  while ((UINTN)(RomEnd - RomPtr) >= sizeof (PCI_EXPANSION_ROM_HEADER)) {
    Offset     = (UINTN)(RomPtr - Rom);
    RomHdr.Raw = RomPtr;

    if (RomHdr.Generic->Signature != PCI_EXPANSION_ROM_HEADER_SIGNATURE) {
      return EFI_COMPROMISED_DATA;
    }

    if ((UINTN)RomHdr.Generic->PcirOffset > ((UINTN)(RomEnd - RomPtr) - sizeof (PCI_DATA_STRUCTURE))) {
      return EFI_COMPROMISED_DATA;
    }

    Pcir = (PCI_DATA_STRUCTURE *)(RomPtr + RomHdr.Generic->PcirOffset);
    if (Pcir->Signature != PCI_DATA_STRUCTURE_SIGNATURE) {
      return EFI_COMPROMISED_DATA;
    }

    ImageSize = (UINTN)Pcir->ImageLength * 512U;
    if ((Pcir->ImageLength == 0) || (ImageSize > (UINTN)(RomEnd - RomPtr))) {
      return EFI_COMPROMISED_DATA;
    }

    if (((UINTN)RomHdr.Generic->PcirOffset + sizeof (PCI_DATA_STRUCTURE)) > ImageSize) {
      return EFI_COMPROMISED_DATA;
    }

    Audit->ImageCount++;

    switch (Pcir->CodeType) {
      case PCI_CODE_TYPE_PCAT_IMAGE:
        Audit->HasLegacyX86 = TRUE;
        LOG (
          L"    image[%u] off=0x%lx size=0x%lx legacy-x86\n",
          (UINT32)ImageIndex,
          (UINT64)Offset,
          (UINT64)ImageSize
          );
        break;

      case PCI_CODE_TYPE_EFI_IMAGE:
        if (ImageSize < sizeof (EFI_PCI_EXPANSION_ROM_HEADER)) {
          return EFI_COMPROMISED_DATA;
        }

        if (RomHdr.Efi->EfiSignature != EFI_PCI_EXPANSION_ROM_HEADER_EFISIGNATURE) {
          return EFI_COMPROMISED_DATA;
        }

        MachineType = RomHdr.Efi->EfiMachineType;
        Subsystem   = RomHdr.Efi->EfiSubsystem;
        Compressed  = (BOOLEAN)(RomHdr.Efi->CompressionType != 0);

        Audit->EfiImageCount++;

        if (IsKnownMachineType (MachineType)) {
          LOG (
            L"    image[%u] off=0x%lx size=0x%lx efi machine=%s subsystem=%s compressed=%s\n",
            (UINT32)ImageIndex,
            (UINT64)Offset,
            (UINT64)ImageSize,
            MachineTypeToStr (MachineType),
            SubsystemToStr (Subsystem),
            Compressed ? L"yes" : L"no"
            );
        } else {
          LOG (
            L"    image[%u] off=0x%lx size=0x%lx efi machine=%s(0x%04x) subsystem=%s compressed=%s\n",
            (UINT32)ImageIndex,
            (UINT64)Offset,
            (UINT64)ImageSize,
            MachineTypeToStr (MachineType),
            (UINT32)MachineType,
            SubsystemToStr (Subsystem),
            Compressed ? L"yes" : L"no"
            );
        }

        if (IsDriverSubsystem (Subsystem)) {
          Audit->DriverImageCount++;
          switch (MachineType) {
            case IMAGE_FILE_MACHINE_I386:
              Audit->HasIa32Uefi = TRUE;
              break;
            case IMAGE_FILE_MACHINE_X64:
              Audit->HasX64Uefi = TRUE;
              break;
            case IMAGE_FILE_MACHINE_ARM64:
              Audit->HasArm64Uefi = TRUE;
              break;
            case IMAGE_FILE_MACHINE_EBC:
              Audit->HasEbcUefi = TRUE;
              break;
            default:
              Audit->HasOtherUefi = TRUE;
              break;
          }
        }
        break;

      default:
        LOG (
          L"    image[%u] off=0x%lx size=0x%lx code-type=0x%x\n",
          (UINT32)ImageIndex,
          (UINT64)Offset,
          (UINT64)ImageSize,
          (UINT32)Pcir->CodeType
          );
        break;
    }

    ImageIndex++;
    if (IS_LAST_ROM_IMAGE (Pcir)) {
      LastImageFound = TRUE;
      break;
    }

    RomPtr += ImageSize;
  }

  if (!LastImageFound) {
    return EFI_COMPROMISED_DATA;
  }

  return EFI_SUCCESS;
}

/*++

Routine Description:

  Updates aggregate statistics for a successfully parsed option ROM.

Arguments:

  Stats  - Input and output parameter.
  Audit  - Input parameter.
  Hit  - Input parameter.

Returns:

  None.

--*/
STATIC
VOID
AccumulateRunStats (
  IN OUT RUN_STATS   *Stats,
  IN     ROM_AUDIT   *Audit,
  IN     BOOLEAN     Hit
  )
{
  if ((Stats == NULL) || (Audit == NULL)) {
    return;
  }

  if (Audit->HasLegacyX86) {
    Stats->DevicesWithLegacyX86++;
  }

  if (Audit->HasIa32Uefi) {
    Stats->DevicesWithIa32Uefi++;
  }

  if (Audit->HasX64Uefi) {
    Stats->DevicesWithX64Uefi++;
  }

  if (Audit->HasArm64Uefi) {
    Stats->DevicesWithArm64Uefi++;
  }

  if (Audit->HasEbcUefi) {
    Stats->DevicesWithEbcUefi++;
  }

  if (Audit->HasOtherUefi) {
    Stats->DevicesWithOtherUefi++;
  }

  if (Hit) {
    Stats->Hits++;
  }
}

/*++

Routine Description:

  Prints the architecture classification for one option ROM.

Arguments:

  Audit  - Input parameter.

Returns:

  None.

--*/
STATIC
VOID
PrintRomSummary (
  IN CONST ROM_AUDIT  *Audit
  )
{
  LOG (
    L"    summary: legacy-x86=%u ia32-uefi=%u x64-uefi=%u arm64-uefi=%u ebc-uefi=%u other-uefi=%u\n",
    Audit->HasLegacyX86 ? 1U : 0U,
    Audit->HasIa32Uefi ? 1U : 0U,
    Audit->HasX64Uefi ? 1U : 0U,
    Audit->HasArm64Uefi ? 1U : 0U,
    Audit->HasEbcUefi ? 1U : 0U,
    Audit->HasOtherUefi ? 1U : 0U
    );
}

/*++

Routine Description:

  Prints aggregate results for the completed audit.

Arguments:

  Stats  - Input parameter.

Returns:

  None.

--*/
STATIC
VOID
PrintFinalSummary (
  IN CONST RUN_STATS             *Stats,
  IN CONST NON_COMPLIANT_DEVICE  *NonCompliantDevices,
  IN UINTN                       NonCompliantDeviceCount
  )
{
  UINTN  Index;

  LOG (L"Scanned EFI_PCI_IO handles : %u\n", (UINT32)Stats->ScannedHandles);
  LOG (L"PCIe devices considered    : %u\n", (UINT32)Stats->PcieDevices);
  LOG (L"PCIe devices with ROM      : %u\n", (UINT32)Stats->DevicesWithRom);
  LOG (L"Devices with legacy x86    : %u\n", (UINT32)Stats->DevicesWithLegacyX86);
  LOG (L"Devices with IA32 UEFI     : %u\n", (UINT32)Stats->DevicesWithIa32Uefi);
  LOG (L"Devices with X64 UEFI      : %u\n", (UINT32)Stats->DevicesWithX64Uefi);
  LOG (L"Devices with AARCH64 UEFI  : %u\n", (UINT32)Stats->DevicesWithArm64Uefi);
  LOG (L"Devices with EBC UEFI      : %u\n", (UINT32)Stats->DevicesWithEbcUefi);
  LOG (L"Devices with other UEFI    : %u\n", (UINT32)Stats->DevicesWithOtherUefi);
  LOG (L"ROM parse errors           : %u\n", (UINT32)Stats->RomParseErrors);
  LOG (L"Inspection errors          : %u\n", (UINT32)Stats->InspectionErrors);
  LOG (L"Non-compliant devices      : %u\n", (UINT32)Stats->Hits);

  LOG (L"\n");
  if (Stats->DevicesWithRom == 0) {
    LOG (
      L"BBR (UEFI 6.3.3.1 UEFI Drivers) Result Summary: SKIP\n"
      );
    LOG (
      L"Reason: No PCIe devices with Option ROM were found.\n"
      );
  } else if (Stats->Hits != 0) {
    LOG (
      L"BBR (UEFI 6.3.3.1 UEFI Drivers) Result Summary: FAIL\n"
      );
    LOG (L"\n----------------------------------------\n");
    for (Index = 0; Index < NonCompliantDeviceCount; Index++) {
      LOG (
        L"  BDF=%04x:%02x:%02x.%x %s [%04x:%04x] DeviceName=%s\n",
        (UINT32)NonCompliantDevices[Index].Segment,
        (UINT32)NonCompliantDevices[Index].Bus,
        (UINT32)NonCompliantDevices[Index].Device,
        (UINT32)NonCompliantDevices[Index].Function,
        VendorIdToStr (NonCompliantDevices[Index].VendorId),
        (UINT32)NonCompliantDevices[Index].VendorId,
        (UINT32)NonCompliantDevices[Index].DeviceId,
        NonCompliantDevices[Index].DeviceName
        );
      LOG (
        L"    Reason: UEFI driver image(s) present but no "
        L"AARCH64 UEFI driver found\n"
        );
    }
  } else {
    LOG (
      L"BBR (UEFI 6.3.3.1 UEFI Drivers) Result Summary: PASS\n"
      );
  }
}

/*++

Routine Description:

  Audits live PCIe option ROMs for their embedded UEFI driver architectures.

Arguments:

  ImageHandle  - Input parameter.
  SystemTable  - Input parameter.

Returns:

  EFI_SUCCESS           - The operation completed successfully.
  Error status           - The operation failed.

--*/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS           Status;
  EFI_HANDLE           *HandleBuffer;
  EFI_PCI_IO_PROTOCOL  *PciIo;
  PCI_TYPE_GENERIC     Pci;
  ROM_AUDIT            Audit;
  RUN_STATS            Stats;
  UINTN                HandleCount;
  UINTN                Index;
  UINTN                Segment;
  UINTN                Bus;
  UINTN                Device;
  UINTN                Function;
  BOOLEAN              ReportMode;
  BOOLEAN              HelpMode;
  BOOLEAN              Hit;
  CHAR16               *ControllerName;
  NON_COMPLIANT_DEVICE *NonCompliantDevices;
  NON_COMPLIANT_DEVICE *NonCompliantDevice;
  UINTN                NonCompliantDeviceCount;
  CONST CHAR16           *VendorName;
  CONST CHAR16           *ClassName;

  Status = OpenLogFile ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  LOG (L"Running PcieOptionRomArchAudit standalone app\n");

  ParseCommandLine (ImageHandle, &ReportMode, &HelpMode);
  if (HelpMode) {
    PrintUsage ();
    ShellCloseFile (&mLogFile);
    return EFI_SUCCESS;
  }

  ZeroMem (&Stats, sizeof (Stats));
  HandleBuffer = NULL;
  HandleCount  = 0;
  NonCompliantDevices     = NULL;
  NonCompliantDeviceCount = 0;

  if (ReportMode) {
    LOG (L"PcieOptionRomArchAudit: report all live PCIe devices and classify option ROM contents\n\n");
  } else {
    LOG (
      L"PcieOptionRomArchAudit: scan live PCIe devices for UEFI driver "
      L"images (IA32/X64/AARCH64/EBC) and list devices without an "
      L"AARCH64 UEFI driver\n\n"
      );
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    LOG (L"LocateHandleBuffer(EFI_PCI_IO_PROTOCOL) failed: %r\n", Status);
    ShellCloseFile (&mLogFile);
    return Status;
  }

  //
  // Count the PCIe devices that expose an option ROM before printing the
  // detailed audit output.  This provides an accurate audit heading without
  // buffering the per-device log entries.
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Stats.ScannedHandles++;

    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = ReadPciConfigHeader (PciIo, &Pci);
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (!IsPciExpressDevice (PciIo, &Pci)) {
      continue;
    }

    Stats.PcieDevices++;
    if ((PciIo->RomImage != NULL) && (PciIo->RomSize != 0)) {
      Stats.DevicesWithRom++;
    }
  }

  LOG (
    L"Scanned %u EFI_PCI_IO handles and found %u PCIe devices with Option ROM images:\n\n",
    (UINT32)Stats.ScannedHandles,
    (UINT32)Stats.DevicesWithRom
    );

  NonCompliantDevices = AllocateZeroPool (
                          HandleCount * sizeof (*NonCompliantDevices)
                          );
  if ((HandleCount != 0) && (NonCompliantDevices == NULL)) {
    FreePool (HandleBuffer);
    ShellCloseFile (&mLogFile);
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIo
                    );
    if (EFI_ERROR (Status)) {
      Stats.InspectionErrors++;
      continue;
    }

    Status = PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
    if (EFI_ERROR (Status)) {
      Stats.InspectionErrors++;
      continue;
    }

    Status = ReadPciConfigHeader (PciIo, &Pci);
    if (EFI_ERROR (Status)) {
      LOG (
        L"%04x:%02x:%02x.%x config read failed: %r\n",
        (UINT32)Segment,
        (UINT32)Bus,
        (UINT32)Device,
        (UINT32)Function,
        Status
        );
      Stats.InspectionErrors++;
      continue;
    }

    if (!IsPciExpressDevice (PciIo, &Pci)) {
      continue;
    }

    if (!ReportMode && ((PciIo->RomImage == NULL) || (PciIo->RomSize == 0))) {
      continue;
    }
    VendorName = VendorIdToStr (Pci.Device.Hdr.VendorId);
    ClassName  = PciClassCodeToStr (
                   Pci.Device.Hdr.ClassCode[2],
                   Pci.Device.Hdr.ClassCode[1],
                   Pci.Device.Hdr.ClassCode[0]
                   );

    if ((PciIo->RomImage == NULL) || (PciIo->RomSize == 0)) {
      LOG (
        L"%04x:%02x:%02x.%x %s [%04x:%04x] class=%s (%02x%02x%02x) rom=none\n",
        (UINT32)Segment,
        (UINT32)Bus,
        (UINT32)Device,
        (UINT32)Function,
        VendorName,
        (UINT32)Pci.Device.Hdr.VendorId,
        (UINT32)Pci.Device.Hdr.DeviceId,
        ClassName,
        (UINT32)Pci.Device.Hdr.ClassCode[2],
        (UINT32)Pci.Device.Hdr.ClassCode[1],
        (UINT32)Pci.Device.Hdr.ClassCode[0]
        );
    } else {
      LOG (
        L"%04x:%02x:%02x.%x %s [%04x:%04x] class=%s (%02x%02x%02x) rom=0x%lx\n",
        (UINT32)Segment,
        (UINT32)Bus,
        (UINT32)Device,
        (UINT32)Function,
        VendorName,
        (UINT32)Pci.Device.Hdr.VendorId,
        (UINT32)Pci.Device.Hdr.DeviceId,
        ClassName,
        (UINT32)Pci.Device.Hdr.ClassCode[2],
        (UINT32)Pci.Device.Hdr.ClassCode[1],
        (UINT32)Pci.Device.Hdr.ClassCode[0],
        (UINT64)PciIo->RomSize
        );
    }

    ControllerName = NULL;
    PrintDeviceContext (HandleBuffer[Index], &ControllerName);

    if ((PciIo->RomImage == NULL) || (PciIo->RomSize == 0)) {
      if (ReportMode) {
        LOG (L"    summary: no option ROM\n\n");
      } else {
        LOG (L"\n");
      }

      continue;
    }

    Status = AuditOptionRom ((UINT8 *)PciIo->RomImage, (UINTN)PciIo->RomSize, &Audit);
    if (EFI_ERROR (Status)) {
      Stats.RomParseErrors++;
      LOG (L"    ROM parse failed: %r\n\n", Status);
      continue;
    }

    Hit = (BOOLEAN)(
            (Audit.DriverImageCount != 0) &&
            !Audit.HasArm64Uefi
            );
    AccumulateRunStats (&Stats, &Audit, Hit);
    PrintRomSummary (&Audit);

    if (Hit && (NonCompliantDevices != NULL) && (NonCompliantDeviceCount < HandleCount)) {
      NonCompliantDevice           = &NonCompliantDevices[NonCompliantDeviceCount];
      NonCompliantDevice->Segment  = Segment;
      NonCompliantDevice->Bus      = Bus;
      NonCompliantDevice->Device   = Device;
      NonCompliantDevice->Function = Function;
      NonCompliantDevice->VendorId = Pci.Device.Hdr.VendorId;
      NonCompliantDevice->DeviceId = Pci.Device.Hdr.DeviceId;
      if (ControllerName != NULL) {
        StrnCpyS (
          NonCompliantDevice->DeviceName,
          sizeof (NonCompliantDevice->DeviceName) / sizeof (NonCompliantDevice->DeviceName[0]),
          ControllerName,
          (sizeof (NonCompliantDevice->DeviceName) / sizeof (NonCompliantDevice->DeviceName[0])) - 1
          );
      } else {
        StrCpyS (
          NonCompliantDevice->DeviceName,
          sizeof (NonCompliantDevice->DeviceName) / sizeof (NonCompliantDevice->DeviceName[0]),
          L"<not found>"
          );
      }

      NonCompliantDeviceCount++;
    }

    if (Hit) {
      LOG (
        L"    HIT: UEFI driver image(s) (IA32/X64/EBC) present but no "
        L"AARCH64 UEFI driver found\n"
        );
    }

    LOG (L"\n");
  }

  PrintFinalSummary (&Stats, NonCompliantDevices, NonCompliantDeviceCount);

  if (NonCompliantDevices != NULL) {
    FreePool (NonCompliantDevices);
  }

  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }

  //
  // Return status:
  //   EFI_SUCCESS          - All audited devices are compliant.
  //   EFI_UNSUPPORTED      - One or more non-compliant option ROMs found.
  //   EFI_COMPROMISED_DATA - Malformed option ROM encountered.
  //   EFI_DEVICE_ERROR     - One or more devices could not be inspected.
  //
  if (Stats.RomParseErrors != 0) {
    Status = EFI_COMPROMISED_DATA;
  } else if (Stats.InspectionErrors != 0) {
    Status = EFI_DEVICE_ERROR;
  } else {
    Status = (Stats.Hits != 0) ? EFI_UNSUPPORTED : EFI_SUCCESS;
  }

  ShellCloseFile (&mLogFile);
  return Status;
}

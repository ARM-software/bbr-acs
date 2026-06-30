/** @file

  Copyright 2006-2016 Unified EFI, Inc.<BR>
  Copyright (c) 2026, Arm Ltd. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
/*++

Module Name:
  UefiDump.c

Abstract:
  Source file for the UEFI dump application, which currently dumps the EBBR conformance profiles table.

--*/

#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ShellLib.h>
#include <Guid/ConformanceProfiles.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>
#include "UefiDump.h"

#define MAX_CONFORMANCE_PROFILE_COUNT  256

/*++

Routine Description:

  Locates a system configuration table entry by GUID.

Arguments:

  TableGuid  - Pointer to the GUID of the configuration table.
  Table      - On success, receives the VendorTable pointer.

Returns:

  EFI_SUCCESS           - Table was found.
  EFI_INVALID_PARAMETER - TableGuid or Table is NULL.
  EFI_NOT_FOUND         - Table was not found.

--*/
STATIC
EFI_STATUS
GetSystemConfigurationTable(
  IN  EFI_GUID  *TableGuid,
  OUT VOID      **Table
  )
{
  UINTN                  Index;
  EFI_CONFIGURATION_TABLE *ConfigTable;

  if (TableGuid == NULL || Table == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ConfigTable = gST->ConfigurationTable;
  for (Index = 0; Index < gST->NumberOfTableEntries; Index++) {
    if (CompareGuid(TableGuid, &ConfigTable[Index].VendorGuid)) {
      *Table = ConfigTable[Index].VendorTable;
      return EFI_SUCCESS;
    }
  }

  *Table = NULL;
  return EFI_NOT_FOUND;
}

/*++

Routine Description:

  Writes a binary buffer to a file on the loaded image's device.

Arguments:

  ImageHandle - The loaded image handle.
  FileName    - The path of the file to create or overwrite.
  Buffer      - The data buffer to write.
  BufferSize  - The number of bytes to write.

Returns:

  EFI_SUCCESS           - The file was written successfully.
  EFI_INVALID_PARAMETER - FileName is NULL, Buffer is NULL, or BufferSize is zero.
  Other EFI_STATUS      - Returned from underlying protocol operations.

--*/
STATIC
EFI_STATUS
GetCurrentDirectoryFallbackPath (
  OUT CHAR16  **FallbackPath
  )
{
  CONST CHAR16  *CurrentDirectory;
  CONST CHAR16  *VolumeRelativeDirectory;
  CONST CHAR16  *MappingSeparator;
  EFI_STATUS     Status;
  CHAR16        *PathBuffer;
  UINTN          PathSize;
  BOOLEAN        NeedsSeparator;

  if (FallbackPath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *FallbackPath = NULL;
  CurrentDirectory = ShellGetCurrentDir (NULL);
  if (CurrentDirectory == NULL) {
    return EFI_NOT_FOUND;
  }

  VolumeRelativeDirectory = CurrentDirectory;
  MappingSeparator = StrStr (CurrentDirectory, L":");
  if (MappingSeparator != NULL) {
    VolumeRelativeDirectory = MappingSeparator + 1;
  }

  NeedsSeparator = ((*VolumeRelativeDirectory != CHAR_NULL) &&
                    (VolumeRelativeDirectory[StrLen (VolumeRelativeDirectory) - 1] != L'\\'));
  PathSize = StrSize (VolumeRelativeDirectory) +
             StrSize (EBBR_PROFILE_TABLE_DUMP_FALLBACK_FILENAME);
  if ((*VolumeRelativeDirectory == CHAR_NULL) || NeedsSeparator) {
    PathSize += sizeof (CHAR16);
  }

  Status = gBS->AllocatePool (EfiBootServicesData, PathSize, (VOID **)&PathBuffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (*VolumeRelativeDirectory == CHAR_NULL) {
    UnicodeSPrint (
      PathBuffer,
      PathSize,
      L"\\%s",
      EBBR_PROFILE_TABLE_DUMP_FALLBACK_FILENAME
      );
  } else if (NeedsSeparator) {
    UnicodeSPrint (
      PathBuffer,
      PathSize,
      L"%s\\%s",
      VolumeRelativeDirectory,
      EBBR_PROFILE_TABLE_DUMP_FALLBACK_FILENAME
      );
  } else {
    UnicodeSPrint (
      PathBuffer,
      PathSize,
      L"%s%s",
      VolumeRelativeDirectory,
      EBBR_PROFILE_TABLE_DUMP_FALLBACK_FILENAME
      );
  }

  *FallbackPath = PathBuffer;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
WriteBinaryFile(
  IN EFI_HANDLE   ImageHandle,
  IN CONST CHAR16 *FileName,
  IN VOID         *Buffer,
  IN UINTN        BufferSize
  )
{
  EFI_STATUS                        Status;
  EFI_LOADED_IMAGE_PROTOCOL        *LoadedImage;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *SimpleFileSystem;
  EFI_FILE_PROTOCOL                *Root;
  EFI_FILE_PROTOCOL                *File;
  EFI_FILE_INFO                    *FileInfo;
  UINTN                             FileInfoSize;
  UINTN                             FileSize;
  CONST CHAR16                     *TargetFileName;
  CHAR16                           *FallbackFilePath;
  BOOLEAN                           UsedFallback;

  if ((FileName == NULL) || (Buffer == NULL) || (BufferSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->HandleProtocol(
                  ImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID **)&LoadedImage
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = gBS->HandleProtocol(
                  LoadedImage->DeviceHandle,
                  &gEfiSimpleFileSystemProtocolGuid,
                  (VOID **)&SimpleFileSystem
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = SimpleFileSystem->OpenVolume(SimpleFileSystem, &Root);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  TargetFileName   = FileName;
  FallbackFilePath = NULL;
  UsedFallback     = FALSE;
  Status           = Root->Open(
                            Root,
                            &File,
                            (CHAR16 *)TargetFileName,
                            EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,
                            0
                            );
  if ((Status == EFI_NOT_FOUND) &&
      (StrCmp(FileName, EBBR_PROFILE_TABLE_DUMP_FALLBACK_FILENAME) != 0)) {
    Status = GetCurrentDirectoryFallbackPath (&FallbackFilePath);
    if (EFI_ERROR (Status)) {
      TargetFileName = EBBR_PROFILE_TABLE_DUMP_FALLBACK_FILENAME;
    } else {
      TargetFileName = FallbackFilePath;
    }

    UsedFallback = TRUE;
    Status       = Root->Open(
                          Root,
                          &File,
                          (CHAR16 *)TargetFileName,
                          EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,
                          0
                          );
  }

  if (EFI_ERROR(Status)) {
    if (FallbackFilePath != NULL) {
      gBS->FreePool (FallbackFilePath);
    }

    Root->Close(Root);
    return Status;
  }

  if (UsedFallback) {
    Print(
      L"EBBR Profile Table Dump: %s not found, writing %s in the current directory\n",
      FileName,
      TargetFileName
      );
  }

  Status = File->SetPosition(File, 0);
  if (EFI_ERROR(Status)) {
    File->Close(File);
    Root->Close(Root);
    if (FallbackFilePath != NULL) {
      gBS->FreePool (FallbackFilePath);
    }

    return Status;
  }

  FileInfo = NULL;
  FileInfoSize = SIZE_OF_EFI_FILE_INFO + 200;
  Status = gBS->AllocatePool(EfiBootServicesData, FileInfoSize, (VOID **)&FileInfo);
  if (EFI_ERROR(Status)) {
    File->Close(File);
    Root->Close(Root);
    if (FallbackFilePath != NULL) {
      gBS->FreePool (FallbackFilePath);
    }

    return Status;
  }

  Status = File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
  if (EFI_ERROR(Status)) {
    gBS->FreePool(FileInfo);
    File->Close(File);
    Root->Close(Root);
    if (FallbackFilePath != NULL) {
      gBS->FreePool (FallbackFilePath);
    }

    return Status;
  }

  FileInfo->FileSize = 0;
  FileInfo->Size = FileInfoSize;
  Status = File->SetInfo(File, &gEfiFileInfoGuid, FileInfoSize, FileInfo);
  gBS->FreePool(FileInfo);
  if (EFI_ERROR(Status)) {
    File->Close(File);
    Root->Close(Root);
    if (FallbackFilePath != NULL) {
      gBS->FreePool (FallbackFilePath);
    }

    return Status;
  }

  FileSize = BufferSize;
  Status = File->Write(File, &FileSize, Buffer);
  if (EFI_ERROR(Status)) {
    File->Close(File);
    Root->Close(Root);
    if (FallbackFilePath != NULL) {
      gBS->FreePool (FallbackFilePath);
    }

    return Status;
  }

  if (FileSize != BufferSize) {
    File->Close(File);
    Root->Close(Root);
    if (FallbackFilePath != NULL) {
      gBS->FreePool (FallbackFilePath);
    }

    return EFI_DEVICE_ERROR;
  }

  File->Close(File);
  Root->Close(Root);
  if (FallbackFilePath != NULL) {
    gBS->FreePool (FallbackFilePath);
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
WriteHexDumpFile(
  IN EFI_HANDLE ImageHandle,
  IN VOID       *Buffer,
  IN UINTN      BufferSize
  )
{
  EFI_STATUS  Status;
  CHAR8       *Text;
  CHAR8       *Cursor;
  UINT8       *Data;
  UINTN       EstimatedSize;
  UINTN       Offset;
  UINTN       Index;
  UINTN       LineBytes;
  UINTN       Remaining;
  UINTN       MaxGuidCount;
  UINTN       GuidCount;
  UINT16      Version;
  UINT16      NumberOfProfiles;

  if ((Buffer == NULL) || (BufferSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  MaxGuidCount = (BufferSize >= 4) ? ((BufferSize - 4) / sizeof (EFI_GUID)) : 0;
  EstimatedSize = ((((BufferSize + 15) / 16) + 1) * 80) + 128 + (MaxGuidCount * 200) + 1;
  Status = gBS->AllocatePool(EfiBootServicesData, EstimatedSize, (VOID **)&Text);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Cursor = Text;
  Data   = (UINT8 *)Buffer;

  for (Offset = 0; Offset < BufferSize; Offset += 16) {
    LineBytes = ((BufferSize - Offset) >= 16) ? 16 : (BufferSize - Offset);
    Remaining = EstimatedSize - (UINTN)(Cursor - Text);
    Cursor += AsciiSPrint(Cursor, Remaining, "%08X  ", (UINT32)Offset);

    for (Index = 0; Index < 16; Index++) {
      Remaining = EstimatedSize - (UINTN)(Cursor - Text);
      if (Index < LineBytes) {
        Cursor += AsciiSPrint(Cursor, Remaining, "%02X", Data[Offset + Index]);
      } else {
        Cursor += AsciiSPrint(Cursor, Remaining, "  ");
      }

      if (Index == 7) {
        *Cursor++ = ' ';
      }

      if (Index != 15) {
        *Cursor++ = ' ';
      }
    }

    *Cursor++ = ' ';
    *Cursor++ = ' ';
    *Cursor++ = '|';

    for (Index = 0; Index < LineBytes; Index++) {
      UINT8 Byte;

      Byte = Data[Offset + Index];
      *Cursor++ = ((Byte >= 0x20) && (Byte <= 0x7E)) ? (CHAR8)Byte : '.';
    }

    *Cursor++ = '|';
    *Cursor++ = '\n';
  }

  Remaining = EstimatedSize - (UINTN)(Cursor - Text);
  Cursor += AsciiSPrint(Cursor, Remaining, "%08X\n", (UINT32)BufferSize);

  Version = 0;
  NumberOfProfiles = 0;
  GuidCount = 0;
  if (BufferSize >= 4) {
    Version = (UINT16)(Data[0] | (Data[1] << 8));
    NumberOfProfiles = (UINT16)(Data[2] | (Data[3] << 8));
    GuidCount = (NumberOfProfiles <= MaxGuidCount) ? NumberOfProfiles : MaxGuidCount;
  }

  Remaining = EstimatedSize - (UINTN)(Cursor - Text);
  Cursor += AsciiSPrint(Cursor, Remaining, "Type: EBBR Profile Table Information\n");
  Remaining = EstimatedSize - (UINTN)(Cursor - Text);
  Cursor += AsciiSPrint(Cursor, Remaining, "Version: %u\n", (UINT32)Version);
  Remaining = EstimatedSize - (UINTN)(Cursor - Text);
  Cursor += AsciiSPrint(Cursor, Remaining, "NumberOfProfiles: %u\n", (UINT32)NumberOfProfiles);

  Remaining = EstimatedSize - (UINTN)(Cursor - Text);
  Cursor += AsciiSPrint (
              Cursor,
              Remaining,
              "INFO: Extracted %u GUID(s) from EBBR profile table\n",
              (UINT32)GuidCount
              );

  for (Index = 0; Index < GuidCount; Index++) {
    EFI_GUID  GuidValue;
    UINTN     GuidOffset;

    GuidOffset = 4 + (Index * sizeof (EFI_GUID));
    CopyMem (&GuidValue, Data + GuidOffset, sizeof (GuidValue));

    Remaining = EstimatedSize - (UINTN)(Cursor - Text);
    Cursor += AsciiSPrint (
                Cursor,
                Remaining,
                "INFO: EBBR profile table GUID[%u] : %08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
                (UINT32)Index,
                GuidValue.Data1,
                GuidValue.Data2,
                GuidValue.Data3,
                GuidValue.Data4[0],
                GuidValue.Data4[1],
                GuidValue.Data4[2],
                GuidValue.Data4[3],
                GuidValue.Data4[4],
                GuidValue.Data4[5],
                GuidValue.Data4[6],
                GuidValue.Data4[7]
                );
  }

  Status = WriteBinaryFile(
             ImageHandle,
             EBBR_PROFILE_TABLE_DUMP_FILENAME,
             Text,
             (UINTN)(Cursor - Text)
             );
  gBS->FreePool(Text);

  return Status;
}

STATIC
EFI_STATUS
WriteZeroedProfileTable(
  IN EFI_HANDLE ImageHandle
  )
{
  UINT16  EmptyProfileTable[2];

  EmptyProfileTable[0] = 0;
  EmptyProfileTable[1] = 0;

  return WriteHexDumpFile(
           ImageHandle,
           EmptyProfileTable,
           sizeof(EmptyProfileTable)
           );
}

/*++

Routine Description:

  Retrieves the EBBR conformance profiles table and writes it to disk.

Arguments:

  ImageHandle - The loaded image handle.

Returns:

  EFI_SUCCESS           - The table was retrieved and written successfully.
  EFI_NOT_FOUND         - The conformance profiles table was not found.
  Other EFI_STATUS      - Returned from the file write operation.

--*/
STATIC
EFI_STATUS
DumpEbbrProfileTable(
  IN EFI_HANDLE ImageHandle
  )
{
  EFI_STATUS                      Status;
  EFI_STATUS                      TableStatus;
  EFI_CONFORMANCE_PROFILES_TABLE *ConfProfTable;
  UINTN                           TableSize;

  ConfProfTable = NULL;

  Status = GetSystemConfigurationTable(&gEfiConfProfilesTableGuid, (VOID **)&ConfProfTable);
  if (EFI_ERROR(Status) || (ConfProfTable == NULL)) {
    TableStatus = EFI_ERROR(Status) ? Status : EFI_NOT_FOUND;
    Print(L"EBBR Profile Table Dump: EFI Conformance Profiles Table not found (%r)\n", TableStatus);

    Status = WriteZeroedProfileTable(ImageHandle);
    if (EFI_ERROR(Status)) {
      Print(L"EBBR Profile Table Dump: Failed to write zeroed %s (%r)\n", EBBR_PROFILE_TABLE_DUMP_FILENAME, Status);
      return Status;
    }

    return TableStatus;
  }

  if (ConfProfTable->NumberOfProfiles > MAX_CONFORMANCE_PROFILE_COUNT) {
    Print(L"EBBR Profile Table Dump: Conformance Profiles Table count %u exceeds safe limit %u\n",
      (UINT32)ConfProfTable->NumberOfProfiles,
      (UINT32)MAX_CONFORMANCE_PROFILE_COUNT);
    return EFI_COMPROMISED_DATA;
  }

  if (ConfProfTable->NumberOfProfiles > ((MAX_UINTN - sizeof(*ConfProfTable)) / sizeof(EFI_GUID))) {
    Print(L"EBBR Profile Table Dump: Conformance Profiles Table size overflow for count %u\n",
      (UINT32)ConfProfTable->NumberOfProfiles);
    return EFI_COMPROMISED_DATA;
  }

  TableSize = sizeof(*ConfProfTable) +
              ConfProfTable->NumberOfProfiles * sizeof(EFI_GUID);

  Status = WriteHexDumpFile(
            ImageHandle,
            ConfProfTable,
            TableSize
            );
  if (EFI_ERROR(Status)) {
    Print(L"EBBR Profile Table Dump: Failed to write %s (%r)\n", EBBR_PROFILE_TABLE_DUMP_FILENAME, Status);
  }

  return Status;
}

/*++

Routine Description:

  UEFI application entry point for the EBBR profile table dump utility.

Arguments:

  ImageHandle - The loaded image handle.
  SystemTable - Pointer to the EFI system table.

Returns:

  EFI_SUCCESS           - The dump completed successfully.
  Other EFI_STATUS      - The status from DumpEbbProfileTable.

--*/
EFI_STATUS
EFIAPI
UefiDump(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                       Status;

  Status = DumpEbbrProfileTable(ImageHandle);
  if (EFI_ERROR(Status)) {
    Print(L"EBBR Profile Table Dump: Failed to dump EBBR Profile table (%r)\n", Status);
  }

  return Status;
}

/** @file

  Copyright (c) 2024, Arm Ltd, All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
/*++

Module Name:
  PlatformResetAttackMitigationPsciBBTestMain.c

Abstract:
  The main source file for Platform Reset Attack Mitigation via PSCI Black-Box Test.

--*/

//
// Includes
//

#include "SctLib.h"
#include "PlatformResetAttackMitigationPsciBBTestMain.h"

//
// Declarations
//

EFI_BB_TEST_PROTOCOL_FIELD gBBTestProtocolField = {
  PLAT_RESET_ATTACK_MITIGATION_PSCI_BB_TEST_REVISION,
  PLAT_RESET_ATTACK_MITIGATION_PSCI_BB_TEST_GUID,
  L"Platform Reset Attack Mitigation PSCI Test",
  L"Platform Reset Attack Mitigation PSCI Black-Box Test"
};

EFI_GUID gSupportProtocolGuid[] = {
  EFI_STANDARD_TEST_LIBRARY_GUID,
  EFI_TEST_RECOVERY_LIBRARY_GUID,
  EFI_NULL_GUID
};

EFI_BB_TEST_ENTRY_FIELD gBBTestEntryField[] = {
  {
    PLAT_RESET_ATTACK_MITIGATION_TEST_GUID,
    L"PlatformResetAttackMitigationPsciTest_func",
    L"Test whether Platform Reset Attack Mitigation implemented with PSCI",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    PlatformResetAttackMitigationPsciTest
  },
    0
};

EFI_BB_TEST_PROTOCOL *gBBTestProtocolInterface;

//
// Prototypes
//

EFI_STATUS
EFIAPI
InitializePlatformResetAttackMitigationPsciBBTest (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

EFI_STATUS
EFIAPI
UnloadPlatformResetAttackMitigationPsciBBTest (
  IN EFI_HANDLE       ImageHandle
  );

//
// Functions
//


/**
 *  Creates/installs the BlackBox Interface and the Entry Point list.
 *  @param ImageHandle    The image handle.
 *  @param SystemTable    The system table.
 *  @return EFI_SUCCESS   The interface was installed successfully.
 *  @return EFI_OUT_OF_RESOURCES    Failed due to the lack of resources.
 *  @return EFI_INVALID_PARAMETER   One of parameters is invalid.
 */
EFI_STATUS
EFIAPI
InitializePlatformResetAttackMitigationPsciBBTest (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  SctInitializeLib (ImageHandle, SystemTable);
  EfiInitializeTestLib (ImageHandle, SystemTable);

  return EfiInitAndInstallBBTestInterface (
           &ImageHandle,
           &gBBTestProtocolField,
           gBBTestEntryField,
           UnloadPlatformResetAttackMitigationPsciBBTest,
           &gBBTestProtocolInterface
           );
}


/**
 *  Unloads the BlackBox Interface and the Entry Point list.
 *  @param ImageHandle    The image handle.
 *  @return EFI_SUCCESS   The interface was uninstalled successfully.
 */
EFI_STATUS
EFIAPI
UnloadPlatformResetAttackMitigationPsciBBTest (
  IN EFI_HANDLE       ImageHandle
  )
{
  return EfiUninstallAndFreeBBTestInterface (
           ImageHandle,
           gBBTestProtocolInterface
           );
}


/**
 *  Support function - Get test support protocol library.
 *  @param SupportHandle  A handle containing support protocols.
 *  @param StandardLib    A pointer to Standard Test Support Protocol.
 *  @param RecoveryLib    A pointer to Test Recovery Support Protocol.
 *  @param LoggingLib     A pointer to Test Logging Support Protocol.
 *  @return EFI_SUCCESS   Successfully.
 *  @return Other value   Something failed.
 */
EFI_STATUS
GetTestSupportLibrary (
  IN EFI_HANDLE                           SupportHandle,
  OUT EFI_STANDARD_TEST_LIBRARY_PROTOCOL  **StandardLib,
  OUT EFI_TEST_RECOVERY_LIBRARY_PROTOCOL  **RecoveryLib,
  OUT EFI_TEST_LOGGING_LIBRARY_PROTOCOL   **LoggingLib
  )
{
  EFI_STATUS            Status;

  //
  // Get the standard test support library interface
  //
  *StandardLib = NULL;
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   (VOID **) StandardLib
                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get the test recovery support library interface
  //
  *RecoveryLib = NULL;
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiTestRecoveryLibraryGuid,
                   (VOID **) RecoveryLib
                   );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Get the test logging support library interface
  //
  *LoggingLib = NULL;
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiTestLoggingLibraryGuid,
                   (VOID **) LoggingLib
                   );
  // Test logging support library is optional

  //
  // Done
  //
  return EFI_SUCCESS;
}

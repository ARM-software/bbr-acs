/** @file

  Copyright (c) 2026, Arm Ltd. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
/*++

Module Name:
  SecureBootAuthVariablePersistenceBBTestMain.c

Abstract:
  Main source file for Secure Boot variable persistence black-box test.

--*/

#include "SctLib.h"
#include "SecureBootAuthVariablePersistenceBBTestMain.h"
#include "../../SecureBoot/BlackBoxTest/SecureBootBBTestSupport.h"

EFI_BB_TEST_PROTOCOL_FIELD gBBTestProtocolField = {
  SECURE_BOOT_AUTH_VARIABLE_PERSISTENCE_BB_TEST_REVISION,
  SECURE_BOOT_AUTH_VARIABLE_PERSISTENCE_BB_TEST_GUID,
  L"Secure Boot Variable Persistence Test",
  L"Secure Boot Variable Persistence Black-Box Test"
};

EFI_GUID gSupportProtocolGuid[] = {
  EFI_STANDARD_TEST_LIBRARY_GUID,
  EFI_TEST_RECOVERY_LIBRARY_GUID,
  EFI_TEST_PROFILE_LIBRARY_GUID,
  EFI_NULL_GUID
};

EFI_BB_TEST_ENTRY_FIELD gBBTestEntryField[] = {
  {
    SECURE_BOOT_AUTH_VARIABLE_PERSISTENCE_TEST_GUID,
    L"SecureBootAuthVariablePersistenceTest_func",
    L"Verify KEK authenticated-update anti-replay state persists across reset.",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid,
    EFI_TEST_CASE_AUTO,
    SecureBootAuthVariablePersistenceTest
  },
  EFI_NULL_GUID
};

EFI_BB_TEST_PROTOCOL *gBBTestProtocolInterface;

EFI_STATUS
EFIAPI
InitializeSecureBootAuthVariablePersistenceBbTest (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

EFI_STATUS
EFIAPI
UnloadSecureBootAuthVariablePersistenceBbTest (
  IN EFI_HANDLE  ImageHandle
  );

EFI_STATUS
EFIAPI
InitializeSecureBootAuthVariablePersistenceBbTest (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  SctInitializeLib (ImageHandle, SystemTable);
  EfiInitializeTestLib (ImageHandle, SystemTable);
  GetSystemDevicePathAndFilePath (ImageHandle);

  return EfiInitAndInstallBBTestInterface (
           &ImageHandle,
           &gBBTestProtocolField,
           gBBTestEntryField,
           UnloadSecureBootAuthVariablePersistenceBbTest,
           &gBBTestProtocolInterface
           );
}

EFI_STATUS
EFIAPI
UnloadSecureBootAuthVariablePersistenceBbTest (
  IN EFI_HANDLE  ImageHandle
  )
{
  return EfiUninstallAndFreeBBTestInterface (
           ImageHandle,
           gBBTestProtocolInterface
           );
}

EFI_STATUS
GetTestSupportLibrary (
  IN EFI_HANDLE                           SupportHandle,
  OUT EFI_STANDARD_TEST_LIBRARY_PROTOCOL  **StandardLib,
  OUT EFI_TEST_RECOVERY_LIBRARY_PROTOCOL  **RecoveryLib,
  OUT EFI_TEST_PROFILE_LIBRARY_PROTOCOL   **ProfileLib,
  OUT EFI_TEST_LOGGING_LIBRARY_PROTOCOL   **LoggingLib
  )
{
  EFI_STATUS  Status;

  *StandardLib = NULL;
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiStandardTestLibraryGuid,
                   (VOID **) StandardLib
                   );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *RecoveryLib = NULL;
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiTestRecoveryLibraryGuid,
                   (VOID **) RecoveryLib
                   );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *ProfileLib = NULL;
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiTestProfileLibraryGuid,
                   (VOID **) ProfileLib
                   );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *LoggingLib = NULL;
  Status = gtBS->HandleProtocol (
                   SupportHandle,
                   &gEfiTestLoggingLibraryGuid,
                   (VOID **) LoggingLib
                   );

  // The logging support protocol is optional.
  return EFI_SUCCESS;
}

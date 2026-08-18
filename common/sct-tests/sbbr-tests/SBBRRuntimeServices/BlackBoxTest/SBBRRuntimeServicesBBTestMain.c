/** @file

  Copyright 2006 - 2016 Unified EFI, Inc.<BR>
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

  SBBRRuntimeServicesBBTestMain.c

Abstract:

  Test driver for SBBR Runtime Services tests.

--*/

#include "SctLib.h"
#include <Library/EfiTestLib.h>
#include "SBBRRuntimeServicesBBTestMain.h"

EFI_STATUS
BBTestResetShutdown (
  IN EFI_BB_TEST_PROTOCOL  *This,
  IN VOID                  *ClientInterface,
  IN EFI_TEST_LEVEL        TestLevel,
  IN EFI_HANDLE            SupportHandle
  );

EFI_TPL TplArray[TPL_ARRAY_SIZE] = {
  TPL_APPLICATION,
  TPL_CALLBACK,
  TPL_NOTIFY
};

EFI_BB_TEST_PROTOCOL_FIELD gBBTestProtocolField = {
  SBBRRUNTIMESERVICES_TEST_REVISION,
  SBBRRUNTIMESERVICES_TEST_GUID,
  L"SBBRRuntimeServices Tests",
  L"ResetSystem shutdown test."
};

EFI_GUID gSupportProtocolGuid[] = {
  EFI_STANDARD_TEST_LIBRARY_GUID,
  EFI_TEST_RECOVERY_LIBRARY_GUID,
  EFI_NULL_GUID
};

EFI_BB_TEST_ENTRY_FIELD gBBTestEntryField[] = {
  {
    SBBRRUNTIMESERVICES_TEST_CASE_RESETSHUTDOWN_GUID,
    L"ResetSystem Shutdown Test",
    L"Manual test for ResetSystem shutdown.",
    EFI_TEST_LEVEL_DEFAULT,
    gSupportProtocolGuid,
    EFI_TEST_CASE_MANUAL,
    BBTestResetShutdown
  },
  EFI_NULL_GUID
};

EFI_BB_TEST_PROTOCOL *gBBTestProtocolInterface;

/**
  @brief Uninstalls the SBBR Runtime Services black-box test interface.

  @param[in] ImageHandle  The test driver image handle.

  @retval EFI_SUCCESS  The interface was uninstalled.
**/
EFI_STATUS
BBTestSBBRRuntimeServicesUnload (
  IN EFI_HANDLE ImageHandle
  )
{
  return EfiUninstallAndFreeBBTestInterface (
           ImageHandle,
           gBBTestProtocolInterface
           );
}

/**
  @brief Initializes and installs the SBBR Runtime Services test interface.

  @param[in] ImageHandle   The test driver image handle.
  @param[in] SystemTable   The EFI system table.

  @retval EFI_SUCCESS           The interface was installed.
  @retval EFI_OUT_OF_RESOURCES  Interface allocation failed.
  @retval EFI_INVALID_PARAMETER  An input parameter was invalid.
**/
EFI_STATUS
InitializeBBTestSBBRRuntimeServices (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EfiInitializeTestLib (ImageHandle, SystemTable);
  SctInitializeLib (ImageHandle, SystemTable);
  return EfiInitAndInstallBBTestInterface (
           &ImageHandle,
           &gBBTestProtocolField,
           gBBTestEntryField,
           BBTestSBBRRuntimeServicesUnload,
           &gBBTestProtocolInterface
           );
}

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

  SBBRRuntimeServicesBBTestFunction.c

Abstract:

  Test case definitions for SBBR Runtime Services tests.

--*/

#include "Guid.h"
#include "SctLib.h"
#include <Library/BaseMemoryLib.h>
#include <Library/EfiTestLib.h>
#include "SBBRRuntimeServicesBBTestMain.h"
#include EFI_TEST_PROTOCOL_DEFINITION(TestRecoveryLibrary)

extern EFI_TPL TplArray[];


/**
  @brief Tests ResetSystem with EfiResetShutdown at supported task priority levels.

  SBBR 3.5.4.

  @param[in] This             The black-box test protocol instance.
  @param[in] ClientInterface  The interface under test.
  @param[in] TestLevel        The requested test thoroughness.
  @param[in] SupportHandle    The handle containing support protocols.

  @retval EFI_SUCCESS     The test completed.
  @retval Other           A required test support protocol was unavailable.
**/
EFI_STATUS
BBTestResetShutdown (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib;
  EFI_TEST_RECOVERY_LIBRARY_PROTOCOL   *RecoveryLib;
  EFI_STATUS                           Status;
  EFI_TEST_ASSERTION                   AssertionType;
  UINTN                                Index;
  EFI_TPL                              OldTpl;
  UINT8                                Buffer[1024];
  UINT8                                EmptyRecoveryData = 0;
  RESET_DATA                           *ResetData;
  UINTN                                Size;

  //
  // Get the Standard Library Interface
  //
  Status = gtBS->HandleProtocol (
              SupportHandle,
              &gEfiStandardTestLibraryGuid,
              (VOID **) &StandardLib
              );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get the Recovery Library Interface
  //
  Status = gtBS->HandleProtocol (
              SupportHandle,
              &gEfiTestRecoveryLibraryGuid,
              (VOID **) &RecoveryLib
              );

  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                StandardLib,
                EFI_TEST_ASSERTION_FAILED,
                gTestGenericFailureGuid,
                L"BS.HandleProtocol - Handle recovery library",
                L"%a:%d:Status - %r",
                __FILE__,
                (UINTN)__LINE__,
                Status
                );
    return Status;
  }

  //
  // Read reset record
  //
  Status = RecoveryLib->ReadResetRecord (
              RecoveryLib,
              &Size,
              Buffer
              );
  ResetData = (RESET_DATA *)Buffer;
  if (EFI_ERROR (Status) || (Size < sizeof (RESET_DATA))) {
    ZeroMem (Buffer, sizeof (Buffer));
    ResetData = (RESET_DATA *)Buffer;
  } else if (ResetData->Step == 1) {
    //
    // Step 2
    //
    if (ResetData->TplIndex < TPL_ARRAY_SIZE) {
      Index = ResetData->TplIndex;
      AssertionType = EFI_TEST_ASSERTION_PASSED;
      goto ManualTestStep2;
    }
  } else {
    return EFI_LOAD_ERROR;
  }

  for (Index = 0; Index < TPL_ARRAY_SIZE; Index++) {
    //
    // 4.2.2.1  ResetSystem must succeed when ResetType is EfiResetShutdown
    //
    ResetData->Step = 1;
    ResetData->TplIndex = Index;
    Status = RecoveryLib->WriteResetRecord (
                RecoveryLib,
                sizeof (RESET_DATA),
                Buffer
                );
    if (EFI_ERROR (Status)) {
      StandardLib->RecordAssertion (
                  StandardLib,
                  EFI_TEST_ASSERTION_FAILED,
                  gTestGenericFailureGuid,
                  L"TestRecoveryLib - WriteResetRecord",
                  L"%a:%d:Status - %r, TPL - %d",
                  __FILE__,
                  (UINTN)__LINE__,
                  Status,
                  TplArray[Index]
                  );
      return Status;
    }

    //
    // Print out some information to avoid the user thought it is an error.
    //
    // And the stall a second is required to make sure the recovery data has
    // been written into the storage device.
    //
    SctPrint (L"System will shut down after 1 second...");
    gtBS->Stall (1000000);

    OldTpl = gtBS->RaiseTPL (TplArray[Index]);
    gtRT->ResetSystem (
                EfiResetShutdown,
                EFI_SUCCESS,
                0,
                NULL
                );
    gtBS->RestoreTPL (OldTpl);
    AssertionType = EFI_TEST_ASSERTION_FAILED;

ManualTestStep2:
    if (AssertionType == EFI_TEST_ASSERTION_FAILED) {
      StandardLib->RecordAssertion (
                    StandardLib,
                    AssertionType,
                    (Index == 1) ?
                      gSBBRRuntimeServicesAssertion002Guid :
                      gSBBRRuntimeServicesAssertion001Guid,
                    L"RT.ResetSystem - EfiResetShutdown",
                    L"%a:%d: ResetSystem returned unexpectedly, TPL - %d",
                    __FILE__,
                    (UINTN)__LINE__,
                    TplArray[Index]
                    );
    } else {
      StandardLib->RecordAssertion (
                    StandardLib,
                    AssertionType,
                    (Index == 1) ?
                      gSBBRRuntimeServicesAssertion002Guid :
                      gSBBRRuntimeServicesAssertion001Guid,
                    L"RT.ResetSystem - EfiResetShutdown",
                    L"%a:%d:Status - %r, TPL - %d",
                    __FILE__,
                    (UINTN)__LINE__,
                    Status,
                    TplArray[Index]
                    );
    }
  }

  Status = RecoveryLib->WriteResetRecord (
                         RecoveryLib,
                         0,
                         &EmptyRecoveryData
                         );
  if (EFI_ERROR (Status)) {
    StandardLib->RecordAssertion (
                  StandardLib,
                  EFI_TEST_ASSERTION_FAILED,
                  gTestGenericFailureGuid,
                  L"TestRecoveryLib - Clear reset record",
                  L"%a:%d:Status - %r",
                  __FILE__,
                  (UINTN)__LINE__,
                  Status
                  );
    return Status;
  }

  return EFI_SUCCESS;
}

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
 PlatformResetAttackMitigationPsciBBTestFunction.c

Abstract:
  Source file for Platform Reset Attack Mitigation PSCI Black-Box Test.
--*/

#include "SctLib.h"
#include "PlatformResetAttackMitigationPsciBBTestMain.h"
#include <Library/ArmSmcLib.h>

ARM_SMC_ARGS  SmcArgs;

//
// Prototypes
//

EFI_STATUS
PlatformResetAttackMitigationPsciTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// Prototypes (internal)
//

EFI_STATUS
PlatformResetAttackMitigationPsciTestSub1 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib
  );

 /* BBSR R310_BBSR - Entry point for Platform Reset Attack Mitigation via PSCI Function Test.
 *  @param This             A pointer to the EFI_BB_TEST_PROTOCOL instance.
 *  @param ClientInterface  A pointer to the interface to be tested.
 *  @param TestLevel        Test "thoroughness" control.
 *  @param SupportHandle    A handle containing support protocols.
 *  @return EFI_SUCCESS     Successfully.
 *  @return Other value     Something failed.
 */
EFI_STATUS
PlatformResetAttackMitigationPsciTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STATUS                          Status;
  EFI_RUNTIME_SERVICES                *RT;
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL  *StandardLib;
  EFI_TEST_RECOVERY_LIBRARY_PROTOCOL  *RecoveryLib;
  EFI_TEST_LOGGING_LIBRARY_PROTOCOL   *LoggingLib;

  //
  // Get test support library interfaces
  //
  Status = GetTestSupportLibrary (
             SupportHandle,
             &StandardLib,
             &RecoveryLib,
             &LoggingLib
             );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (FALSE == CheckBBTestCanRunAndRecordAssertion(
                  StandardLib,
                  L"Platform Reset Attack Mitigation via PSCI test not supported in EFI",
                  __FILE__,
                  (UINTN)__LINE__
                  )) {
    return EFI_SUCCESS;
  }

  RT = (EFI_RUNTIME_SERVICES *)ClientInterface;

  Status = PlatformResetAttackMitigationPsciTestSub1 (RT, StandardLib, LoggingLib);

  return EFI_SUCCESS;
}

//
// Platform Reset Attack Mitigation Psci Test
//
EFI_STATUS
PlatformResetAttackMitigationPsciTestSub1 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib
  )
{
  EFI_STATUS            Status;
  UINT32                Index;
  UINT32                Attribute;
  UINTN                 DataSize;
  VOID                  *Data;
  EFI_TEST_ASSERTION    Result = EFI_TEST_ASSERTION_PASSED;

  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->EnterFunction (
                  LoggingLib,
                  L"PlatformResetAttackMitigationPsciTestSub1",
                  L"BBSR R310"
                  );
  }

  // PSCI MEM_PROTECT check
  SmcArgs.Arg0 = ARM_SMC_ID_PSCI_PSCI_FEATURES;
  SmcArgs.Arg1 = ARM_SMC_ID_PSCI_MEM_PROTECT;

  ArmCallSmc(&SmcArgs);

  StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"\r\nPSCI feature return value = %x",
                     SmcArgs.Arg0
                     );

  // If MEM_PROTECT function is implemented, PSCI_FEATURES returns a value of 0.
  if (SmcArgs.Arg0 == 0)
      Result = EFI_TEST_ASSERTION_PASSED;
  else
      Result = EFI_TEST_ASSERTION_FAILED;

  StandardLib->RecordAssertion (
                   StandardLib,
                   Result,
                   gPlatformResetAttackMitigationPsciBbTestAssertionGuid001,
                   L"Platfrom Reset Attack prevention support via PSCI",
                   L"%a:%d:",
                   __FILE__,
                   (UINTN)__LINE__
                   );

  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->ExitFunction (
                  LoggingLib,
                  L"PlatformResetAttackMitigationPsciTestSub1",
                  L"BBSR R310"
                  );
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}


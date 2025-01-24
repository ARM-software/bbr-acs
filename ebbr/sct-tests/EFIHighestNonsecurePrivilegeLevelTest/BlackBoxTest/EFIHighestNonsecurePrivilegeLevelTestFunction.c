/** @file

  Copyright 2006-2012 Unified EFI, Inc.<BR>
  Copyright (c) 2021-2024, Arm Ltd, All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php
 
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 
**/
/*++

Module Name:
 EFIHighestNonsecurePrivilegeLevelTestFunction.c

Abstract:
  Source file for EFI Highest Nonsecure Privilege Level Black-Box Test - Function Test.

--*/

#include "SctLib.h"
#include "EFIHighestNonsecurePrivilegeLevelTestMain.h"
#include "AArch64/PeSysReg.h"

//
// Prototypes (external)
//

EFI_STATUS
EFIHighestNonsecurePrivilegeLevelTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// Prototypes (internal)
//

EFI_STATUS
EFIHighestNonsecurePrivilegeLevelTestSub1 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib
  );

 /* EFIHighestNonsecurePrivilegeLevelTest - Entry point for EFIHighestNonsecurePrivilegeLevel Function Test.
 *  @param This             A pointer to the EFI_BB_TEST_PROTOCOL instance.
 *  @param ClientInterface  A pointer to the interface to be tested.
 *  @param TestLevel        Test "thoroughness" control.
 *  @param SupportHandle    A handle containing support protocols.
 *  @return EFI_SUCCESS     Successfully.
 *  @return Other value     Something failed.
 */
EFI_STATUS
EFIHighestNonsecurePrivilegeLevelTest (
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
                  L"EFIHighestNonsecurePrivilegeLevelTest_func - Failed to test highest non secure privilege level",
                  __FILE__,
                  (UINTN)__LINE__
                  )) {
    return EFI_SUCCESS;
  }

  RT = (EFI_RUNTIME_SERVICES *)ClientInterface;

  Status = EFIHighestNonsecurePrivilegeLevelTestSub1 (RT, StandardLib, LoggingLib);

  return EFI_SUCCESS;
}

 /* EFIHighestNonsecurePrivilegeLevelTestSub1 - Entry point for EFIHighestNonsecurePrivilegeLevel Sub Function Test.
 *  @param RT             A pointer to the Runtime services.
 *  @param StandardLib    A pointer to the Standard library.
 *  @param LoggingLib     A pointer to the Logging library.
 *  @return EFI_SUCCESS     Successfully.
 *  @return Other value     Something failed.
 */
EFI_STATUS
EFIHighestNonsecurePrivilegeLevelTestSub1 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib
  )
{
  EFI_TEST_ASSERTION  AssertionType;
  UINT16 *PrivilegeLevelStatus = NULL;
  UINT64 PfrRegData = 0;
  UINT64 CurrentElRegData = 0;

  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->EnterFunction (
                  LoggingLib,
                  L"EFIHighestNonsecurePrivilegeLevelTestSub1",
                  L"UEFI R/2.3"
                  );
  }

  CurrentElRegData = AA64ReadCurrentEl();
  CurrentElRegData = VAL_EXTRACT_BITS(CurrentElRegData, 2, 3); 
  SctPrint(L"CureentEl : 0x%llx\n", CurrentElRegData);

  PfrRegData = AA64ReadPfr0El1();
  SctPrint(L"AA64PFR0_EL1 : 0x%llx\n", PfrRegData);

  /* bits 1:0, 5:4 and 9:8 must not be zero */
  if ((CurrentElRegData == 2) && (PfrRegData & 0x300)) {
    PrivilegeLevelStatus = L"EL2";
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  }
  else if ((CurrentElRegData == 1) && (PfrRegData & 0x30) && !(PfrRegData & 0x300)) {
    PrivilegeLevelStatus = L"EL1";
    AssertionType = EFI_TEST_ASSERTION_PASSED;
  }
  else {
    PrivilegeLevelStatus = L"None, failed to test";
    AssertionType = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
              StandardLib,
              EFI_TEST_ASSERTION_PASSED,
              gHighestNonsecurePrivilegeLevelTestAssertionGuid001,
              L"Highest nonsecure privilege level test",
              L"%a:%d: Highest nonsecure privilege level-%s",
              __FILE__,
              (UINTN)__LINE__,
              PrivilegeLevelStatus
              );

  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->ExitFunction (
                  LoggingLib,
                  L"EFIHighestNonsecurePrivilegeLevelTestSub1",
                  L"UEFI R/2.3"
                  );
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}

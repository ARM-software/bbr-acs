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
 BBSRVariableSizeBbTestFunction.c

Abstract:
  Source file for BBSR Variable Size Function Black-Box Test - Function Test.

--*/

#include "SctLib.h"
#include "BBSRVariableSizeBBTestMain.h"

//
// Prototypes (external)
//

EFI_STATUS
BBSRVariableSizeTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// Prototypes (internal)
//

EFI_STATUS
BBSRVariableSizeTestSub1 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib
  );

 /*  BBSR R040/050 - Entry point for BBSRVariableSize Function Test.
 *  @param This             A pointer to the EFI_BB_TEST_PROTOCOL instance.
 *  @param ClientInterface  A pointer to the interface to be tested.
 *  @param TestLevel        Test "thoroughness" control.
 *  @param SupportHandle    A handle containing support protocols.
 *  @return EFI_SUCCESS     Successfully.
 *  @return Other value     Something failed.
 */
EFI_STATUS
BBSRVariableSizeTest (
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
                  L"BBSR VariableSize_Func - BBSR Variable Size Test not supporte in EFI",
                  __FILE__,
                  (UINTN)__LINE__
                  )) {
    return EFI_SUCCESS;
  }

  RT = (EFI_RUNTIME_SERVICES *)ClientInterface;

  Status = BBSRVariableSizeTestSub1 (RT, StandardLib, LoggingLib);

  return EFI_SUCCESS;
}

//
// BBSR Variable Size Test
//
EFI_STATUS
BBSRVariableSizeTestSub1 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib
  )
{
  EFI_STATUS            QueryVarStatus;
  UINT32                ValidAttributes = EFI_VARIABLE_NON_VOLATILE|EFI_VARIABLE_BOOTSERVICE_ACCESS|EFI_VARIABLE_RUNTIME_ACCESS;
  UINT64                MaximumVariableStorageSize;
  UINT64                RemainingVariableStorageSize;
  UINT64                MaximumVariableSize;
  EFI_TEST_ASSERTION Result = EFI_TEST_ASSERTION_PASSED;
  
  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->EnterFunction (
                  LoggingLib,
                  L"VariableSizeFuncTestSub1",
                  L"BBSR R040/R050"
                  );
  }
    QueryVarStatus = RT->QueryVariableInfo (
                              ValidAttributes,
                              &MaximumVariableStorageSize,
                              &RemainingVariableStorageSize,
                              &MaximumVariableSize
                              );

     StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"\r\nQueryVariable MaxStorageSize is %d MaxVariableSize is %d",
                     MaximumVariableStorageSize,
                     MaximumVariableSize
                     );

   //Check MaxVariableStorageSize is larger than 128kb as per R040
    if ( MaximumVariableStorageSize < 131072) {
                     StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"\r\nQueryVariable MaxStorageSize is %d, but must be at least 128KB",
                     MaximumVariableStorageSize
                     );

          Result = EFI_TEST_ASSERTION_FAILED;
    }
    //
    // Record assertion
    //
    StandardLib->RecordAssertion (
                   StandardLib,
                   Result,
                   gBBSRVariableSizeTestBbTestVarSizeAssertionGuid001,
                   L"RT.SecurityVariableSizeTest - BBSR Variable Size test",
                   L"%a:%d:",
                   __FILE__,
                   (UINTN)__LINE__
                   );

   // Reset Result variable
   Result = EFI_TEST_ASSERTION_PASSED;

   //Check MaxVariableSize is larger than 64kb as per R050

   //In some implementations a 64KB max variable size results in a usable size slightly less
   //hence, relaxing the test to check for 60000 bytes.
    if ( MaximumVariableSize < 60000) {
                     StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"\r\nQueryVariable MaxVariableSize is %d, but must be at least 64KB",
                     MaximumVariableSize
                     );

          Result = EFI_TEST_ASSERTION_FAILED;
   }
   //
    // Record assertion
    //
    StandardLib->RecordAssertion (
                   StandardLib,
                   Result,
                   gBBSRVariableSizeTestBbTestVarSizeAssertionGuid002,
                   L"RT.SecurityVariableSizeTest - BBSR Variable Size test",
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
                  L"BBSRVariableSizeFuncTestSub1",
                  L"BBSR R040/050"
                  );
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}


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
 EbbrProfileTableTestFunction.c

Abstract:
  Source file for EFI EBBR Profile Table Black-Box Test - Function Test.

--*/

#include "SctLib.h"
#include "EbbrProfileTableTestMain.h"

//
// Prototypes (external)
//

EFI_STATUS
EbbrProfileTableTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// Prototypes (internal)
//

EFI_STATUS
EbbrProfileTableTestSub1 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib
  );

 /* EbbrProfileTableTest - Entry point for EbbrProfileTable Function Test.
 *  @param This             A pointer to the EFI_BB_TEST_PROTOCOL instance.
 *  @param ClientInterface  A pointer to the interface to be tested.
 *  @param TestLevel        Test "thoroughness" control.
 *  @param SupportHandle    A handle containing support protocols.
 *  @return EFI_SUCCESS     Successfully.
 *  @return Other value     Something failed.
 */
EFI_STATUS
EbbrProfileTableTest (
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
                  L"EBBR EbbrProfileTableTest_func - Failed to test EBBR profile table",
                  __FILE__,
                  (UINTN)__LINE__
                  )) {
    return EFI_SUCCESS;
  }

  RT = (EFI_RUNTIME_SERVICES *)ClientInterface;

  Status = EbbrProfileTableTestSub1 (RT, StandardLib, LoggingLib);

  return EFI_SUCCESS;
}

 /* EbbrProfileTableTestSub1 - Entry point for EbbrProfileTable Sub Function Test.
 *  @param RT             A pointer to the Runtime services.
 *  @param StandardLib    A pointer to the Standard library.
 *  @param LoggingLib     A pointer to the Logging library.
 *  @return EFI_SUCCESS     Successfully.
 *  @return Other value     Something failed.
 */
EFI_STATUS
EbbrProfileTableTestSub1 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_GUID *EntryGuid = NULL;
  EFI_CONFORMANCE_PROFILES_TABLE *ConfProfTable = NULL;
  CHAR16 *EbbrProfileName = NULL;
  UINTN Profile = 0;
  UINTN EbbrTableCheck = 0;
  UINTN TableWalker = 0;
  UINT64 ConformanceProfileTableAddress = 0;

  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->EnterFunction (
                  LoggingLib,
                  L"EbbrProfileTableTestSub1",
                  L"EBBR R/2.4.2"
                  );
  }

  Status = SctGetSystemConfigurationTable (&gEfiConfProfilesTableGuid, (VOID **)&ConfProfTable);
  if( Status == EFI_SUCCESS ) {
    EbbrProfileName = L"Unknown";
    EntryGuid = (EFI_GUID *)(ConfProfTable + 1);
    for (Profile = 0; Profile < ConfProfTable->NumberOfProfiles; Profile++, EntryGuid++) {
      if (CompareGuid (EntryGuid, &gEfiConfProfilesEbbrSpec21Guid)) {
        EbbrProfileName = L"EBBR_2.1";
        EbbrTableCheck = 1;
        break;
      }
      else if (CompareGuid (EntryGuid, &gEfiConfProfilesEbbrSpec22Guid)) {
        EbbrProfileName = L"EBBR_2.2";
        EbbrTableCheck = 1;
        break;
      }
    }

    if(EbbrTableCheck == 0) {
        StandardLib->RecordAssertion (
           StandardLib,
           EFI_TEST_ASSERTION_WARNING,
           gEbbrProfileTableTestAssertionGuid001,
           L"Ebbr Profile Table test",
           L"%s profile table, %a:%d: Status - %r",
           EbbrProfileName,
           __FILE__,
           (UINTN)__LINE__,
           Status
           );
    }
    else {
        StandardLib->RecordAssertion (
           StandardLib,
           EFI_TEST_ASSERTION_PASSED,
           gEbbrProfileTableTestAssertionGuid001,
           L"Ebbr Profile Table test",
           L"Found %s profile table, %a:%d: Status - %r",
           EbbrProfileName,
           __FILE__,
           __FILE__,
           (UINTN)__LINE__,
           Status
           );
    }

    StandardLib->RecordMessage (
                StandardLib,
                EFI_VERBOSE_LEVEL_DEFAULT,
                L"\r\nEbbr Profile Table Status - %r",
                Status
                );
  }
  else {
    StandardLib->RecordAssertion (
               StandardLib,
               EFI_TEST_ASSERTION_WARNING,
               gEfiConformanceProfileTableTestAssertionGuid002,
               L"Not Found EFI Conformance Profile Table",
               L"%a:%d: Status - %r",
               __FILE__,
               (UINTN)__LINE__,
	       Status
               );
  }

  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->ExitFunction (
                  LoggingLib,
                  L"EbbrProfileTableTestSub1",
                  L"EBBR R/2.4.2"
                  );
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}

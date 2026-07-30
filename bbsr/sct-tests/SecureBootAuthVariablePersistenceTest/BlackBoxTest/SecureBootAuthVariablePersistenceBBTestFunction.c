/** @file

  Copyright (c) 2026, Arm Ltd. All rights reserved.<BR>

  This program and the accompanying materials are licensed and made available
  under the terms and conditions of the BSD License which accompanies this
  distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "SctLib.h"
#include "SecureBootAuthVariablePersistenceBBTestMain.h"
#include "../../SecureBoot/BlackBoxTest/SecureBootBBTestSupport.h"

#define KEK_ATTRIBUTES  (EFI_VARIABLE_NON_VOLATILE | \
                         EFI_VARIABLE_BOOTSERVICE_ACCESS | \
                         EFI_VARIABLE_RUNTIME_ACCESS | \
                         EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS)

#define PERSISTENCE_MARKER_SIZE  2U

STATIC CONST UINT8  mPersistenceMarker[PERSISTENCE_MARKER_SIZE] = { 0x4B, 0x34 };

STATIC
EFI_STATUS
RecordPersistenceAssertion (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL  *StandardLib,
  IN EFI_TEST_ASSERTION                   Result,
  IN CHAR16                               *Detail,
  IN UINTN                                LineNumber
  )
{
  return StandardLib->RecordAssertion (
                        StandardLib,
                        Result,
                        gSecureBootAuthVariablePersistenceBbTestAssertionGuid001,
                        L"SecureBoot - authenticated KEK state persists across reset",
                        L"%a:%d: %s",
                        __FILE__,
                        LineNumber,
                        Detail
                        );
}

STATIC
EFI_STATUS
LoadDependencyFile (
  IN CHAR16  *FileName,
  OUT VOID   **Data,
  OUT UINTN  *DataSize
  )
{
  EFI_STATUS       Status;
  EFI_FILE_HANDLE  FileHandle;
  UINT32           FileSize;
  UINTN            BufferSize;
  VOID             *Buffer;

  *Data = NULL;
  *DataSize = 0;
  Status = OpenFileAndGetSize (FileName, &FileHandle, &FileSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Buffer = SctAllocatePool (FileSize);
  if (Buffer == NULL) {
    FileHandle->Close (FileHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  BufferSize = FileSize;
  Status = FileHandle->Read (FileHandle, &BufferSize, Buffer);
  FileHandle->Close (FileHandle);
  if (EFI_ERROR (Status)) {
    gtBS->FreePool (Buffer);
    return EFI_LOAD_ERROR;
  }

  *Data = Buffer;
  *DataSize = BufferSize;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
ReadKek (
  IN EFI_RUNTIME_SERVICES  *RT,
  OUT UINT32               *Attributes,
  OUT VOID                 **Data,
  OUT UINTN                *DataSize
  )
{
  EFI_STATUS  Status;
  VOID        *Buffer;

  *Attributes = 0;
  *Data = NULL;
  *DataSize = 0;
  Status = RT->GetVariable (
                 L"KEK",
                 &gEfiGlobalVariableGuid,
                 Attributes,
                 DataSize,
                 NULL
                 );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return Status;
  }

  Buffer = SctAllocatePool (*DataSize);
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = RT->GetVariable (
                 L"KEK",
                 &gEfiGlobalVariableGuid,
                 Attributes,
                 DataSize,
                 Buffer
                 );
  if (EFI_ERROR (Status)) {
    gtBS->FreePool (Buffer);
    return Status;
  }

  *Data = Buffer;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
ValidateTestKek4 (
  IN EFI_RUNTIME_SERVICES                *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL  *StandardLib,
  OUT BOOLEAN                            *Valid
  )
{
  EFI_STATUS  Status;
  UINT32      Attributes;
  VOID        *ExpectedData;
  UINTN       ExpectedSize;
  VOID        *KekData;
  UINTN       KekSize;

  *Valid = FALSE;
  ExpectedData = NULL;
  KekData = NULL;
  Status = LoadDependencyFile (L"TestKEK4.esl", &ExpectedData, &ExpectedSize);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_DEFAULT,
                   L"Could not load TestKEK4.esl: %r",
                   Status
                   );
    return Status;
  }

  Status = ReadKek (RT, &Attributes, &KekData, &KekSize);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_DEFAULT,
                   L"Could not read KEK: %r",
                   Status
                   );
    gtBS->FreePool (ExpectedData);
    return Status;
  }

  StandardLib->RecordMessage (
                 StandardLib,
                 EFI_VERBOSE_LEVEL_DEFAULT,
                 L"KEK present: attr=0x%08x size=%d",
                 Attributes,
                 KekSize
                 );
  *Valid = (Attributes == KEK_ATTRIBUTES) &&
           (KekSize == ExpectedSize) &&
           (SctCompareMem (KekData, ExpectedData, ExpectedSize) == 0);
  if (!*Valid) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_DEFAULT,
                   L"KEK does not exactly match TestKEK4."
                   );
  }

  gtBS->FreePool (KekData);
  gtBS->FreePool (ExpectedData);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
SetKekFromFile (
  IN EFI_RUNTIME_SERVICES  *RT,
  IN CHAR16                *FileName
  )
{
  EFI_STATUS  Status;
  VOID        *Data;
  UINTN       DataSize;

  Data = NULL;
  Status = LoadDependencyFile (FileName, &Data, &DataSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = RT->SetVariable (
                 L"KEK",
                 &gEfiGlobalVariableGuid,
                 KEK_ATTRIBUTES,
                 DataSize,
                 Data
                 );
  gtBS->FreePool (Data);
  return Status;
}

STATIC
EFI_STATUS
UpdateTestKek4AtT2 (
  IN EFI_RUNTIME_SERVICES                *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL  *StandardLib
  )
{
  EFI_STATUS  Status;

  Status = SetKekFromFile (RT, L"TestKEK4T2.auth");
  if (Status != EFI_SUCCESS) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_DEFAULT,
                   L"Authenticated TestKEK4 payload with newer T2 timestamp "
                   L"failed: %r",
                   Status
                   );
  }

  return Status;
}

STATIC
EFI_STATUS
ClearPersistenceRecord (
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL  *StandardLib,
  IN EFI_TEST_RECOVERY_LIBRARY_PROTOCOL  *RecoveryLib
  )
{
  EFI_STATUS  Status;
  UINT8       EmptyRecord;

  EmptyRecord = 0;
  Status = RecoveryLib->WriteResetRecord (RecoveryLib, 0, &EmptyRecord);
  if (EFI_ERROR (Status)) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_DEFAULT,
                   L"Could not clear the persistence recovery record: %r",
                   Status
                   );
  }

  return Status;
}

STATIC
EFI_STATUS
CleanupAndRecordFailure (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib,
  IN EFI_TEST_PROFILE_LIBRARY_PROTOCOL    *ProfileLib,
  IN EFI_TEST_RECOVERY_LIBRARY_PROTOCOL   *RecoveryLib,
  IN CHAR16                               *Detail,
  IN UINTN                                LineNumber
  )
{
  EFI_STATUS  TeardownStatus;
  EFI_STATUS  ClearStatus;

  TeardownStatus = SecureBootVariableCleanup (
                     RT,
                     StandardLib,
                     LoggingLib,
                     ProfileLib
                     );
  if (EFI_ERROR (TeardownStatus)) {
    StandardLib->RecordMessage (
                   StandardLib,
                   EFI_VERBOSE_LEVEL_DEFAULT,
                   L"Normal Secure Boot teardown failed after the persistence "
                   L"test: %r.",
                   TeardownStatus
                   );
  }

  ClearStatus = ClearPersistenceRecord (StandardLib, RecoveryLib);
  if (EFI_ERROR (TeardownStatus) || EFI_ERROR (ClearStatus)) {
    (VOID)RecordPersistenceAssertion (
            StandardLib,
            EFI_TEST_ASSERTION_FAILED,
            L"Secure Boot cleanup failed after the persistence test.",
            (UINTN)__LINE__
            );
  }

  return RecordPersistenceAssertion (
           StandardLib,
           EFI_TEST_ASSERTION_FAILED,
           Detail,
           LineNumber
           );
}

STATIC
EFI_STATUS
SecureBootAuthVariablePersistenceStart (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib,
  IN EFI_TEST_PROFILE_LIBRARY_PROTOCOL    *ProfileLib,
  IN EFI_TEST_RECOVERY_LIBRARY_PROTOCOL   *RecoveryLib
  )
{
  EFI_STATUS  Status;
  BOOLEAN     Valid;

  Status = SetKekFromFile (RT, L"TestKEK4T1.auth");
  if (Status != EFI_SUCCESS) {
    return CleanupAndRecordFailure (
             RT,
             StandardLib,
             LoggingLib,
             ProfileLib,
             RecoveryLib,
             L"Could not install TestKEK4 with the PK-signed T1 request.",
             (UINTN)__LINE__
             );
  }

  Status = ValidateTestKek4 (RT, StandardLib, &Valid);
  if (EFI_ERROR (Status) || !Valid) {
    return CleanupAndRecordFailure (
             RT,
             StandardLib,
             LoggingLib,
             ProfileLib,
             RecoveryLib,
             L"KEK does not exactly match TestKEK4 after the T1 install.",
             (UINTN)__LINE__
             );
  }

  Status = RecoveryLib->WriteResetRecord (
                          RecoveryLib,
                          sizeof (mPersistenceMarker),
                          (VOID *)mPersistenceMarker
                          );
  if (EFI_ERROR (Status)) {
    return CleanupAndRecordFailure (
             RT,
             StandardLib,
             LoggingLib,
             ProfileLib,
             RecoveryLib,
             L"Could not save persistence state before reset.",
             (UINTN)__LINE__
             );
  }

  StandardLib->RecordMessage (
                 StandardLib,
                 EFI_VERBOSE_LEVEL_DEFAULT,
                 L"TestKEK4 was installed and validated at T1."
                 );
  SctPrint (L"System will cold reset to verify KEK persistence.\n");
  gtBS->Stall (1000000);
  RT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);

  return CleanupAndRecordFailure (
           RT,
           StandardLib,
           LoggingLib,
           ProfileLib,
           RecoveryLib,
           L"ResetSystem returned unexpectedly.",
           (UINTN)__LINE__
           );
}

STATIC
EFI_STATUS
SecureBootAuthVariablePersistenceRecover (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib,
  IN EFI_TEST_PROFILE_LIBRARY_PROTOCOL    *ProfileLib,
  IN EFI_TEST_RECOVERY_LIBRARY_PROTOCOL   *RecoveryLib,
  OUT BOOLEAN                             *RecoveryHandled
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  T2Status;
  EFI_STATUS  TeardownStatus;
  EFI_STATUS  ClearStatus;
  UINT8       Record[PERSISTENCE_MARKER_SIZE];
  UINTN       RecordSize;
  BOOLEAN     PersistenceValid;
  BOOLEAN     ReplayValid;
  BOOLEAN     T2Valid;

  *RecoveryHandled = FALSE;
  RecordSize = sizeof (Record);
  Status = RecoveryLib->ReadResetRecord (RecoveryLib, &RecordSize, Record);
  if (Status == EFI_NOT_FOUND) {
    return EFI_SUCCESS;
  }

  *RecoveryHandled = TRUE;
  if (EFI_ERROR (Status) ||
      (RecordSize != sizeof (mPersistenceMarker)) ||
      (SctCompareMem (Record, (VOID *)mPersistenceMarker, sizeof (Record)) != 0)) {
    TeardownStatus = SecureBootVariableCleanup (
                       RT,
                       StandardLib,
                       LoggingLib,
                       ProfileLib
                       );
    ClearStatus = ClearPersistenceRecord (StandardLib, RecoveryLib);
    if (EFI_ERROR (TeardownStatus) ||
        EFI_ERROR (ClearStatus)) {
      StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"Recovery cleanup status: teardown=%r, reset record=%r.",
                     TeardownStatus,
                     ClearStatus
                     );
    }
    return RecordPersistenceAssertion (
             StandardLib,
             EFI_TEST_ASSERTION_FAILED,
             L"The persistence recovery record is unreadable or invalid.",
             (UINTN)__LINE__
             );
  }

  Status = ValidateTestKek4 (RT, StandardLib, &PersistenceValid);
  if (EFI_ERROR (Status)) {
    PersistenceValid = FALSE;
  }

  ReplayValid = FALSE;
  T2Valid = FALSE;
  T2Status = EFI_NOT_READY;
  if (PersistenceValid) {
    Status = SetKekFromFile (RT, L"TestKEK4T1.auth");
    if (Status == EFI_SECURITY_VIOLATION) {
      ReplayValid = TRUE;
      StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"Authenticated TestKEK4 payload with T1 timestamp was "
                     L"correctly rejected with EFI_SECURITY_VIOLATION."
                     );
    } else {
      StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"Authenticated TestKEK4 payload with T1 timestamp "
                     L"returned %r instead of EFI_SECURITY_VIOLATION.",
                     Status
                     );
    }

    T2Status = UpdateTestKek4AtT2 (RT, StandardLib);
    if (T2Status == EFI_SUCCESS) {
      T2Valid = TRUE;
      StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"Authenticated TestKEK4 payload with newer T2 timestamp "
                     L"was accepted successfully."
                     );
    }
  }

  TeardownStatus = SecureBootVariableCleanup (
                     RT,
                     StandardLib,
                     LoggingLib,
                     ProfileLib
                     );
  if (EFI_ERROR (TeardownStatus)) {
    if (PersistenceValid) {
      StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"TestKEK4 payload with newer T2 timestamp returned %r "
                     L"and normal Secure Boot teardown failed: %r.",
                     T2Status,
                     TeardownStatus
                     );
    } else {
      StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"Normal Secure Boot teardown failed after KEK persistence "
                     L"failure: %r.",
                     TeardownStatus
                     );
    }
  }

  ClearStatus = ClearPersistenceRecord (StandardLib, RecoveryLib);
  if (!PersistenceValid) {
    return RecordPersistenceAssertion (
             StandardLib,
             EFI_TEST_ASSERTION_FAILED,
             L"TestKEK4 did not persist unchanged across the cold reset.",
             (UINTN)__LINE__
             );
  }

  if (!ReplayValid) {
    return RecordPersistenceAssertion (
             StandardLib,
             EFI_TEST_ASSERTION_FAILED,
             L"The authenticated TestKEK4 payload with T1 timestamp was not "
             L"rejected after reset.",
             (UINTN)__LINE__
             );
  }

  if (!T2Valid) {
    return RecordPersistenceAssertion (
             StandardLib,
             EFI_TEST_ASSERTION_FAILED,
             L"The authenticated TestKEK4 payload with newer T2 timestamp was "
             L"not accepted.",
             (UINTN)__LINE__
             );
  }

  if (EFI_ERROR (TeardownStatus) || EFI_ERROR (ClearStatus)) {
    return RecordPersistenceAssertion (
             StandardLib,
             EFI_TEST_ASSERTION_FAILED,
             L"Secure Boot teardown or recovery-record cleanup failed after the persistence test.",
             (UINTN)__LINE__
             );
  }

  return RecordPersistenceAssertion (
           StandardLib,
           EFI_TEST_ASSERTION_PASSED,
           L"TestKEK4 and its T1 anti-replay timestamp persisted, and the "
           L"payload with newer T2 timestamp was accepted.",
           (UINTN)__LINE__
           );
}

EFI_STATUS
SecureBootAuthVariablePersistenceTest (
  IN EFI_BB_TEST_PROTOCOL  *This,
  IN VOID                  *ClientInterface,
  IN EFI_TEST_LEVEL        TestLevel,
  IN EFI_HANDLE            SupportHandle
  )
{
  EFI_STATUS                            Status;
  EFI_RUNTIME_SERVICES                  *RT;
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL    *StandardLib;
  EFI_TEST_RECOVERY_LIBRARY_PROTOCOL    *RecoveryLib;
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL     *ProfileLib;
  EFI_TEST_LOGGING_LIBRARY_PROTOCOL     *LoggingLib;
  BOOLEAN                               RecoveryHandled;

  Status = GetTestSupportLibrary (
             SupportHandle,
             &StandardLib,
             &RecoveryLib,
             &ProfileLib,
             &LoggingLib
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (FALSE == CheckBBTestCanRunAndRecordAssertion (
                  StandardLib,
                  L"Secure Boot authenticated KEK persistence test not supported in EFI",
                  __FILE__,
                  (UINTN)__LINE__
                  )) {
    return EFI_SUCCESS;
  }

  if (LoggingLib != NULL) {
    LoggingLib->EnterFunction (
                  LoggingLib,
                  L"SecureBootAuthVariablePersistenceTest",
                  L"PK-authenticated TestKEK4 update and anti-replay persistence"
                  );
  }

  RT = (EFI_RUNTIME_SERVICES *)ClientInterface;
  Status = GetSystemData (ProfileLib);
  if (EFI_ERROR (Status)) {
    Status = RecordPersistenceAssertion (
               StandardLib,
               EFI_TEST_ASSERTION_FAILED,
               L"Could not locate the Secure Boot dependency directory.",
               (UINTN)__LINE__
               );
    goto Exit;
  }

  Status = SecureBootAuthVariablePersistenceRecover (
             RT,
             StandardLib,
             LoggingLib,
             ProfileLib,
             RecoveryLib,
             &RecoveryHandled
             );
  if (!RecoveryHandled && !EFI_ERROR (Status)) {
    Status = SecureBootAuthVariablePersistenceStart (
               RT,
               StandardLib,
               LoggingLib,
               ProfileLib,
               RecoveryLib
               );
  }

Exit:
  if (LoggingLib != NULL) {
    LoggingLib->ExitFunction (
                  LoggingLib,
                  L"SecureBootAuthVariablePersistenceTest",
                  L"PK-authenticated TestKEK4 update and anti-replay persistence"
                  );
  }

  return Status;
}

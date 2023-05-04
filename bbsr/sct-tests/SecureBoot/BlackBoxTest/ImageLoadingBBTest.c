/** @file

  Copyright 2006 - 2012 Unified EFI, Inc.<BR>
  Copyright (c) 2010 - 2018, Intel Corporation. All rights reserved.<BR>
  Copyright 2021, Arm LTD.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php
 
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 
**/
/*++

Module Name:
  ImageLoadingBBTest.c

Abstract:
  Source file for Secure Boot Image Loading Black-Box Test - Conformance Test.

--*/

#include "SctLib.h"
#include "SecureBootBBTestMain.h"
#include "SecureBootBBTestSupport.h"

#define KEK_ATTRIBUTES (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | \
                       EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS)

typedef UINT32 EFI_IMAGE_EXECUTION_ACTION;
#define EFI_IMAGE_EXECUTION_AUTHENTICATION 0x00000007
#define EFI_IMAGE_EXECUTION_AUTH_UNTESTED 0x00000000
#define EFI_IMAGE_EXECUTION_AUTH_SIG_FAILED 0x00000001
#define EFI_IMAGE_EXECUTION_AUTH_SIG_PASSED 0x00000002
#define EFI_IMAGE_EXECUTION_AUTH_SIG_NOT_FOUND 0x00000003
#define EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND 0x00000004
#define EFI_IMAGE_EXECUTION_POLICY_FAILED 0x00000005
#define EFI_IMAGE_EXECUTION_INITIALIZED 0x00000008

typedef struct {
 EFI_IMAGE_EXECUTION_ACTION Action;
 UINT32 InfoSize;
// CHAR16 Name[…];
// EFI_DEVICE_PATH_PROTOCOL DevicePath;
// EFI_SIGNATURE_LIST Signature;
} EFI_IMAGE_EXECUTION_INFO;


//
// globals variables
//

UINTN                    *NumberOfImages;
EFI_IMAGE_EXECUTION_INFO *ImageExecutionInfo;

//
// Prototypes (external)
//

EFI_STATUS
ImageLoadingTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );


//
// Prototypes (internal)
//

EFI_STATUS
ImageLoadingVariableInit  (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib,
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL   *ProfileLib
  );

EFI_STATUS
ImageLoadingTestCheckpoint1 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib,
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL   *ProfileLib
  );

EFI_STATUS
ImageLoadingTestCheckpoint2 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib,
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL   *ProfileLib
  );

EFI_STATUS
VerifyImageEntry (
  IN  CHAR16          *FileName,
  IN  EFI_IMAGE_EXECUTION_ACTION Action
  );

EFI_STATUS
ImageLoadingTestCheckpoint3 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib,
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL   *ProfileLib
  );


//
// Functions
//

/**
 *  Entry point for Secure Boot Image Loading Test.
 *  @param This             A pointer to the EFI_BB_TEST_PROTOCOL instance.
 *  @param ClientInterface  A pointer to the interface to be tested.
 *  @param TestLevel        Test "thoroughness" control.
 *  @param SupportHandle    A handle containing support protocols.
 *  @return EFI_SUCCESS     Successfully.
 *  @return Other value     Something failed.
 */
EFI_STATUS
ImageLoadingTest(
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  )
{
  EFI_STATUS                          Status;
  EFI_RUNTIME_SERVICES                *RT;
  EFI_STANDARD_TEST_LIBRARY_PROTOCOL  *StandardLib;
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL   *ProfileLib;
  EFI_TEST_LOGGING_LIBRARY_PROTOCOL   *LoggingLib;

  //
  // Get test support library interfaces
  //
  Status = GetTestSupportLibrary (
             SupportHandle,
             &StandardLib,
             &ProfileLib,
             &LoggingLib
             );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  RT = (EFI_RUNTIME_SERVICES *)ClientInterface;

  //
  // Get the system device path and file path
  //
  Status = GetSystemData (ProfileLib);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Initialize KEK, db, dbx
  Status = ImageLoadingVariableInit (RT, StandardLib, LoggingLib, ProfileLib);
  if (EFI_ERROR(Status)) {
    SecureBootVariableCleanup (RT, StandardLib, LoggingLib, ProfileLib);
    return Status;
  }

  Status = ImageLoadingTestCheckpoint1 (RT, StandardLib, LoggingLib, ProfileLib);
  if (EFI_ERROR(Status)) {
    SecureBootVariableCleanup (RT, StandardLib, LoggingLib, ProfileLib);
    return Status;
  }


  Status = ImageLoadingTestCheckpoint2 (RT, StandardLib, LoggingLib, ProfileLib);
  if (EFI_ERROR(Status)) {
    SecureBootVariableCleanup (RT, StandardLib, LoggingLib, ProfileLib);
    return Status;
  }

  ImageLoadingTestCheckpoint3 (RT, StandardLib, LoggingLib, ProfileLib);

  // Restore KEK, db, dbx
  Status = SecureBootVariableCleanup (RT, StandardLib, LoggingLib, ProfileLib);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Done
  //
  return EFI_SUCCESS;

}

/**
 *  Initialize Secure Boot variables for this test
 *
 *  @param StandardLib    A pointer to EFI_STANDARD_TEST_LIBRARY_PROTOCOL
 *                        instance.
 *  @param LoggingLib     A pointer to EFI_TEST_LOGGING_LIBRARY_PROTOCOL
 *                        instance.
 *  @param ProfileLib     A pointer to EFI_TEST_PROFILE_LIBRARY_PROTOCOL
 *                        instance.
 *  @return EFI_SUCCESS   Successfully.
 *  @return Other value   Something failed.
 */
EFI_STATUS
ImageLoadingVariableInit  (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib,
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL   *ProfileLib
  )
{
  EFI_STATUS            Status;
  UINTN                 DataSize;
  UINT8                 Data[MAX_BUFFER_SIZE];
  UINT32                Attributes;
  EFI_FILE_HANDLE       KeyFHandle;
  UINT32                KeyFileSize;
  CHAR16                *FileName;
  VOID                  *Buffer;
  UINTN                 BufferSize;
  UINT32                DBAttributes;
  UINT32                DBXAttributes;

  DataSize = MAX_BUFFER_SIZE;
  Status = RT->GetVariable (
                 L"SecureBoot",               // VariableName
                 &gEfiGlobalVariableGuid,     // VendorGuid
                 &Attributes,                 // Attributes
                 &DataSize,                   // DataSize
                 Data                         // Data
                 );

  // if SecureBoot is not enabled, exit
  if (EFI_ERROR(Status) || Data[0] != 1) {
    StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"Secure Boot ImageLoadingTest: SecureBoot not enabled\n"
                     );
    return EFI_NOT_FOUND;
  }

  //
  // Initialize KEK
  //

  FileName = L"KEKSigList1.auth";

  //
  //read the key file into memory.
  //
  Status = OpenFileAndGetSize (
             FileName,
             &KeyFHandle,
             &KeyFileSize
             );

  if (EFI_ERROR(Status)) {
    return EFI_NOT_FOUND;
  }

  Buffer = SctAllocatePool (KeyFileSize);

  if (Buffer == NULL) {
    KeyFHandle->Close (KeyFHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  BufferSize = KeyFileSize;

  Status = KeyFHandle->Read (
                      KeyFHandle,
                      &BufferSize,
                      Buffer
                      );

  if (EFI_ERROR(Status)) {
    KeyFHandle->Close (KeyFHandle);
    gtBS->FreePool (Buffer);
    return EFI_LOAD_ERROR;
  }

  Status = RT->SetVariable (
                     L"KEK",                    // VariableName
                     &gEfiGlobalVariableGuid,   // VendorGuid
                     KEK_ATTRIBUTES,            // Attributes
                     BufferSize,                // DataSize
                     Buffer                     // Data
                     );

  gtBS->FreePool (Buffer);

  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"Secure Boot ImageLoadingTest: unable to set KEK. Status=%r.\n",
                     Status
                     );
    return EFI_NOT_FOUND;
  }


  //
  // Initialize db
  //

  // get db variable attributes
  DataSize = 0;
  Status = RT->GetVariable (
                 L"db",                          // VariableName
                 &gEfiImageSecurityDatabaseGuid, // VendorGuid
                 &DBAttributes,                  // Attributes
                 &DataSize,                      // DataSize
                 NULL                            // Data
                 );

  FileName = L"dbSigList2.auth";

  //
  //read the key file into memory.
  //
  Status = OpenFileAndGetSize (
             FileName,
             &KeyFHandle,
             &KeyFileSize
             );

  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"Secure Boot ImageLoadingTest: couldn't load dbSigList2.auth. Status=%r.\n",
                     Status
                     );
    return EFI_NOT_FOUND;
  }

  Buffer = SctAllocatePool (KeyFileSize);

  if (Buffer == NULL) {
    KeyFHandle->Close (KeyFHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  BufferSize = KeyFileSize;

  Status = KeyFHandle->Read (
                      KeyFHandle,
                      &BufferSize,
                      Buffer
                      );

  if (EFI_ERROR(Status)) {
    KeyFHandle->Close (KeyFHandle);
    gtBS->FreePool (Buffer);
    return EFI_LOAD_ERROR;
  }

  Status = RT->SetVariable (
                     L"db",                    // VariableName
                     &gEfiImageSecurityDatabaseGuid,  // Vendor GUID
                     DBAttributes,            // Attributes
                     BufferSize,                // DataSize
                     Buffer                     // Data
                     );

  gtBS->FreePool (Buffer);

  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"Secure Boot ImageLoadingTest: unable to set db. Status=%r.\n",
                     Status
                     );
    return EFI_NOT_FOUND;
  }


  //
  // Initialize dbx
  //

  // get dbx variable attributes
  DataSize = 0;
  Status = RT->GetVariable (
                 L"dbx",                          // VariableName
                 &gEfiImageSecurityDatabaseGuid, // VendorGuid
                 &DBXAttributes,                    // Attributes
                 &DataSize,                      // DataSize
                 NULL                            // Data
                 );

  FileName = L"dbxRevokedList1.auth";

  //
  //read the key file into memory.
  //
  Status = OpenFileAndGetSize (
             FileName,
             &KeyFHandle,
             &KeyFileSize
             );

  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"Secure Boot ImageLoadingTest: unable to load dbxRevokedList1.auth. Status=%r.\n",
                     Status
                     );
    return EFI_NOT_FOUND;
  }

  Buffer = SctAllocatePool (KeyFileSize);

  if (Buffer == NULL) {
    KeyFHandle->Close (KeyFHandle);
    return EFI_OUT_OF_RESOURCES;
  }

  BufferSize = KeyFileSize;

  Status = KeyFHandle->Read (
                      KeyFHandle,
                      &BufferSize,
                      Buffer
                      );

  if (EFI_ERROR(Status)) {
    KeyFHandle->Close (KeyFHandle);
    gtBS->FreePool (Buffer);
    return EFI_LOAD_ERROR;
  }

  Status = RT->SetVariable (
                     L"dbx",                    // VariableName
                     &gEfiImageSecurityDatabaseGuid,  // Vendor GUID
                     DBXAttributes,            // Attributes
                     BufferSize,                // DataSize
                     Buffer                     // Data
                     );

  gtBS->FreePool (Buffer);

  if (EFI_ERROR(Status)) {
    StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"Secure Boot ImageLoadingTest: unable to set dbx. Status=%r.\n",
                     Status
                     );
    return EFI_NOT_FOUND;
  }


  //
  // Done
  //
  return EFI_SUCCESS;
}

/**
 *  Verify image loading for images with certificates in db
 *
 *  @param StandardLib    A pointer to EFI_STANDARD_TEST_LIBRARY_PROTOCOL
 *                        instance.
 *  @param LoggingLib     A pointer to EFI_TEST_LOGGING_LIBRARY_PROTOCOL
 *                        instance.
 *  @param ProfileLib     A pointer to EFI_TEST_PROFILE_LIBRARY_PROTOCOL
 *                        instance.
 *  @return EFI_SUCCESS   Successfully.
 *  @return Other value   Something failed.
 */
EFI_STATUS
ImageLoadingTestCheckpoint1 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib,
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL   *ProfileLib
  )
{
  EFI_STATUS               Status;
  EFI_TEST_ASSERTION       Result;
  EFI_DEVICE_PATH_PROTOCOL *FilePath;
  CHAR16                   *EntireFileName;
  CHAR16                   *FileName;
  EFI_HANDLE               ImageHandle;

  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->EnterFunction (
                  LoggingLib,
                  L"ImageLoadingTestCheckpoint1",
                  L"UEFI spec, 32.4, 32.4.1"
                  );
  }

  //
  //  Test assertion 1: Verify loading image with no signature.  Load image (TestImage1)
  //   with no signature.  Expect result: SECURITY_VIOLATION or ACCESS_DENIED.
  //

  FileName = L"TestImage1.bin";
  EntireFileName = SctPoolPrint (L"%s\\%s", gFilePath, FileName);
  FilePath = SctFileDevicePath (gDeviceHandle, EntireFileName);
  SctFreePool (EntireFileName);

  ImageHandle = NULL;
  Status = gtBS->LoadImage (
                     FALSE,
                     gImageHandle,
                     FilePath,
                     NULL,
                     0,
                     &ImageHandle
                     );

  // As per UEFI spec, valid return codes for EFI_BOOT_SERVICES.LoadImage()
  // for signature check failures are security violation or access denied.
  if (Status == EFI_SECURITY_VIOLATION) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else if (Status == EFI_ACCESS_DENIED && ImageHandle == NULL) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else {
    Result = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid001,
                 L"SecureBoot - Verify load of unsigned image.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  //  Test assertion 2: Verify loading signed image, but not in db. Load image (TestImage2)
  //  with signature matching no keys in DBSigList2.  Expect result: SECURITY_VIOLATION or
  //  access denied.
  //

  FileName = L"TestImage2.bin";
  EntireFileName = SctPoolPrint (L"%s\\%s", gFilePath, FileName);
  FilePath = SctFileDevicePath (gDeviceHandle, EntireFileName);
  SctFreePool (EntireFileName);

  ImageHandle = NULL;
  Status = gtBS->LoadImage (
                     FALSE,
                     gImageHandle,
                     FilePath,
                     NULL,
                     0,
                     &ImageHandle
                     );

  if (Status == EFI_SECURITY_VIOLATION) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else if (Status == EFI_ACCESS_DENIED && ImageHandle == NULL) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else {
    Result = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid002,
                 L"SecureBoot - Verify load of signed image, but not in db.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  //  Test assertion 3: Verify signed image with first signature in list.  Load image
  //   (TestImage3) signed with first certificate in DBSigList2.  Expected result: SUCCESS.
  //

  FileName = L"TestImage3.bin";
  EntireFileName = SctPoolPrint (L"%s\\%s", gFilePath, FileName);
  FilePath = SctFileDevicePath (gDeviceHandle, EntireFileName);
  SctFreePool (EntireFileName);

  ImageHandle = NULL;
  Status = gtBS->LoadImage (
                     FALSE,
                     gImageHandle,
                     FilePath,
                     NULL,
                     0,
                     &ImageHandle
                     );

  if (Status == EFI_SUCCESS) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else {
    Result = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid003,
                 L"SecureBoot - Verify load of signed image, first signature in list.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  //  Test assertion 4: Verify signed image with 2nd signature in list.  Load image (TestImage4)
  //   signed with second certificate in DBSigList2.  Expected result: SUCCESS.
  //

  FileName = L"TestImage4.bin";
  EntireFileName = SctPoolPrint (L"%s\\%s", gFilePath, FileName);
  FilePath = SctFileDevicePath (gDeviceHandle, EntireFileName);
  SctFreePool (EntireFileName);

  ImageHandle = NULL;
  Status = gtBS->LoadImage (
                     FALSE,
                     gImageHandle,
                     FilePath,
                     NULL,
                     0,
                     &ImageHandle
                     );

  if (Status == EFI_SUCCESS) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else {
    Result = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid004,
                 L"SecureBoot - Verify load of signed image, second signature in list.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  //  Test assertion 5: Verify unsigned image with hash in DB.  Load unsigned image (TestImage5)
  //   with SHA256 hash in DBSigList2.  Expected result: SUCCESS
  //

  FileName = L"TestImage5.bin";
  EntireFileName = SctPoolPrint (L"%s\\%s", gFilePath, FileName);
  FilePath = SctFileDevicePath (gDeviceHandle, EntireFileName);
  SctFreePool (EntireFileName);

  ImageHandle = NULL;
  Status = gtBS->LoadImage (
                     FALSE,
                     gImageHandle,
                     FilePath,
                     NULL,
                     0,
                     &ImageHandle
                     );

  if (Status == EFI_SUCCESS) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else {
    Result = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid005,
                 L"SecureBoot - Verify load of unsigned image with hash in list.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // Done
  //
  return EFI_SUCCESS;

}

/**
 *  Verify image loading when images are revoked in dbx
 *
 *  @param StandardLib    A pointer to EFI_STANDARD_TEST_LIBRARY_PROTOCOL
 *                        instance.
 *  @param LoggingLib     A pointer to EFI_TEST_LOGGING_LIBRARY_PROTOCOL
 *                        instance.
 *  @param ProfileLib     A pointer to EFI_TEST_PROFILE_LIBRARY_PROTOCOL
 *                        instance.
 *  @return EFI_SUCCESS   Successfully.
 *  @return Other value   Something failed.
 */
EFI_STATUS
ImageLoadingTestCheckpoint2 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib,
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL   *ProfileLib
  )
{
  EFI_STATUS               Status;
  EFI_TEST_ASSERTION       Result;
  EFI_DEVICE_PATH_PROTOCOL *FilePath;
  CHAR16                   *EntireFileName;
  CHAR16                   *FileName;
  EFI_HANDLE               ImageHandle;

  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->EnterFunction (
                  LoggingLib,
                  L"ImageLoadingTestCheckpoint2",
                  L"UEFI spec, 32.4, 32.4.1"
                  );
  }

  //
  //  Test assertion 1: Verify revocation with signed image with signature in db 
  //  but revoked in dbx with EFI_CERT_X509_SHA256_GUID.  Load image (TestImage6)
  //  signed with RevokedCert1.  Expect result: SECURITY_VIOLATION.
  //

  FileName = L"TestImage6.bin";
  EntireFileName = SctPoolPrint (L"%s\\%s", gFilePath, FileName);
  FilePath = SctFileDevicePath (gDeviceHandle, EntireFileName);
  SctFreePool (EntireFileName);

  ImageHandle = NULL;
  Status = gtBS->LoadImage (
                     FALSE,
                     gImageHandle,
                     FilePath,
                     NULL,
                     0,
                     &ImageHandle
                     );

  if (Status == EFI_SECURITY_VIOLATION) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else if (Status == EFI_ACCESS_DENIED && ImageHandle == NULL) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else {
    Result = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid006,
                 L"SecureBoot - Verify load of signed image with signature in db and revoked in dbx with SHA256 hash.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  //  Test assertion 2: Verify revocation with signed image with signature in db but
  //  revoked in dbx with EFI_CERT_X509_SHA384_GUID.  Load image (TestImage7) signed
  //  with RevokedCert2.  Expect result: SECURITY_VIOLATION.
  //

  FileName = L"TestImage7.bin";
  EntireFileName = SctPoolPrint (L"%s\\%s", gFilePath, FileName);
  FilePath = SctFileDevicePath (gDeviceHandle, EntireFileName);
  SctFreePool (EntireFileName);

  ImageHandle = NULL;
  Status = gtBS->LoadImage (
                     FALSE,
                     gImageHandle,
                     FilePath,
                     NULL,
                     0,
                     &ImageHandle
                     );

  if (Status == EFI_SECURITY_VIOLATION) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else if (Status == EFI_ACCESS_DENIED && ImageHandle == NULL) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else {
    Result = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid007,
                 L"SecureBoot - Verify load of signed image with signature in db and revoked in dbx with SHA384 hash.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  //  Test assertion 3: Verify revocation with signed image (TestImage8) with signature in
  //  db but revoked in dbx with EFI_CERT_X509_SHA512_GUID.  Load image signed with
  //  RevokedCert3.  Expect result: SECURITY_VIOLATION.
  //

  FileName = L"TestImage8.bin";
  EntireFileName = SctPoolPrint (L"%s\\%s", gFilePath, FileName);
  FilePath = SctFileDevicePath (gDeviceHandle, EntireFileName);
  SctFreePool (EntireFileName);

  ImageHandle = NULL;
  Status = gtBS->LoadImage (
                     FALSE,
                     gImageHandle,
                     FilePath,
                     NULL,
                     0,
                     &ImageHandle
                     );

  if (Status == EFI_SECURITY_VIOLATION) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else if (Status == EFI_ACCESS_DENIED && ImageHandle == NULL) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else {
    Result = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid008,
                 L"SecureBoot - Verify load of signed image with signature in db and revoked in dbx with SHA512 hash.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  //  Test assertion 4: Verify revocation with image (TestImage9) with signature in db,
  //  but signature also in DBXRevokedList1.  Load image signed with RevokedCert4.
  //  Expect result: SECURITY_VIOLATION.
  //

  FileName = L"TestImage9.bin";
  EntireFileName = SctPoolPrint (L"%s\\%s", gFilePath, FileName);
  FilePath = SctFileDevicePath (gDeviceHandle, EntireFileName);
  SctFreePool (EntireFileName);

  ImageHandle = NULL;
  Status = gtBS->LoadImage (
                     FALSE,
                     gImageHandle,
                     FilePath,
                     NULL,
                     0,
                     &ImageHandle
                     );

  if (Status == EFI_SECURITY_VIOLATION) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else if (Status == EFI_ACCESS_DENIED && ImageHandle == NULL) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else {
    Result = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid009,
                 L"SecureBoot - Verify load of signed image with signature in db and revoked in dbx with certificate.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  //  Test assertion 5: Verify revocation with image with signature in db,
  //  but revoked with SHA256 hash of image in DBXRevokedList1.  Load image
  //  (TestImage10).  Expect result: SECURITY_VIOLATION.
  //

  FileName = L"TestImage10.bin";
  EntireFileName = SctPoolPrint (L"%s\\%s", gFilePath, FileName);
  FilePath = SctFileDevicePath (gDeviceHandle, EntireFileName);
  SctFreePool (EntireFileName);

  ImageHandle = NULL;
  Status = gtBS->LoadImage (
                     FALSE,
                     gImageHandle,
                     FilePath,
                     NULL,
                     0,
                     &ImageHandle
                     );

  if (Status == EFI_SECURITY_VIOLATION) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else if (Status == EFI_ACCESS_DENIED && ImageHandle == NULL) {
    Result = EFI_TEST_ASSERTION_PASSED;
  } else {
    Result = EFI_TEST_ASSERTION_FAILED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid010,
                 L"SecureBoot - Verify load of signed image with signature in db and revoked in dbx with hash of image.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );


  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->ExitFunction (
                  LoggingLib,
                  L"ImageLoadingTestCheckpoint2",
                  L"UEFI spec, 32.5, 32.4.1"
                  );
  }

  //
  // Done
  //
  return EFI_SUCCESS;

}

/**
 *  Verify if image is in Image Execution Info table and has correct action code.
 *  @param FileName Name of image.
 *  @param Result Result of check.
 *  @return EFI_SUCCESS The result of the check is found.
 */
EFI_STATUS
VerifyImageEntry (
  IN  CHAR16          *FileName,
  IN  EFI_IMAGE_EXECUTION_ACTION Action
  )
{
  EFI_IMAGE_EXECUTION_INFO *InfoPtr;
  VOID                     *ptr;
  CHAR16                   *ImagePath;
  CHAR16                   *ImageName;

  // Move ptr to begin of Image Execution Table
  ptr = ImageExecutionInfo;

  // Iterate through Image Execution Info Table
  for (int i = 0; i < *NumberOfImages; i++) {
    InfoPtr = ptr;
    ImagePath = ptr + sizeof(EFI_IMAGE_EXECUTION_INFO);
    ImageName = ImagePath;

    if (GetBaseName (ImagePath,&ImageName) == EFI_SUCCESS) {

      // Verify if this entries image name matches expected image name
      if (SctStrCmp(FileName,ImageName) == 0) {

        // Verify if this entries Action matches expected Action
        if (InfoPtr->Action == Action) {
           return EFI_SUCCESS;
        } else {
           return EFI_NOT_FOUND;
        }
      }
    }
    // advance pointer to next element in table
    ptr += InfoPtr->InfoSize;
  }

  return EFI_NOT_FOUND;
}


/**
 *  Verify image execution info table entries
 *
 *  @param StandardLib    A pointer to EFI_STANDARD_TEST_LIBRARY_PROTOCOL
 *                        instance.
 *  @param LoggingLib     A pointer to EFI_TEST_LOGGING_LIBRARY_PROTOCOL
 *                        instance.
 *  @param ProfileLib     A pointer to EFI_TEST_PROFILE_LIBRARY_PROTOCOL
 *                        instance.
 *  @return EFI_SUCCESS   Successfully.
 *  @return Other value   Something failed.
 */
EFI_STATUS
ImageLoadingTestCheckpoint3 (
  IN EFI_RUNTIME_SERVICES                 *RT,
  IN EFI_STANDARD_TEST_LIBRARY_PROTOCOL   *StandardLib,
  IN EFI_TEST_LOGGING_LIBRARY_PROTOCOL    *LoggingLib,
  EFI_TEST_PROFILE_LIBRARY_PROTOCOL   *ProfileLib
  )
{
  EFI_STATUS               Status;
  EFI_TEST_ASSERTION       Result;
  VOID                     *ptr;

  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->EnterFunction (
                  LoggingLib,
                  L"ImageLoadingTestCheckpoint3",
                  L"UEFI spec, 32.4, 32.4.2"
                  );
  }

  // Get pointer for Image Execution Info Table
  Status = SctGetSystemConfigurationTable(&gEfiImageSecurityDatabaseGuid, &ptr);

  if (Status != EFI_SUCCESS) {
    StandardLib->RecordMessage (
                     StandardLib,
                     EFI_VERBOSE_LEVEL_DEFAULT,
                     L"Secure Boot ImageLoadingTest: EFI Image Execution Info Table not found.\n"
                     );
     return EFI_NOT_FOUND;
  }

  // first element of table is number of images
  NumberOfImages = ptr;

  // advance pointer to the first IMAGE_EXECUTION_INFO entry
  ptr += sizeof(NumberOfImages);

  ImageExecutionInfo = ptr;

// UEFI 2.10 spec table 32.6
// EFI_IMAGE_EXECUTION_AUTH_UNTESTED The image contained no certificates
  Status = VerifyImageEntry(L"TestImage1.bin", EFI_IMAGE_EXECUTION_AUTH_UNTESTED);
  if (Status) {
    Result = EFI_TEST_ASSERTION_FAILED;
  } else {
    Result = EFI_TEST_ASSERTION_PASSED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid011,
                 L"SecureBoot - TestImage1.bin in Image Execution Info Table with AUTH_UNTESTED.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  Status = VerifyImageEntry(L"TestImage2.bin", EFI_IMAGE_EXECUTION_AUTH_SIG_NOT_FOUND);
  if (Status) {
    Result = EFI_TEST_ASSERTION_FAILED;
  } else {
    Result = EFI_TEST_ASSERTION_PASSED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid012,
                 L"SecureBoot - TestImage2.bin in Image Execution Info Table with SIG_NOT_FOUND.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  Status = VerifyImageEntry(L"TestImage6.bin", EFI_IMAGE_EXECUTION_AUTH_SIG_FAILED);
  if (Status) {
    Result = EFI_TEST_ASSERTION_FAILED;
  } else {
    Result = EFI_TEST_ASSERTION_PASSED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid013,
                 L"SecureBoot - TestImage6.bin in Image Execution Info Table with SIG_FAILED.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  Status = VerifyImageEntry(L"TestImage7.bin", EFI_IMAGE_EXECUTION_AUTH_SIG_FAILED);
  if (Status) {
    Result = EFI_TEST_ASSERTION_FAILED;
  } else {
    Result = EFI_TEST_ASSERTION_PASSED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid014,
                 L"SecureBoot - TestImage7.bin in Image Execution Info Table with SIG_FAILED.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  Status = VerifyImageEntry(L"TestImage8.bin", EFI_IMAGE_EXECUTION_AUTH_SIG_FAILED);
  if (Status) {
    Result = EFI_TEST_ASSERTION_FAILED;
  } else {
    Result = EFI_TEST_ASSERTION_PASSED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid015,
                 L"SecureBoot - TestImage8.bin in Image Execution Info Table with SIG_FAILED.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  Status = VerifyImageEntry(L"TestImage9.bin", EFI_IMAGE_EXECUTION_AUTH_SIG_FAILED);
  if (Status) {
    Result = EFI_TEST_ASSERTION_FAILED;
  } else {
    Result = EFI_TEST_ASSERTION_PASSED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid016,
                 L"SecureBoot - TestImage9.bin in Image Execution Info Table with SIG_FAILED.",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

// UEFI 2.10 spec table 32.6
// EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND The image has at least one certificate, and the image digest is in the forbidden database.
  Status = VerifyImageEntry(L"TestImage10.bin", EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND);
  if (Status) {
    Result = EFI_TEST_ASSERTION_FAILED;
  } else {
    Result = EFI_TEST_ASSERTION_PASSED;
  }

  StandardLib->RecordAssertion (
                 StandardLib,
                 Result,
                 gSecureBootImageLoadingBbTestAssertionGuid017,
                 L"SecureBoot - Verify load of TestImage10.bin recorded in Image Execution Info Table with AUTH_SIG_FOUND",
                 L"%a:%d:Status - %r",
                 __FILE__,
                 (UINTN)__LINE__,
                 Status
                 );

  //
  // Trace ...
  //
  if (LoggingLib != NULL) {
    LoggingLib->ExitFunction (
                  LoggingLib,
                  L"ImageLoadingTestCheckpoint2",
                  L"UEFI spec, 32.5, 32.4.2"
                  );
  }

  //
  // Done
  //
  return EFI_SUCCESS;
}

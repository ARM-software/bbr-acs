/** @file

  Copyright 2006 - 2016 Unified EFI, Inc.<BR>
  Copyright (c) 2021-2024, Arm Inc. All rights reserved.<BR>


  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at 
  http://opensource.org/licenses/bsd-license.php
 
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 
**/
/*++

Module Name:
  SbbrEfiFgtFeaturesTestMain.h

Abstract:
  Header file for SBBR EFI FGT featrures Black-Box Test.

--*/

//
// Includes
//
#include "Efi.h"
#include "Guid.h"
#include <Library/EfiTestLib.h>
#include <Protocol/Cpu.h>

#include EFI_TEST_PROTOCOL_DEFINITION(TestRecoveryLibrary)
#include EFI_TEST_PROTOCOL_DEFINITION(TestLoggingLibrary)

//
// Definitions
//

#define SBBR_EFI_FGT_FEATURES_BB_TEST_REVISION    0x00010000

#define SBBR_EFI_FGT_FEATURES_BB_TEST_GUID        \
  { 0x5D6AF07A, 0x5906, 0x45B0, {0x9C, 0xAF, 0x03, 0xD5, 0xE0, 0x24, 0x09, 0x21 }}

#define VAL_EXTRACT_BITS(data, start, end) ((data >> start) & ((1ul << (end-start+1))-1))

#define GET_VARIABLE  0
#define SET_VARIABLE  1

#define FGT_TEST_IS_IN_PROGRESS  1
#define FGT_TEST_COMPLETED       2

EFI_STATUS
SbbrEfiFgtFeaturesTest (
  IN EFI_BB_TEST_PROTOCOL       *This,
  IN VOID                       *ClientInterface,
  IN EFI_TEST_LEVEL             TestLevel,
  IN EFI_HANDLE                 SupportHandle
  );

//
// Support functions
//
EFI_STATUS
GetTestSupportLibrary (
  IN EFI_HANDLE                           SupportHandle,
  OUT EFI_STANDARD_TEST_LIBRARY_PROTOCOL  **StandardLib,
  OUT EFI_TEST_RECOVERY_LIBRARY_PROTOCOL  **RecoveryLib,
  OUT EFI_TEST_LOGGING_LIBRARY_PROTOCOL   **LoggingLib
  );

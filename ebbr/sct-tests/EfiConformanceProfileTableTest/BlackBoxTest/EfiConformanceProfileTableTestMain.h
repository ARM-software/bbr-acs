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
  EfiConformanceProfileTableTestMain.h

Abstract:
  Header file for EFI Conformance Profile Table Black-Box Test.

--*/

//
// Includes
//
#include "Efi.h"
#include "Guid.h"
#include <Library/EfiTestLib.h>
#include <Guid/ConformanceProfiles.h>
#include <Library/PrintLib.h>

#include EFI_TEST_PROTOCOL_DEFINITION(TestRecoveryLibrary)
#include EFI_TEST_PROTOCOL_DEFINITION(TestLoggingLibrary)

//
// Definitions
//

#define EFI_CONFORMANCE_PROFILE_TABLE_BB_TEST_REVISION    0x00010000

#define EFI_CONFORMANCE_PROFILE_TABLE_BB_TEST_GUID        \
  { 0x14bc21ec, 0x6d04, 0x4684, {0xa8, 0x92, 0x1b, 0x46, 0x5a, 0xca, 0xc4, 0x23 }}

EFI_STATUS
EfiConformanceProfileTableTest (
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

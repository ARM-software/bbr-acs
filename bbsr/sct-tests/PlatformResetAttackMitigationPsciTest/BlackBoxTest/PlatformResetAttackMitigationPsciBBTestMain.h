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
  PlatformResetAttackMitigationPsciBBTestMain.h

Abstract:
  Header file for Platform Reset Attack Mitigation via PSCI Black-Box Test.

--*/

//
// Includes
//
#include "Efi.h"
#include "Guid.h"
#include <Library/EfiTestLib.h>

#include EFI_TEST_PROTOCOL_DEFINITION(TestRecoveryLibrary)
#include EFI_TEST_PROTOCOL_DEFINITION(TestLoggingLibrary)

//
// Definitions
//

#define PLAT_RESET_ATTACK_MITIGATION_PSCI_BB_TEST_REVISION    0x00010000

#define PLAT_RESET_ATTACK_MITIGATION_PSCI_BB_TEST_GUID        \
{0XC4567D24, 0XBB60, 0X484E, {0X80, 0XEC, 0XA3, 0X6A, 0XCE, 0XEA, 0X5C, 0X43} }

// PSCI function IDs defined in PSCI [https://developer.arm.com/documentation/den0022/latest/]
#define ARM_SMC_ID_PSCI_MEM_PROTECT     0x84000013
#define ARM_SMC_ID_PSCI_PSCI_FEATURES   0x8400000A

EFI_STATUS
PlatformResetAttackMitigationPsciTest (
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

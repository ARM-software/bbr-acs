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
#ifndef _SBBRRUNTIMESERVICES_TEST_MAIN_H_
#define _SBBRRUNTIMESERVICES_TEST_MAIN_H_

#include "Efi.h"

#include EFI_TEST_PROTOCOL_DEFINITION(TestRecoveryLibrary)

#define SBBRRUNTIMESERVICES_TEST_REVISION  0x00010000

#define SBBRRUNTIMESERVICES_TEST_GUID \
{ 0x52a69bbf, 0xe19f, 0x4e82, {0x93, 0xd6, 0xaa, 0xc4, 0x6e, 0x2e, 0x1e, 0x06 }}

#define TPL_ARRAY_SIZE  3

typedef struct {
  UINTN   Step;
  UINTN   TplIndex;
  UINT32  RepeatTimes;
} RESET_DATA;

EFI_STATUS
InitializeBBTestSBBRRuntimeServices (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

#define SBBRRUNTIMESERVICES_TEST_CASE_RESETSHUTDOWN_GUID \
{ 0x877143a4, 0xad97, 0x4213, {0x8f, 0x7f, 0x7d, 0xd3, 0x7e, 0xa4, 0x63, 0xd5 }}

#endif /* _SBBRRUNTIMESERVICES_TEST_MAIN_H_ */

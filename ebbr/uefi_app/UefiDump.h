/** @file

  Header file for the UEFI dump application.

  Copyright 2006-2016 Unified EFI, Inc.<BR>
  Copyright (c) 2026, Arm Ltd. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _UEFI_DUMP_H_
#define _UEFI_DUMP_H_

#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#define EBBR_PROFILE_TABLE_DUMP_FILENAME \
  L"\\acs_results_template\\acs_results\\uefi_dump\\ebbr_profile_table.log"
#define EBBR_PROFILE_TABLE_DUMP_FALLBACK_FILENAME \
  L"ebbr_profile_table.log"

EFI_STATUS
EFIAPI
UefiDump(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

#endif // _UEFI_DUMP_H_

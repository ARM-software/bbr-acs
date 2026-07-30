/** @file

  Copyright (c) 2026, Arm Ltd. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

/*++

Module Name:

  Guid.h

Abstract:

  GUIDs auto-generated for Secure Boot variable persistence test assertions.

--*/

#ifndef SECURE_BOOT_AUTH_VARIABLE_PERSISTENCE_GUID_H
#define SECURE_BOOT_AUTH_VARIABLE_PERSISTENCE_GUID_H

#define SECURE_BOOT_AUTH_VARIABLE_PERSISTENCE_TEST_GUID \
{ 0x3a719204, 0x9f8c, 0x4b97, { 0x98, 0x55, 0x14, 0xe5, 0x85, 0xee, 0xc9, 0x7a } }

#define EFI_TEST_SECUREBOOTVARIABLEPERSISTENCEBBTEST_ASSERTION_001_GUID \
{ 0xcfe95f50, 0x67a1, 0x4e56, { 0x8f, 0x2a, 0xef, 0x2f, 0xdd, 0xa3, 0x2e, 0x4f } }

extern EFI_GUID gSecureBootAuthVariablePersistenceBbTestAssertionGuid001;

#endif

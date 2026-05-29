#
# The material contained herein is not a license, either
# expressly or impliedly, to any intellectual property owned
# or controlled by any of the authors or developers of this
# material or to any contribution thereto. The material
# contained herein is provided on an "AS IS" basis and, to the
# maximum extent permitted by applicable law, this information
# is provided AS IS AND WITH ALL FAULTS, and the authors and
# developers of this material hereby disclaim all other
# warranties and conditions, either express, implied or
# statutory, including, but not limited to, any (if any)
# implied warranties, duties or conditions of merchantability,
# of fitness for a particular purpose, of accuracy or
# completeness of responses, of results, of workmanlike
# effort, of lack of viruses and of lack of negligence, all
# with regard to this material and any contribution thereto.
# Designers must not rely on the absence or characteristics of
# any features or instructions marked "reserved" or
# "undefined." The Unified EFI Forum, Inc. reserves any
# features or instructions so marked for future definition and
# shall have no responsibility whatsoever for conflicts or
# incompatibilities arising from future changes to them. ALSO,
# THERE IS NO WARRANTY OR CONDITION OF TITLE, QUIET ENJOYMENT,
# QUIET POSSESSION, CORRESPONDENCE TO DESCRIPTION OR
# NON-INFRINGEMENT WITH REGARD TO THE TEST SUITE AND ANY
# CONTRIBUTION THERETO.
#
# IN NO EVENT WILL ANY AUTHOR OR DEVELOPER OF THIS MATERIAL OR
# ANY CONTRIBUTION THERETO BE LIABLE TO ANY OTHER PARTY FOR
# THE COST OF PROCURING SUBSTITUTE GOODS OR SERVICES, LOST
# PROFITS, LOSS OF USE, LOSS OF DATA, OR ANY INCIDENTAL,
# CONSEQUENTIAL, DIRECT, INDIRECT, OR SPECIAL DAMAGES WHETHER
# UNDER CONTRACT, TORT, WARRANTY, OR OTHERWISE, ARISING IN ANY
# WAY OUT OF THIS OR ANY OTHER AGREEMENT RELATING TO THIS
# DOCUMENT, WHETHER OR NOT SUCH PARTY HAD ADVANCE NOTICE OF
# THE POSSIBILITY OF SUCH DAMAGES.
#
# Copyright 2006 - 2016 Unified EFI, Inc. All
# Rights Reserved, subject to all existing rights in all
# matters included within this Test Suite, to which United
# EFI, Inc. makes no claim of right.
#
# Copyright (c) 2026, ARM Ltd. All rights reserved.<BR>
#
#/*++
#
# Module Name:
#
#  UefiDump.dsc
#
# Abstract:
#
#  This is a build description file used to build the UEFI dump application, which currently dumps the EBBR conformance profiles table.
#--*/

[Defines]
  PLATFORM_NAME                  = UefiDump
  PLATFORM_GUID                  = 4fed62c9-b3cc-416e-82e5-0060b6b46a46
  PLATFORM_VERSION               = 1.0
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/MdeModule
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT

!include MdePkg/MdeLibs.dsc.inc

[LibraryClasses]
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  SortLib|MdeModulePkg/Library/BaseSortLib/BaseSortLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  DebugLib|MdePkg/Library/UefiDebugLibStdErr/UefiDebugLibStdErr.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf

[LibraryClasses.common.UEFI_APPLICATION]
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf

[Components]
  ebbr/uefi_app/UefiDump.inf

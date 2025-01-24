#/** @file
# Copyright (c) 2016-2020,2021,2024, Arm Limited or its affiliates. All rights reserved.
# SPDX-License-Identifier : Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#**/

//
// Private worker functions for ASM_PFX()
//
#define _CONCATENATE(a, b)  __CONCATENATE(a, b)
#define __CONCATENATE(a, b) a ## b

#define __USER_LABEL_PREFIX__
//
// The __USER_LABEL_PREFIX__ macro predefined by GNUC represents the prefix
// on symbols in assembly language.
//
#define ASM_PFX(name) _CONCATENATE (__USER_LABEL_PREFIX__, name)

#define GCC_ASM_EXPORT(func__)  \
       .global  _CONCATENATE (__USER_LABEL_PREFIX__, func__)    ;\
       .type ASM_PFX(func__), %function


.text
.align 3

GCC_ASM_EXPORT (AA64ReadSp)
GCC_ASM_EXPORT (AA64WriteSp)
GCC_ASM_EXPORT (AA64ReadMmfr0)
GCC_ASM_EXPORT (AA64ReadHdfgrtrEl2)

ASM_PFX(AA64ReadSp):
  mov x0, sp
  ret

ASM_PFX(AA64WriteSp):
  mov sp, x0
  ret

ASM_PFX(AA64ReadMmfr0):
  mrs x0, ID_AA64MMFR0_EL1
  ret

ASM_PFX(AA64ReadHdfgrtrEl2):
  mrs x0, S3_4_C3_C1_4 //HDFGRTR_EL2
  ret

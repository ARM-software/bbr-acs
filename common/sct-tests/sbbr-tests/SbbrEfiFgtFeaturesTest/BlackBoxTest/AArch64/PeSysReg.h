/** @file
 * Copyright (c) 2016-2020,2022-2024, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#ifndef __PE_REG_SYS_H__
#define __PE_REG_SYS_H__

//
//  AARCH64 processor exception types.
//
#define EXCEPT_AARCH64_SYNCHRONOUS_EXCEPTIONS    0
#define EXCEPT_AARCH64_IRQ                       1
#define EXCEPT_AARCH64_FIQ                       2
#define EXCEPT_AARCH64_SERROR                    3

UINT64 AA64ReadSp(void);

UINT64 AA64WriteSp(UINT64 write_data);

UINT64 AA64ReadHdfgrtrEl2(VOID);

UINT64 AA64ReadMmfr0(VOID);

#endif

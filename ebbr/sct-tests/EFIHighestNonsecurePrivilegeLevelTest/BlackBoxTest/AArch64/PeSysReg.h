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

UINT64 AA64ReadPfr0El1(VOID);

UINT64 AA64ReadCurrentEl(VOID);

UINT64 AA64ReadScrEl3(VOID);
#endif

#!/bin/sh

# @file
# Copyright (c) 2025, Arm Limited or its affiliates. All rights reserved.
# SPDX-License-Identifier : Apache-2.0

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

echo -off

set bbr_recipe sbbr  # Change to ebbr if needed

# Run the SCT test
for %i in 0 1 2 3 4 5 6 7 8 9 A B C D E F then
    if exist FS%i:\StandaloneSctStartup.nsh then
        echo "Running SCT test (%bbr_recipe%) from FS%i:"
        FS%i:\StandaloneSctStartup.nsh %bbr_recipe%
        goto DoneSCT
    endif
endfor

echo "ERROR: StandaloneSctStartup.nsh not found on any FS0..FSF."

:DoneSCT

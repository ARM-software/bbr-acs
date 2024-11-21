#!/usr/bin/env bash

# Copyright (c) 2021-2024, Arm Limited or its affiliates. All rights reserved.
#  SPDX-License-Identifier : Apache-2.0
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
##


echo -off

for %i in 0 1 2 3 4 5 6 7 8 9 A B C D E F
  if exist FS%i:\acs_tests\bbr\SCT\SCRT then
    #
    # Found EFI SCRT harness
    #
    FS%i:
    cd FS%i:\acs_tests\bbr\SCT\SCRT

    #Check if SCRT run is already in progress
    if  exist SCRT_run_progress.flag then
        #This is a restart case after SCRT run.
        #Save the logs and exit
        echo "SCRT run is in progress. Saving the logs"
        rm -q SCRT_run_progress.flag
        SCRTAPP.efi -g SCRT.log

        #Save the logs in acs_results
        for %j in 0 1 2 3 4 5 6 7 8 9 A B C D E F then
            if exists FS%j:\acs_results\ then
                mkdir FS%j:\acs_results\scrt_results
                mkdir FS%j:\acs_results\scrt_results
                cp SCRT.log  FS%j:\acs_results\scrt_results\SCRT.log
                cp SCRT.conf FS%j:\acs_results\scrt_results\SCRT.conf
            endif
        endfor

        goto Done
    endif

    echo SCRT run. Press any key to stop the EFI SCRT running
    FS%i:\acs_tests\bbr\SCT\stallforkey.efi 5
    if %lasterror% == 0 then
      goto Done
    endif

    echo "Note: The System will automatically reset as part of SCRT testing"

    if  exist SCRT.log then
        echo "SCRT is already run."
        echo "Press any key to run SCRT again."
        echo "WARNING: Ensure you have backed up the existing logs."
        FS%i:\acs_tests\bbr\SCT\stallforkey.efi 5
        if %lasterror% == 0 then
            #Backup the existing logs
            cp SCRT.log SCRT.log_previous_run
            rm -q SCRT.log
            goto StartSCRT
        else
            goto Done
        endif
    else
        goto StartSCRT
    endif


:StartSCRT
    cp SCRT.conf SCRT_run_progress.flag
    Load SCRTDRIVER.efi
    SCRTAPP -f SCRT.conf

 endif
endfor

:Done



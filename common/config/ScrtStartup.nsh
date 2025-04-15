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
        if %1 == "true" then
            FS%i:
            acs_tests\parser\Parser.efi -scrt
            if "%automation_scrt_run%" == "" then
                echo "automation_scrt_run variable does not exist"
            else
                if "%automation_scrt_run%" == "false" then
                    echo "************ SCRT is disabled in config file(acs_run_config.ini) ************"
                    goto Done
                endif
            endif
        endif
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
            mkdir FS%i:\acs_results\scrt_results
            if exist SCRT.log then
                mv SCRT.log  FS%i:\acs_results\scrt_results\SCRT.log
            else
                echo "SCRT run failed" > FS%i:\acs_results\scrt_results\SCRT.log
            endif

            cp SCRT.conf FS%i:\acs_results\scrt_results\SCRT.conf
            goto Done
        endif

        echo "Press any key to stop the EFI SCRT running"
        FS%i:\acs_tests\bbr\SCT\stallforkey.efi 5
        if %lasterror% == 0 then
            goto Done
        endif

        echo "Note: The System will automatically reset as part of SCRT testing"
        if  exist FS%i:\acs_results\scrt_results\SCRT.log then
            echo "SCRT is already run."
            echo "Press any key to run SCRT again."
            echo "WARNING: Ensure you have backed up the existing logs."
            FS%i:\acs_tests\bbr\SCT\stallforkey.efi 5
            if %lasterror% == 0 then
                #Backup the existing logs
                cp FS%i:\acs_results\scrt_results\SCRT.log FS%i:\acs_results\scrt_results\SCRT.log_previous_run
                rm -q FS%i:\acs_results\scrt_results\SCRT.log
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

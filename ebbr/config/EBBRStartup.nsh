#!/usr/bin/env bash

# Copyright (c) 2021-2025, Arm Limited or its affiliates. All rights reserved.
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
    if exist FS%i:\acs_tests\bbr\SCT then
        #
        # Found EFI SCT harness
        #
        FS%i:
        cd FS%i:\acs_tests\bbr\SCT
        echo "Press any key to stop the EFI SCT running"
        stallforkey.efi 5
        if %lasterror% == 0 then
            goto Done
        endif
        if exists FS%i:\acs_results_template\acs_results then
            if exists FS%i:\acs_results_template\acs_results\sct_results then

                #Check if SCT run has already completed
                if  exist FS%i:\acs_results_template\acs_results\sct_results\Overall\Summary.log then
                    echo "SCT has completed run."
                    echo "Press any key to start SCT execution from the beginning."
                    echo "WARNING: Ensure you have backed up the existing logs."
                    stallforkey.efi 5
                    if %lasterror% == 0 then
                        #Backup the existing logs
                        rm -q FS%i:\acs_results_template\acs_results\sct_results_previous_run
                        mkdir FS%i:\acs_results_template\acs_results\sct_results_previous_run
                        cp -r FS%i:\acs_results_template\acs_results\sct_results FS%i:\acs_results_template\acs_results\sct_results_previous_run
                        rm -q FS%i:\acs_results_template\acs_results\sct_results
                        goto StartSCT
                    else
                        goto Done
                    endif
                endif

                if exist FS%i:\acs_tests\bbr\SCT\.passive.mode then
                    if exist FS%i:\acs_tests\bbr\SCT\.verbose.mode then
                        Sct -c -p mnp -v
                    else
                        Sct -c -p mnp
                    endif
                    else
                    if exist FS%i:\acs_tests\bbr\SCT\.verbose.mode then
                        Sct -c -v
                    else
                        Sct -c
                    endif

                    #SCT execution has finished. Copy the logs to acs_results
                    if  exist FS%i:\acs_results_template\acs_results\sct_results\ then
                        if  exist FS%i:\acs_tests\bbr\SCT\Overall then
                            cp -r FS%i:\acs_tests\bbr\SCT\Overall FS%i:\acs_results_template\acs_results\sct_results\
                        endif
                        if  exist FS%i:\acs_tests\bbr\SCT\Dependency\EfiCompliantBBTest then
                            cp -r FS%i:\acs_tests\bbr\SCT\Dependency\EfiCompliantBBTest FS%i:\acs_results_template\acs_results\sct_results\
                        endif
                        if  exist FS%i:\acs_tests\bbr\SCT\Sequence then
                            cp -r FS%i:\acs_tests\bbr\SCT\Sequence FS%i:\acs_results_template\acs_results\sct_results\
                        endif

                        #Restart to avoid an impact of running SCT tests on rest of the suites
                        echo "Reset the system ..."
                        reset
                    endif
                endif
            else
:StartSCT
                FS%i:
                cd FS%i:\acs_results_template\acs_results
                mkdir sct_results
                cd FS%i:\acs_tests\bbr\SCT
                if %1 == sct_extd then
                    echo "Starting extended run of SCT"
                    Sct -s EBBR_extd_run.seq
                else
                    Sct -s EBBR.seq
                endif
            endif
        endif
    endif
endfor

:Done

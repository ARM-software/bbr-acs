#!/usr/bin/env bash

# Copyright (c) 2025, Arm Limited or its affiliates. All rights reserved.
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

for %i in 0 1 2 3 4 5 6 7 8 9 A B C D E F then
    if exist FS%i:\SCT then
        #
        # Found EFI SCT harness
        #
        FS%i:
        cd FS%i:\SCT
        echo "Press any key to stop the EFI SCT running"
        stallforkey.efi 5
        if %lasterror% == 0 then
            goto Done
        endif
        if exists FS%i:\sct_results then
            #Check if SCT run has already completed
            if  exist FS%i:\sct_results\Overall\Summary.log then
                echo "SCT has completed run."
                echo "Press any key to start SCT execution from the beginning."
                echo "WARNING: Ensure you have backed up the existing logs."
                stallforkey.efi 5
                if %lasterror% == 0 then
                    #Backup the existing logs
                    rm -q FS%i:\sct_results_previous_run
                    mkdir FS%i:\sct_results_previous_run
                    cp -r FS%i:\sct_results FS%i:\sct_results_previous_run
                    rm -q FS%i:\sct_results
                    goto StartSCT
                else
                    goto Done
                endif
            endif

            Sct -c

            #SCT execution has finished. Copy the logs to acs_results
            if  exist FS%i:\sct_results\ then
                if  exist FS%i:\SCT\Overall then
                    cp -r FS%i:\SCT\Overall FS%i:\sct_results\
                endif
                if  exist FS%i:\SCT\Dependency\EfiCompliantBBTest then
                    cp -r FS%i:\SCT\Dependency\EfiCompliantBBTest FS%i:\sct_results\
                endif
                if  exist FS%i:\SCT\Sequence then
                    cp -r FS%i:\SCT\Sequence FS%i:\sct_results\
                endif

                #Restart to avoid an impact of running SCT tests on rest of the suites
                echo "Reset the system ..."
                reset
            endif
        else
:StartSCT
            FS%i:
            mkdir FS%i:\sct_results
            cd FS%i:\SCT
            if %1 == sbbr then
                Sct -s SBBR.seq
            else if %1 == ebbr then
                Sct -s EBBR.seq
            else
                echo "Usage: set bbr_recipe=sbbr/ebbr in startup.nsh "
            endif
        endif
    endif
endfor

:Done

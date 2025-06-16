#!/usr/bin/env bash

# Copyright (c) 2021, 2024, Arm Limited or its affiliates. All rights reserved.
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

TOP_DIR=`pwd`

create_scripts_link()
{
 ln -s $TOP_DIR/../../common/scripts/framework.sh             $TOP_DIR/build-scripts/framework.sh
 ln -s $TOP_DIR/../../common/scripts/parse_params.sh          $TOP_DIR/build-scripts/parse_params.sh
 ln -s $TOP_DIR/../../common/scripts/build-sct.sh             $TOP_DIR/build-scripts/build-sct.sh
 ln -s $TOP_DIR/../../common/scripts/build-uefi-apps.sh       $TOP_DIR/build-scripts/build-uefi-apps.sh
}

init_dir()
{
 mkdir -p $TOP_DIR/build-scripts/config
 cp -r $TOP_DIR/../config/*                                 $TOP_DIR/build-scripts/config/
}

create_scripts_link
init_dir

source ./build-scripts/build-sct.sh  EBBR S
source ./build-scripts/build-uefi-apps.sh  EBBR S

TOP_DIR=`pwd`

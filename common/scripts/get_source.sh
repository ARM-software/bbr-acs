#!/usr/bin/env bash

#  Copyright (c) 2021-2026,  Arm Limited or its affiliates. All rights reserved.
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

. $TOP_DIR/../../common/config/bbr_source.cfg

get_cross_compiler()
{
    if [ $(uname -m) == "aarch64" ]; then
        echo "=================================================================="
        echo "aarch64 native build"
        echo "WARNING: no cross compiler needed, GCC version recommended: ${GCC_TOOLS_VERSION}"
        echo "=================================================================="
    else
        echo "Downloading cross compiler. Version : ${GCC_TOOLS_VERSION}"
        TAG=aarch64-none-linux-gnu
        mkdir -p tools
        pushd $TOP_DIR/tools
        wget $CROSS_COMPILER_URL --no-check-certificate
        tar -xf arm-gnu-toolchain-${GCC_TOOLS_VERSION}-x86_64-${TAG}.tar.xz
        rm arm-gnu-toolchain-${GCC_TOOLS_VERSION}-x86_64-${TAG}.tar.xz
        popd
    fi
}

get_sct_src()
{
    git clone --single-branch https://github.com/tianocore/edk2-test
    pushd $TOP_DIR/edk2-test
    git checkout $SCT_SRC_TAG
    popd
}

get_uefi_src()
{
    git clone --depth 1 --single-branch \
    --branch $EDK2_SRC_VERSION https://github.com/tianocore/edk2.git
    pushd $TOP_DIR/edk2
    git submodule update --init
    popd
}

sudo apt install git curl mtools gdisk gcc\
 openssl automake autotools-dev libtool bison flex\
 bc uuid-dev python3 libglib2.0-dev libssl-dev autopoint libbsd-dev\
 autoconf

get_uefi_src
get_sct_src
get_cross_compiler

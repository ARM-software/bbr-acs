#!/usr/bin/env bash

# Copyright (c) 2021, 2023, ARM Limited and Contributors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# Neither the name of ARM nor the names of its contributors may be used
# to endorse or promote products derived from this software without specific
# prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

TOP_DIR=`pwd`

. $TOP_DIR/../../common/config/bbr_common_config.cfg

#Optional argument 'arm' shall be set when targeting a 32bit Arm device
if [ "$1" == "arm" ]; then
    TARGET_ARCH="arm"
else
    TARGET_ARCH="aarch64"
fi

get_cross_compiler()
{
    if [ $(uname -m) == "aarch64" ]; then
        echo "=================================================================="
        echo "aarch64 native build"
        echo "WARNING: no cross compiler needed, GCC version recommended: ${GCC_TOOLS_VERSION}"
        echo "=================================================================="
    else
        echo "Downloading cross compiler. Version : ${GCC_TOOLS_VERSION}"
    if [ $TARGET_ARCH == "arm" ]; then
        TAG=arm-linux-gnueabihf
    else
        TAG=aarch64-none-linux-gnu
    fi
        mkdir -p tools
        pushd $TOP_DIR/tools
        wget $CROSS_COMPILER_URL
        tar -xf gcc-arm-${GCC_TOOLS_VERSION}-x86_64-${TAG}.tar.xz
        rm gcc-arm-${GCC_TOOLS_VERSION}-x86_64-${TAG}.tar.xz
        popd
    fi
}

get_fwts_src()
{
    git clone --single-branch https://github.com/fwts/fwts
    pushd $TOP_DIR/fwts
    git checkout $FWTS_SRC_TAG
    git submodule update --init
    popd
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
get_fwts_src

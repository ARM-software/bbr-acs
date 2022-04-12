#!/usr/bin/env bash

# Copyright (c) 2021, ARM Limited and Contributors. All rights reserved.
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

#
# This script uses the following environment variables from the variant
#
# VARIANT - build variant name
# TOP_DIR - workspace root directory
# CROSS_COMPILE - PATH to GCC including CROSS-COMPILE prefix
# PARALLELISM - number of cores to build across
# UEFI_BUILD_ENABLED - Flag to enable building UEFI
# UEFI_PATH - sub-directory containing UEFI code
# UEFI_BUILD_MODE - DEBUG or RELEASE
# UEFI_TOOLCHAIN - Toolchain supported by Linaro uefi-tools: GCC49, GCC48 or GCC47
# UEFI_PLATFORMS - List of platforms to build
# UEFI_PLAT_{platform name} - array of platform parameters:
#     - platname - the name of the platform used by the build
#     - makefile - the makefile to execute for this platform
#     - output - where to store the files in packaging phase
#     - defines - extra platform defines during the build
#     - binary - what to call the final output binary

TOP_DIR=`pwd`
UEFI_PATH=edk2
UEFI_TOOLCHAIN=GCC5
UEFI_BUILD_MODE=DEBUG
TARGET_ARCH=AARCH64

# toolchain
. $TOP_DIR/../../common/scripts/common_cross_toolchain.sh

BUILD_PLAT=$1
BUILD_TYPE=$2

if [ $BUILD_PLAT = SR ]; then
   BUILD_PLAT=ES
fi

if [[ ! ${ARCH+x} ]]; then
    # use AARCH64
    TARGET_ARCH=AARCH64
else
    case $ARCH in
    arm)
        TARGET_ARCH=ARM
        ;;
    *)
        TARGET_ARCH=AARCH64
        ;;
    esac
fi

#Currently the BUILD_PLAT flag is not used. For future use
if ! [[ $BUILD_PLAT = IR ]] && ! [[ $BUILD_PLAT = ES ]] ; then
    echo "Please provide a target."
    echo "Usage $0 <IR/ES> <BUILD_TYPE>"
    echo "S->Standalone BBR,F->Full systemready"
    exit
fi

if ! [[ $BUILD_TYPE = S ]] && ! [[  $BUILD_TYPE = F  ]] ; then
    echo "Please provide a Build type."
    echo "Usage $0 <target> <S/F>"
    echo "S->Standalone BBR,F->Full systemready"
    exit
fi

echo "Target: $BUILD_PLAT"
echo "Build type: $BUILD_TYPE"


if [[ $BUILD_TYPE = S ]]; then
    BBR_DIR=$TOP_DIR/../../
else
    BBR_DIR=$TOP_DIR/bbr-acs
fi

do_build()
{

    pushd $TOP_DIR/$UEFI_PATH
    CROSS_COMPILE_DIR=$(dirname $CROSS_COMPILE)
    PATH="$PATH:$CROSS_COMPILE_DIR"

    export EDK2_TOOLCHAIN=$UEFI_TOOLCHAIN
    if [[ ! ${ARCH+x} ]]; then
            # use ARCH=arm64
            CROSS_COMPILE_DIR=$(dirname $CROSS_COMPILE)
            PATH="$PATH:$CROSS_COMPILE_DIR"
            export ${UEFI_TOOLCHAIN}_AARCH64_PREFIX=$CROSS_COMPILE
            ARCH_TARGET_FOR_UEFI=AARCH64
        else
            case $ARCH in
            arm)
                export ${UEFI_TOOLCHAIN}_ARM_PREFIX=$CROSS_COMPILE
                ARCH_TARGET_FOR_UEFI=ARM
                ;;
            *)
                export ${UEFI_TOOLCHAIN}_AARCH64_PREFIX=$CROSS_COMPILE
                ARCH_TARGET_FOR_UEFI=AARCH64
                ;;
            esac
        fi

    local vars=
    export PACKAGES_PATH=$TOP_DIR/$UEFI_PATH
    export PYTHON_COMMAND=/usr/bin/python3
    export WORKSPACE=$TOP_DIR/$UEFI_PATH

    #Build base tools
    source $TOP_DIR/$UEFI_PATH/edksetup.sh
    make -C $TOP_DIR/$UEFI_PATH/BaseTools

    build -a $ARCH_TARGET_FOR_UEFI -t GCC5 -p MdeModulePkg/MdeModulePkg.dsc
    popd
}

do_clean()
{
    pushd $TOP_DIR/$UEFI_PATH
    CROSS_COMPILE_DIR=$(dirname $CROSS_COMPILE)
    PATH="$PATH:$CROSS_COMPILE_DIR"
    source $TOP_DIR/$UEFI_PATH/edksetup.sh
    make -C $TOP_DIR/$UEFI_PATH/BaseTools clean

    popd

}

do_package ()
{
    echo "Packaging...";

    if [ -f $TOP_DIR/$UEFI_PATH/Build/MdeModule/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/${TARGET_ARCH}/CapsuleApp.efi ]; then
     echo "CapsuleApp.efi successfully generated at $TOP_DIR/$UEFI_PATH/Build/MdeModule/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/${TARGET_ARCH}/CapsuleApp.efi"
    else
     echo "Error: CapsuleApp.efi could not be generated. Please check the logs"
    fi

}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/framework.sh $@


#!/usr/bin/env bash

# Copyright (c) 2021, 2023-2024, Arm Limited or its affiliates. All rights reserved.
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
arch=$(uname -m)
UEFI_PATH=edk2
UEFI_TOOLCHAIN=GCC5
UEFI_BUILD_MODE=DEBUG
TARGET_ARCH=AARCH64
KEYS_DIR=$TOP_DIR/bbsr-keys

BUILD_PLAT=$1
BUILD_TYPE=$2

# if BBR standalone build
if [[ $BUILD_TYPE = S ]]; then
    . $TOP_DIR/../../common/config/bbr_source.cfg
else
    # source common config if arm-systemready ACS build
. $TOP_DIR/../common/config/systemready-band-source.cfg
fi

if [[ $arch != "aarch64" ]]; then
    CROSS_COMPILE=$TOP_DIR/$GCC
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
    if [[ $arch != "aarch64" ]]; then
        CROSS_COMPILE_DIR=$(dirname $CROSS_COMPILE)
        PATH="$PATH:$CROSS_COMPILE_DIR"
        export ${UEFI_TOOLCHAIN}_AARCH64_PREFIX=$CROSS_COMPILE
    fi

    export EDK2_TOOLCHAIN=$UEFI_TOOLCHAIN
    export PACKAGES_PATH=$TOP_DIR/$UEFI_PATH
    export PYTHON_COMMAND=/usr/bin/python3
    export WORKSPACE=$TOP_DIR/$UEFI_PATH

    #Build base tools
    source $TOP_DIR/$UEFI_PATH/edksetup.sh
    make -C $TOP_DIR/$UEFI_PATH/BaseTools

    build -a AARCH64 -t GCC5 -p MdeModulePkg/MdeModulePkg.dsc
    if [[ $BUILD_TYPE = S ]]; then
        # Shell.efi is required to run the standalone SCT. Copy it into the SBBR/EBBR-SCT
        # package and place it as EFI/BOOT/bootaa64.efi for UEFI boot.
        build -a AARCH64 -t GCC5 -p ShellPkg/ShellPkg.dsc
    fi
    popd
}

do_clean()
{
    pushd $TOP_DIR/$UEFI_PATH
    if [[ $arch != "aarch64" ]]; then
        CROSS_COMPILE_DIR=$(dirname $CROSS_COMPILE)
        PATH="$PATH:$CROSS_COMPILE_DIR"
    fi
    source $TOP_DIR/$UEFI_PATH/edksetup.sh
    make -C $TOP_DIR/$UEFI_PATH/BaseTools clean
    popd
}

do_package ()
{
    echo "Packaging CapsuleApp...";

    if [ $BUILD_TYPE = F ]; then
        sbsign --key $KEYS_DIR/TestDB1.key --cert $KEYS_DIR/TestDB1.crt $TOP_DIR/$UEFI_PATH/Build/MdeModule/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/${TARGET_ARCH}/CapsuleApp.efi --output $TOP_DIR/$UEFI_PATH/Build/MdeModule/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/${TARGET_ARCH}/CapsuleApp.efi
    fi

    if [ -f $TOP_DIR/$UEFI_PATH/Build/MdeModule/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/${TARGET_ARCH}/CapsuleApp.efi ]; then
     echo "CapsuleApp.efi successfully generated at $TOP_DIR/$UEFI_PATH/Build/MdeModule/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/${TARGET_ARCH}/CapsuleApp.efi"
    else
     echo "Error: CapsuleApp.efi could not be generated. Please check the logs"
    fi

    if [ $BUILD_TYPE = S ]; then
     # Shell.efi is required to run the standalone SCT. Copy it into the SBBR/EBBR-SCT
     # package and place it as EFI/BOOT/bootaa64.efi for UEFI boot.
     cp $TOP_DIR/$UEFI_PATH/Build/Shell/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/${TARGET_ARCH}/ShellPkg/Application/Shell/Shell/$UEFI_BUILD_MODE/Shell.efi $TOP_DIR/edk2-test/uefi-sct/${BUILD_PLAT}-SCT/EFI/BOOT/bootaa64.efi
    fi
}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/framework.sh $@

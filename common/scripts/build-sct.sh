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
SCT_PATH=edk2-test
UEFI_TOOLCHAIN=GCC5
UEFI_BUILD_MODE=DEBUG
TARGET_ARCH=AARCH64
GCC=tools/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
CROSS_COMPILE=$TOP_DIR/$GCC

BBR=$1
BUILD_TYPE=$2

if ! [[ $BBR = IR ]] && ! [[ $BBR = ES ]] && ! [[ $BBR = SE ]] ; then
    echo "Please provide a target."
    echo "Usage build-sct.sh <IR/ES/SE> <BUILD_TYPE>"
    exit
fi

if ! [[ $BUILD_TYPE = S ]] && ! [[  $BUILD_TYPE = F  ]] ; then
    echo "Please provide a Build type."
    echo "Usage build-sct.sh <target> <S/F>"
    echo "S->Standalone BBR,F->Full systemready"
    exit
fi

if [[ $BUILD_TYPE = S ]]; then
    BBR_DIR=$TOP_DIR/../../
else
    BBR_DIR=$TOP_DIR/bbr-acs
fi

echo "Target: $BBR"
echo "Build type: $BUILD_TYPE"

SBBR_TEST_DIR=$BBR_DIR/common/sct-tests/sbbr-tests

do_build()
{
   
    pushd $TOP_DIR/$SCT_PATH
    CROSS_COMPILE_DIR=$(dirname $CROSS_COMPILE)
    export PATH="$TOP_DIR/efitools:$PATH:$CROSS_COMPILE_DIR"

    export EDK2_TOOLCHAIN=$UEFI_TOOLCHAIN
    export ${UEFI_TOOLCHAIN}_AARCH64_PREFIX=$CROSS_COMPILE
    local vars=
    export PACKAGES_PATH=$TOP_DIR/$UEFI_PATH
    export PYTHON_COMMAND=/usr/bin/python3
    export WORKSPACE=$TOP_DIR/$SCT_PATH/uefi-sct
    export KEYS_DIR=$TOP_DIR/security-extension-acs-keys
    #export HOST_ARCH = `uname -m`
    #MACHINE=`uname -m`

    #Build base tools
    source $TOP_DIR/$UEFI_PATH/edksetup.sh
    make -C $TOP_DIR/$UEFI_PATH/BaseTools
    
    #Copy over extra files needed for SBBR tests
    if [[ $BBR != SE ]] ; then
        cp -r $SBBR_TEST_DIR/SbbrBootServices uefi-sct/SctPkg/TestCase/UEFI/EFI/BootServices/
        cp -r $SBBR_TEST_DIR/SbbrEfiSpecVerLvl $SBBR_TEST_DIR/SbbrRequiredUefiProtocols $SBBR_TEST_DIR/SbbrSmbios $SBBR_TEST_DIR/SbbrSysEnvConfig uefi-sct/SctPkg/TestCase/UEFI/EFI/Generic/
        cp -r $SBBR_TEST_DIR/SBBRRuntimeServices uefi-sct/SctPkg/TestCase/UEFI/EFI/RuntimeServices/
        cp $SBBR_TEST_DIR/BBR_SCT.dsc uefi-sct/SctPkg/UEFI/
        cp $SBBR_TEST_DIR/build_bbr.sh uefi-sct/SctPkg/
    fi
    
    #Startup/runtime files.
    mkdir -p uefi-sct/SctPkg/BBR
    if [ $BBR = IR ]; then
    #EBBR
    cp $BBR_DIR/ebbr/config/EBBRStartup.nsh uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/ebbr/config/EBBR.seq uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/ebbr/config/EfiCompliant_EBBR.ini uefi-sct/SctPkg/BBR/
    elif [ $BBR = ES ]; then
    #SBBR
    cp $BBR_DIR/sbbr/config/SBBRStartup.nsh uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/sbbr/config/SBBR.seq uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/sbbr/config/EfiCompliant_SBBR.ini  uefi-sct/SctPkg/BBR/
    fi

    if [[ $BBR != SE ]] ; then
        if ! patch -R -p1 -s -f --dry-run < $BBR_DIR/common/patches/edk2-test-bbr.patch; then
            echo "Applying SCT patch ..."
            patch  -p1  < $BBR_DIR/common/patches/edk2-test-bbr.patch
        fi
    fi

    pushd uefi-sct
    if [[ $BBR = SE ]] ; then
        ./SctPkg/build.sh $TARGET_ARCH GCC $UEFI_BUILD_MODE
    else
        ./SctPkg/build_bbr.sh $TARGET_ARCH GCC $UEFI_BUILD_MODE
    fi
    
    popd
}

do_clean()
{
    pushd $TOP_DIR/$SCT_PATH/uefi-sct
    CROSS_COMPILE_DIR=$(dirname $CROSS_COMPILE)
    PATH="$PATH:$CROSS_COMPILE_DIR"
    source $TOP_DIR/$UEFI_PATH/edksetup.sh
    make -C $TOP_DIR/$UEFI_PATH/BaseTools clean
    rm -rf Build/bbrSct
    rm -rf ${TARGET_ARCH}_SCT

    popd

}

do_package ()
{
    echo "Packaging sct... $VARIANT";
    # Copy binaries to output folder
    pushd $TOP_DIR/$SCT_PATH/uefi-sct

    mkdir -p ${TARGET_ARCH}_SCT/SCT

    if [ $BBR = IR ]; then
        #EBBR
        cp -r Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/${TARGET_ARCH}/* ${TARGET_ARCH}_SCT/SCT/
        cp Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/EBBRStartup.nsh ${TARGET_ARCH}_SCT/SctStartup.nsh
        cp SctPkg/BBR/EfiCompliant_EBBR.ini ${TARGET_ARCH}_SCT/SCT/Dependency/EfiCompliantBBTest/EfiCompliant.ini
        #rm ${TARGET_ARCH}_SCT/SCT/Sequence/SBBR.seq

    elif [ $BBR = ES ]; then
        #SBBR
        cp -r Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/${TARGET_ARCH}/* ${TARGET_ARCH}_SCT/SCT/
        cp Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/SBBRStartup.nsh ${TARGET_ARCH}_SCT/SctStartup.nsh
        cp SctPkg/BBR/EfiCompliant_SBBR.ini ${TARGET_ARCH}_SCT/SCT/Dependency/EfiCompliantBBTest/EfiCompliant.ini
        #rm ${TARGET_ARCH}_SCT/SCT/Sequence/EBBR.seq
    elif [ $BBR = SE ]; then
        cp -r Build/UefiSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/${TARGET_ARCH}/* ${TARGET_ARCH}_SCT/SCT/
        cp $BBR_DIR/bbsr/config/BBSRStartup.nsh ${TARGET_ARCH}_SCT/SctStartup.nsh
        cp $BBR_DIR/bbsr/config/BBSR.seq  ${TARGET_ARCH}_SCT/SCT/Sequence
    else # error
        echo "Error: unexpected platform type"
        exit
    fi

    pushd $TOP_DIR

}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/framework.sh $@


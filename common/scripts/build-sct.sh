#!/usr/bin/env bash

# Copyright (c) 2021-2023, ARM Limited and Contributors. All rights reserved.
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
arch=$(uname -m)
UEFI_PATH=edk2
SCT_PATH=edk2-test
UEFI_TOOLCHAIN=GCC5
UEFI_BUILD_MODE=DEBUG
TARGET_ARCH=AARCH64
KEYS_DIR=$TOP_DIR/security-interface-extension-keys
TEST_DB1_KEY=$KEYS_DIR/TestDB1.key
TEST_DB1_CRT=$KEYS_DIR/TestDB1.crt
SCT_FRAMEWORK=$TOP_DIR/$SCT_PATH/uefi-sct/Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/${TARGET_ARCH}

BUILD_PLAT=$1
BUILD_TYPE=$2

# if BBR standalone build
if [[ $BUILD_TYPE = S ]]; then
    . $TOP_DIR/../../common/config/bbr_common_config.cfg
else
    # source common config if arm-systemready ACS build
    . $TOP_DIR/../../common/config/sr_es_common_config.cfg
fi

if [[ $arch != "aarch64" ]]; then
    export CROSS_COMPILE_64=$TOP_DIR/$GCC
fi


if [ $BUILD_PLAT = SR ]; then
   BUILD_PLAT=ES
fi

if ! [[ $BUILD_PLAT = IR ]] && ! [[ $BUILD_PLAT = ES ]]  ; then
    echo "Please provide a target."
    echo "Usage build-sct.sh <IR/ES> <BUILD_TYPE>"
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

echo "Target: $BUILD_PLAT"
echo "Build type: $BUILD_TYPE"

SBBR_TEST_DIR=$BBR_DIR/common/sct-tests/sbbr-tests
BBSR_TEST_DIR=$BBR_DIR/bbsr/sct-tests
if [[ $BUILD_TYPE = S ]]; then
    sed -i 's|SctPkg/TestCase/UEFI/EFI/RuntimeServices/SecureBoot/BlackBoxTest/SecureBootBBTest.inf|#SctPkg/TestCase/UEFI/EFI/RuntimeServices/SecureBoot/BlackBoxTest/SecureBootBBTest.inf|g' $BBR_DIR/common/sct-tests/sbbr-tests/BBR_SCT.dsc
    sed -i 's|SctPkg/TestCase/UEFI/EFI/RuntimeServices/BBSRVariableSizeTest/BlackBoxTest/BBSRVariableSizeBBTest.inf|#SctPkg/TestCase/UEFI/EFI/RuntimeServices/BBSRVariableSizeTest/BlackBoxTest/BBSRVariableSizeBBTest.inf|g' $BBR_DIR/common/sct-tests/sbbr-tests/BBR_SCT.dsc
    sed -i 's|SctPkg/TestCase/UEFI/EFI/Protocol/TCG2Protocol/BlackBoxTest/TCG2ProtocolBBTest.inf|#SctPkg/TestCase/UEFI/EFI/Protocol/TCG2Protocol/BlackBoxTest/TCG2ProtocolBBTest.inf|g' $BBR_DIR/common/sct-tests/sbbr-tests/BBR_SCT.dsc
    sed -i 's|SctPkg/TestCase/UEFI/EFI/RuntimeServices/SecureBoot/BlackBoxTest/Dependency/Images/Images.inf|#SctPkg/TestCase/UEFI/EFI/RuntimeServices/SecureBoot/BlackBoxTest/Dependency/Images/Images.inf|g' $BBR_DIR/common/sct-tests/sbbr-tests/BBR_SCT.dsc
fi

do_build()
{
    pushd $TOP_DIR/$SCT_PATH
    export KEYS_DIR=$TOP_DIR/security-interface-extension-keys
    export EDK2_TOOLCHAIN=$UEFI_TOOLCHAIN

    # required for SIE keys generation
    export PATH="$PATH:$TOP_DIR/efitools"

    # export EDK2 enviromnent variables
    export PACKAGES_PATH=$TOP_DIR/$UEFI_PATH
    export PYTHON_COMMAND=/usr/bin/python3
    export WORKSPACE=$TOP_DIR/$SCT_PATH/uefi-sct
    #export HOST_ARCH = `uname -m`
    #MACHINE=`uname -m`
    #Build base tools
    source $TOP_DIR/$UEFI_PATH/edksetup.sh
    make -C $TOP_DIR/$UEFI_PATH/BaseTools
    #Copy over extra files needed for SBBR tests
    cp -r $SBBR_TEST_DIR/SbbrBootServices uefi-sct/SctPkg/TestCase/UEFI/EFI/BootServices/
    cp -r $SBBR_TEST_DIR/SbbrEfiSpecVerLvl $SBBR_TEST_DIR/SbbrRequiredUefiProtocols $SBBR_TEST_DIR/SbbrSysEnvConfig uefi-sct/SctPkg/TestCase/UEFI/EFI/Generic/
    cp -r $SBBR_TEST_DIR/SBBRRuntimeServices uefi-sct/SctPkg/TestCase/UEFI/EFI/RuntimeServices/
    cp $SBBR_TEST_DIR/BBR_SCT.dsc uefi-sct/SctPkg/UEFI/
    cp $SBBR_TEST_DIR/build_bbr.sh uefi-sct/SctPkg/

    # copy SIE SCT tests to edk2-test
    if [[ $BUILD_TYPE != S ]]; then
        cp -r $BBSR_TEST_DIR/BBSRVariableSizeTest uefi-sct/SctPkg/TestCase/UEFI/EFI/RuntimeServices
        cp -r $BBSR_TEST_DIR/SecureBoot uefi-sct/SctPkg/TestCase/UEFI/EFI/RuntimeServices
        cp -r $BBSR_TEST_DIR/TCG2Protocol uefi-sct/SctPkg/TestCase/UEFI/EFI/Protocol
        cp -r $BBSR_TEST_DIR/TCG2.h uefi-sct/SctPkg/UEFI/Protocol
    fi

    #Startup/runtime files.
    mkdir -p uefi-sct/SctPkg/BBR
    if [ $BUILD_PLAT = IR ]; then
    #EBBR
    cp $BBR_DIR/ebbr/config/EBBRStartup.nsh uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/ebbr/config/EBBR.seq uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/ebbr/config/EBBR_manual.seq uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/ebbr/config/EBBR_extd_run.seq uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/ebbr/config/EfiCompliant_EBBR.ini uefi-sct/SctPkg/BBR/
    elif [ $BUILD_PLAT = ES ]; then
    #SBBR
    cp $BBR_DIR/sbbr/config/SBBRStartup.nsh uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/sbbr/config/SBBR.seq uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/sbbr/config/SBBR_manual.seq uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/sbbr/config/SBBR_extd_run.seq uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/sbbr/config/EfiCompliant_SBBR.ini  uefi-sct/SctPkg/BBR/
    fi

    #Common
    #SCRT
    cp $BBR_DIR/common/config/ScrtStartup.nsh uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/common/config/SCRT.conf uefi-sct/SctPkg/BBR/

    if git apply --check $BBR_DIR/common/patches/edk2-test-bbr-build.patch; then
        echo "Applying edk2-test BBR build patch..."
        git apply --ignore-whitespace --ignore-space-change $BBR_DIR/common/patches/edk2-test-bbr-build.patch
    else
        echo  "Error while applying edk2-test BBR build patch..."
    fi
    if git apply --check $BBR_DIR/common/patches/edk2-test-bbr.patch; then
        echo "Applying edk2-test BBR patch..."
        git apply --ignore-whitespace --ignore-space-change $BBR_DIR/common/patches/edk2-test-bbr.patch
    else
        echo  "Error while applying edk2-test BBR patch..."
    fi
    if [[ $BUILD_TYPE != S ]]; then
        if git apply --check $BBR_DIR/bbsr/patches/0001-SIE-Patch-for-UEFI-SCT-Build.patch; then
            echo "Applying SIE SCT patch..."
            git apply --ignore-whitespace --ignore-space-change $BBR_DIR/bbsr/patches/0001-SIE-Patch-for-UEFI-SCT-Build.patch
        else
            echo  "Error while applying SIE SCT patch..."
        fi
    fi

    pushd uefi-sct
    ./SctPkg/build_bbr.sh $TARGET_ARCH GCC $UEFI_BUILD_MODE  -n $PARALLELISM
    popd
}

do_clean()
{
    pushd $TOP_DIR/$SCT_PATH/uefi-sct
    source $TOP_DIR/$UEFI_PATH/edksetup.sh
    make -C $TOP_DIR/$UEFI_PATH/BaseTools clean
    rm -rf Build
    rm -rf ${TARGET_ARCH}_SCT

    popd

}

# sign SCT efi files
SecureBootSign() {
    echo "KEYS_DIR = $KEYS_DIR"

    for f in $1/*.efi
    do
        echo "sbsign --key $TEST_DB1_KEY --cert $TEST_DB1_CRT $f --output $f"
        sbsign --key $TEST_DB1_KEY --cert $TEST_DB1_CRT $f --output $f
    done
}

# signing SCT test dependency files
SecureBootSignDependency() {
    echo "KEYS_DIR = $KEYS_DIR"

    for f in $SCT_FRAMEWORK/Dependency/$1BBTest/*.efi
    do
        echo "sbsign --key $TEST_DB1_KEY --cert $TEST_DB1_CRT $f --output $f"
        sbsign --key $TEST_DB1_KEY --cert $TEST_DB1_CRT $f --output $f
    done
}

do_package ()
{
    echo "Packaging sct... $VARIANT";
    # Copy binaries to output folder
    pushd $TOP_DIR/$SCT_PATH/uefi-sct

    mkdir -p ${TARGET_ARCH}_SCT/SCT
    mkdir -p ${TARGET_ARCH}_SCT/SCT/SCRT
    mkdir -p ${TARGET_ARCH}_SCT/SCT/Sequence

    if [ $BUILD_PLAT = IR ]; then
        #EBBR
        cp -r Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/${TARGET_ARCH}/* ${TARGET_ARCH}_SCT/SCT/
        cp Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/EBBRStartup.nsh ${TARGET_ARCH}_SCT/SctStartup.nsh
        cp SctPkg/BBR/EfiCompliant_EBBR.ini ${TARGET_ARCH}_SCT/SCT/Dependency/EfiCompliantBBTest/EfiCompliant.ini
        cp SctPkg/BBR/EBBR_manual.seq ${TARGET_ARCH}_SCT/SCT/Sequence/EBBR_manual.seq

    elif [ $BUILD_PLAT = ES ]; then
        # Sign the SCT binaries
        if [ $BUILD_TYPE != S ]; then
            SecureBootSign $SCT_FRAMEWORK
            SecureBootSign $SCT_FRAMEWORK/Support
            SecureBootSign $TOP_DIR/$SCT_PATH/uefi-sct/Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}
            SecureBootSign $SCT_FRAMEWORK/SCRT
            SecureBootSign $SCT_FRAMEWORK/Test
            SecureBootSign $SCT_FRAMEWORK/Ents/Support
            SecureBootSign $SCT_FRAMEWORK/Ents/Test
            SecureBootSignDependency LoadedImage
            SecureBootSignDependency ImageServices
            SecureBootSignDependency ProtocolHandlerServices
            SecureBootSignDependency ConfigKeywordHandler
            SecureBootSignDependency PciIo
            #BBSR
            cp $BBR_DIR/bbsr/config/sie_SctStartup.nsh ${TARGET_ARCH}_SCT/sie_SctStartup.nsh
            cp $BBR_DIR/bbsr/config/BBSR.seq  ${TARGET_ARCH}_SCT/SCT/Sequence/
        fi
        #SBBR
        cp -r Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/${TARGET_ARCH}/* ${TARGET_ARCH}_SCT/SCT/
        cp Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/SBBRStartup.nsh ${TARGET_ARCH}_SCT/SctStartup.nsh
        cp SctPkg/BBR/EfiCompliant_SBBR.ini ${TARGET_ARCH}_SCT/SCT/Dependency/EfiCompliantBBTest/EfiCompliant.ini
        cp SctPkg/BBR/SBBR_manual.seq ${TARGET_ARCH}_SCT/SCT/Sequence/SBBR_manual.seq
        cp SctPkg/BBR/SBBR_extd_run.seq ${TARGET_ARCH}_SCT/SCT/Sequence/SBBR_extd_run.seq
    else
         echo "Error: unexpected platform type"
         exit
    fi

    #Common
    #SCRT
    cp SctPkg/BBR/ScrtStartup.nsh ${TARGET_ARCH}_SCT/ScrtStartup.nsh
    cp SctPkg/BBR/SCRT.conf ${TARGET_ARCH}_SCT/SCT/SCRT/SCRT.conf

    pushd $TOP_DIR

}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/framework.sh $@


#!/usr/bin/env bash

#  Copyright (c) 2021-2024, Arm Limited or its affiliates. All rights reserved.
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
SCT_PATH=edk2-test
UEFI_TOOLCHAIN=GCC5
UEFI_BUILD_MODE=DEBUG
TARGET_ARCH=AARCH64
KEYS_DIR=$TOP_DIR/bbsr-keys
TEST_DB1_KEY=$KEYS_DIR/TestDB1.key
TEST_DB1_CRT=$KEYS_DIR/TestDB1.crt
SCT_FRAMEWORK=$TOP_DIR/$SCT_PATH/uefi-sct/Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/${TARGET_ARCH}

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
    export CROSS_COMPILE_64=$TOP_DIR/$GCC
fi


if ! [[ $BUILD_PLAT = EBBR ]] && ! [[ $BUILD_PLAT = SBBR ]]  ; then
    echo "Please provide a target."
    echo "Usage build-sct.sh <EBBR/SBBR> <BUILD_TYPE>"
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
    sed -i 's|SctPkg/TestCase/UEFI/EFI/Protocol/TCG2/BlackBoxTest/TCG2ProtocolBBTest.inf|#SctPkg/TestCase/UEFI/EFI/Protocol/TCG2/BlackBoxTest/TCG2ProtocolBBTest.inf|g' $BBR_DIR/common/sct-tests/sbbr-tests/BBR_SCT.dsc
    sed -i 's|SctPkg/TestCase/UEFI/EFI/RuntimeServices/TCGMemoryOverwriteRequest/BlackBoxTest/TCGMemoryOverwriteRequestBBTest.inf|#SctPkg/TestCase/UEFI/EFI/RuntimeServices/TCGMemoryOverwriteRequest/BlackBoxTest/TCGMemoryOverwriteRequestBBTest.inf|g' $BBR_DIR/common/sct-tests/sbbr-tests/BBR_SCT.dsc
    sed -i 's|SctPkg/TestCase/UEFI/EFI/RuntimeServices/SecureBoot/BlackBoxTest/Dependency/Images/Images.inf|#SctPkg/TestCase/UEFI/EFI/RuntimeServices/SecureBoot/BlackBoxTest/Dependency/Images/Images.inf|g' $BBR_DIR/common/sct-tests/sbbr-tests/BBR_SCT.dsc
sed -i \
's|SctPkg/TestCase/UEFI/EFI/Generic/PlatformResetAttackMitigationPsciTest/BlackBoxTest/PlatformResetAttackMitigationPsciBBTest.inf|#SctPkg/TestCase/UEFI/EFI/Generic/PlatformResetAttackMitigationPsciTest/BlackBoxTest/PlatformResetAttackMitigationPsciBBTest.inf|g' $BBR_DIR/common/sct-tests/sbbr-tests/BBR_SCT.dsc
fi

do_build()
{
    pushd $TOP_DIR/$SCT_PATH
    export KEYS_DIR=$TOP_DIR/bbsr-keys
    export EDK2_TOOLCHAIN=$UEFI_TOOLCHAIN

    # required for BBSR keys generation
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
    cp $SBBR_TEST_DIR/BBR_SCT.dsc uefi-sct/SctPkg/UEFI/
    cp $SBBR_TEST_DIR/build_bbr.sh uefi-sct/SctPkg/

    # copy BBSR SCT tests to edk2-test
    if [[ $BUILD_TYPE != S ]]; then
        cp -r $BBSR_TEST_DIR/BBSRVariableSizeTest uefi-sct/SctPkg/TestCase/UEFI/EFI/RuntimeServices
        cp -r $BBSR_TEST_DIR/SecureBoot uefi-sct/SctPkg/TestCase/UEFI/EFI/RuntimeServices
        cp -r $BBSR_TEST_DIR/PlatformResetAttackMitigationPsciTest \
        uefi-sct/SctPkg/TestCase/UEFI/EFI/Generic/
    fi

    #Startup/runtime files.
    mkdir -p uefi-sct/SctPkg/BBR
    # undo any previously applied patches on edk2-test
    git checkout .

    if [ $BUILD_PLAT = EBBR ]; then
        #EBBR
        cp $BBR_DIR/ebbr/config/EBBRStartup.nsh uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/ebbr/config/EBBR.seq uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/ebbr/config/EBBR_manual.seq uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/ebbr/config/EBBR_extd_run.seq uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/ebbr/config/EfiCompliant_EBBR.ini uefi-sct/SctPkg/BBR/
    elif [ $BUILD_PLAT = SBBR ]; then
    #SBBR
        cp $BBR_DIR/sbbr/config/SBBRStartup.nsh uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/sbbr/config/SBBR.seq uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/sbbr/config/SBBR_manual.seq uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/sbbr/config/SBBR_extd_run.seq uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/sbbr/config/EfiCompliant_SBBR.ini  uefi-sct/SctPkg/BBR/
        if [[ $BUILD_TYPE != S ]]; then
        if git apply --check $TOP_DIR/patches/sctversion.patch; then
                echo "Applying edk2-test BBR sctversion patch..."
                git apply --ignore-whitespace --ignore-space-change $TOP_DIR/patches/sctversion.patch
            else
                echo  "Error while applying edk2-test BBR sctversion patch..."
                exit
            fi
        fi
    fi
    #Common
    #SCRT
    cp $BBR_DIR/common/config/ScrtStartup.nsh uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/common/config/SCRT.conf uefi-sct/SctPkg/BBR/

    # apply version patches for standalone BBR builds
    if [[ $BUILD_TYPE = S ]]; then
        if [ $BUILD_PLAT = EBBR ]; then
            if git apply --check $BBR_DIR/ebbr/patches/standalone_ebbr_ver.patch; then
                echo "Applying EBBR SCT version patch..."
                git apply --ignore-whitespace --ignore-space-change $BBR_DIR/ebbr/patches/standalone_ebbr_ver.patch
            else
                echo  "Error while applying EBBR SCT version patch..."
                exit
            fi
        elif [ $BUILD_PLAT = SBBR ]; then
            if git apply --check $BBR_DIR/sbbr/patches/standalone_sbbr_ver.patch; then
                echo "Applying SBBR SCT version patch..."
                git apply --ignore-whitespace --ignore-space-change $BBR_DIR/sbbr/patches/standalone_sbbr_ver.patch
            else
                echo  "Error while applying SBBR SCT version patch..."
                exit
            fi
        fi
    fi

    if git apply --check $BBR_DIR/common/patches/edk2-test-bbr-build.patch; then
        echo "Applying edk2-test BBR build patch..."
        git apply --ignore-whitespace --ignore-space-change $BBR_DIR/common/patches/edk2-test-bbr-build.patch
    else
        echo  "Error while applying edk2-test BBR build patch..."
        exit
    fi
    if git apply --check $BBR_DIR/common/patches/edk2-test-bbr.patch; then
        echo "Applying edk2-test BBR patch..."
        git apply --ignore-whitespace --ignore-space-change $BBR_DIR/common/patches/edk2-test-bbr.patch
    else
        echo  "Error while applying edk2-test BBR patch..."
        exit
    fi
    if [[ $BUILD_TYPE != S ]]; then
        if git apply --check $BBR_DIR/bbsr/patches/0001-BBSR-Patch-for-UEFI-SCT-Build.patch; then
            echo "Applying BBSR SCT patch..."
            git apply --ignore-whitespace --ignore-space-change $BBR_DIR/bbsr/patches/0001-BBSR-Patch-for-UEFI-SCT-Build.patch
        else
            echo  "Error while applying BBSR SCT patch..."
            exit
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

    if [ $BUILD_PLAT = EBBR ]; then
        #EBBR
        cp -r Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/${TARGET_ARCH}/* ${TARGET_ARCH}_SCT/SCT/
        cp Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}/SctPackage${TARGET_ARCH}/EBBRStartup.nsh ${TARGET_ARCH}_SCT/SctStartup.nsh
        cp SctPkg/BBR/EfiCompliant_EBBR.ini ${TARGET_ARCH}_SCT/SCT/Dependency/EfiCompliantBBTest/EfiCompliant.ini
        cp SctPkg/BBR/EBBR_manual.seq ${TARGET_ARCH}_SCT/SCT/Sequence/EBBR_manual.seq

    elif [ $BUILD_PLAT = SBBR ]; then
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
            cp $BBR_DIR/bbsr/config/BBSRStartup.nsh ${TARGET_ARCH}_SCT/bbsr_SctStartup.nsh
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


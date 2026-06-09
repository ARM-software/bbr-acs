#!/usr/bin/env bash

#  Copyright (c) 2021-2026, Arm Limited or its affiliates. All rights reserved.
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
# UEFI_TOOLCHAIN - Toolchain supported by Linaro uefi-tools:
#     GCC49, GCC48 or GCC47
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
SCT_FRAMEWORK_PREFIX="$TOP_DIR/$SCT_PATH/uefi-sct/Build/bbrSct"
SCT_FRAMEWORK_ROOT="$SCT_FRAMEWORK_PREFIX/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}"
SCT_FRAMEWORK="$SCT_FRAMEWORK_ROOT/SctPackage${TARGET_ARCH}/${TARGET_ARCH}"

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


if ! [[ $BUILD_PLAT = EBBR ]] && ! [[ $BUILD_PLAT = SBBR ]]; then
    echo "Please provide a target."
    echo "Usage build-sct.sh <EBBR/SBBR> <BUILD_TYPE>"
    exit
fi

if ! [[ $BUILD_TYPE = S ]] && ! [[ $BUILD_TYPE = F ]]; then
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
EBBR_TEST_DIR=$BBR_DIR/ebbr/sct-tests
BBR_SCT_DSC=$BBR_DIR/common/sct-tests/sbbr-tests/BBR_SCT.dsc

SCTPKG_TEST_DIR="SctPkg/TestCase/UEFI/EFI"
generic_dir="$SCTPKG_TEST_DIR/Generic"
protocol_dir="$SCTPKG_TEST_DIR/Protocol"
runtime_services="$SCTPKG_TEST_DIR/RuntimeServices"
secureboot_dir="$runtime_services/SecureBoot"

if [[ $BUILD_TYPE = S ]]; then
    ## These tests are hosted locally in BBSR folder

    secureboot_bbtest="$secureboot_dir/BlackBoxTest/SecureBootBBTest.inf"
    bbsr_vs_dir="$runtime_services/BBSRVariableSizeTest"
    bbsr_vs_bbtest="$bbsr_vs_dir/BlackBoxTest/BBSRVariableSizeBBTest.inf"
    pram_dir="$generic_dir/PlatformResetAttackMitigationPsciTest"
    pram_bbtest_dir="$pram_dir/BlackBoxTest"
    pram_bbtest="$pram_bbtest_dir/PlatformResetAttackMitigationPsciBBTest.inf"
    secureboot_images_dir="$secureboot_dir/BlackBoxTest/Dependency/Images"
    secureboot_images="$secureboot_images_dir/Images.inf"

    sed -i "s|$secureboot_bbtest|#$secureboot_bbtest|g" "$BBR_SCT_DSC"
    sed -i "s|$bbsr_vs_bbtest|#$bbsr_vs_bbtest|g" "$BBR_SCT_DSC"
    sed -i "s|$pram_bbtest|#$pram_bbtest|g" "$BBR_SCT_DSC"
    sed -i "s|$secureboot_images|#$secureboot_images|g" "$BBR_SCT_DSC"

    ## These tests are in edk2-test repo
    if [[ $BUILD_PLAT = SBBR ]]; then
        tcg2_protocol_dir="$protocol_dir/TCG2/BlackBoxTest"
        tcg2_protocol_bbtest="$tcg2_protocol_dir/TCG2ProtocolBBTest.inf"
        sed -i "s|$tcg2_protocol_bbtest|#$tcg2_protocol_bbtest|g" \
            "$BBR_SCT_DSC"
    fi

    tcg_mem_dir="$runtime_services/TCGMemoryOverwriteRequest"
    tcg_mem_bbtest_dir="$tcg_mem_dir/BlackBoxTest"
    tcg_mem_bbtest="$tcg_mem_bbtest_dir/TCGMemoryOverwriteRequestBBTest.inf"
    sed -i "s|$tcg_mem_bbtest|#$tcg_mem_bbtest|g" \
        "$BBR_SCT_DSC"
fi

if [[ $BUILD_PLAT = SBBR ]]; then
    ## These tests are hosted locally in EBBR folder
    conf_profile_dir="$generic_dir/EfiCompliant/EfiEbbrProfileTableTest/BlackBoxTest"
    conf_profile_name="EfiEbbrProfileTableTest.inf"
    conf_profile_bbtest="$conf_profile_dir/$conf_profile_name"
    sed -i "s|$conf_profile_bbtest|#$conf_profile_bbtest|g" "$BBR_SCT_DSC"
fi

do_build()
{
    export PACKAGES_PATH=$TOP_DIR/$UEFI_PATH
    export PYTHON_COMMAND=/usr/bin/python3
    export WORKSPACE=$TOP_DIR/$SCT_PATH/uefi-sct
    export EDK2_TOOLCHAIN=$UEFI_TOOLCHAIN

    # required for BBSR keys generation
    if [[ $BUILD_TYPE != S ]]; then
        export KEYS_DIR=$TOP_DIR/bbsr-keys
        export PATH="$PATH:$TOP_DIR/efitools"
    fi

    pushd $TOP_DIR/$SCT_PATH

    #Build base tools
    source $TOP_DIR/$UEFI_PATH/edksetup.sh
    make -C $TOP_DIR/$UEFI_PATH/BaseTools

    #Copy over extra files needed for SBBR tests
    generic_test_dir="uefi-sct/SctPkg/TestCase/UEFI/EFI/Generic"
    bbr_uefi_dir="uefi-sct/SctPkg/UEFI"

    sctpkg_efi_dir="uefi-sct/SctPkg/TestCase/UEFI/EFI"
    cp -r "$SBBR_TEST_DIR/SbbrBootServices" \
        "$sctpkg_efi_dir/BootServices/"
    cp -r \
        "$SBBR_TEST_DIR/SbbrEfiSpecVerLvl" \
        "$SBBR_TEST_DIR/SbbrRequiredUefiProtocols" \
        "$SBBR_TEST_DIR/SbbrSysEnvConfig" \
        "$generic_test_dir/"
    cp "$SBBR_TEST_DIR/BBR_SCT.dsc" "$bbr_uefi_dir/"
    cp "$SBBR_TEST_DIR/build_bbr.sh" "uefi-sct/SctPkg/"

    if [[ $BUILD_PLAT = EBBR ]]; then
        mkdir -p "$generic_test_dir/EfiCompliant"
        cp -r "$EBBR_TEST_DIR/EfiEbbrProfileTableTest" \
            "$generic_test_dir/EfiCompliant/"
    fi

    # copy BBSR SCT tests to edk2-test
    if [[ $BUILD_TYPE != S ]]; then
        sctpkg_runtime_dir="$sctpkg_efi_dir/RuntimeServices"
        cp -r "$BBSR_TEST_DIR/BBSRVariableSizeTest" \
            "$sctpkg_runtime_dir"
        cp -r "$BBSR_TEST_DIR/PlatformResetAttackMitigationPsciTest" \
            "$generic_test_dir/"
    fi

    #Startup/runtime files.
    mkdir -p uefi-sct/SctPkg/BBR
    # undo any previously applied patches on edk2-test
    git checkout .
    git_apply_args="--ignore-whitespace --ignore-space-change"

    if [ $BUILD_PLAT = EBBR ]; then
        cp "$BBR_DIR/ebbr/config/EBBRStartup.nsh" uefi-sct/SctPkg/BBR/
        cp "$BBR_DIR/ebbr/config/EBBR.seq" uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/ebbr/config/EBBR_manual.seq uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/ebbr/config/EBBR_extd_run.seq uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/ebbr/config/EfiCompliant_EBBR.ini uefi-sct/SctPkg/BBR/
    elif [ $BUILD_PLAT = SBBR ]; then
        cp $BBR_DIR/sbbr/config/SBBRStartup.nsh uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/sbbr/config/SBBR.seq uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/sbbr/config/SBBR_extd_run.seq uefi-sct/SctPkg/BBR/
        cp $BBR_DIR/sbbr/config/EfiCompliant_SBBR.ini  uefi-sct/SctPkg/BBR/
        if [[ $BUILD_TYPE != S ]]; then
            patch_file="$TOP_DIR/patches/sctversion.patch"
            if git apply --check "$patch_file"; then
                echo "Applying edk2-test BBR sctversion patch..."
                git apply $git_apply_args "$patch_file"
            else
                echo "Error applying edk2-test BBR sctversion patch..."
                exit
            fi
        fi
    fi

    #Common - SCRT
    cp $BBR_DIR/common/config/ScrtStartup.nsh uefi-sct/SctPkg/BBR/
    cp $BBR_DIR/common/config/SCRT.conf uefi-sct/SctPkg/BBR/

    # Apply version patches for standalone BBR builds
    if [[ $BUILD_TYPE = S ]]; then
        if [ $BUILD_PLAT = EBBR ]; then
            patch_file="$BBR_DIR/ebbr/patches/standalone_ebbr_ver.patch"
            if git apply --check "$patch_file"; then
                echo "Applying EBBR SCT version patch..."
                git apply $git_apply_args "$patch_file"
            else
                echo "Error applying EBBR SCT version patch..."
                exit
            fi
        elif [ $BUILD_PLAT = SBBR ]; then
            patch_file="$BBR_DIR/sbbr/patches/standalone_sbbr_ver.patch"
            if git apply --check "$patch_file"; then
                echo "Applying SBBR SCT version patch..."
                git apply $git_apply_args "$patch_file"
            else
                echo "Error applying SBBR SCT version patch..."
                exit
            fi
        fi
    fi

    # Apply edk2-test-bbr-build patch
    patch_file="$BBR_DIR/common/patches/edk2-test-bbr-build.patch"
    if git apply --check "$patch_file"; then
        echo "Applying edk2-test BBR build patch..."
        git apply $git_apply_args "$patch_file"
    else
        echo "Error applying edk2-test BBR build patch..."
        exit
    fi

    # Apply edk2-test-bbr patch
    patch_file="$BBR_DIR/common/patches/edk2-test-bbr.patch"
    if git apply --check "$patch_file"; then
        echo "Applying edk2-test BBR patch..."
        git apply $git_apply_args "$patch_file"
    else
        echo "Error applying edk2-test BBR patch..."
        exit
    fi

    # Apply EBBR-only SCT patches: patch1 to edk2-test, patch2 to edk2.
    if [[ "$BUILD_PLAT" == "EBBR" ]]; then
        EBBR_SCT_PATCH_DIR="$BBR_DIR/ebbr/patches"
        EBBR_SCT_PATCH1_NAME="$(printf '%s%s' \
            "0001-sct-getnextmonotoniccount-" \
            "handle-device-error.patch")"
        EBBR_SCT_PATCH2_NAME="$(printf '%s%s' \
            "edk2-update-ConformanceProfiles-with-EBBR-2.3-" \
            "GUID.patch")"
        EBBR_SCT_PATCH1="$EBBR_SCT_PATCH_DIR/$EBBR_SCT_PATCH1_NAME"
        EBBR_SCT_PATCH2="$EBBR_SCT_PATCH_DIR/$EBBR_SCT_PATCH2_NAME"

        if git -C "$TOP_DIR/$SCT_PATH" apply --check "$EBBR_SCT_PATCH1"; then
            echo "Applying EBBR SCT GetNextMonotonicCount patch..."
            git -C "$TOP_DIR/$SCT_PATH" \
                apply --ignore-whitespace --ignore-space-change \
                "$EBBR_SCT_PATCH1"
        else
            echo "Error applying EBBR SCT GetNextMonotonicCount patch..."
            exit
        fi

        # Find edk2 repository directory in common candidate locations.
        # Apply patch2 in the located edk2 repo.
        EDK2_CANDIDATES=(
            "$TOP_DIR/$UEFI_PATH"
            "$TOP_DIR/../$UEFI_PATH"
            "$TOP_DIR/ebbr/scripts/$UEFI_PATH"
            "$BBR_DIR/ebbr/$UEFI_PATH"
            "$BBR_DIR/ebbr/scripts/$UEFI_PATH"
        )
        EDK2_DIR=""
        for d in "${EDK2_CANDIDATES[@]}"; do
            if [ -d "$d" ] && [ -d "$d/MdePkg" ]; then
                EDK2_DIR="$d"
                break
            fi
        done
        if [ -z "$EDK2_DIR" ]; then
            echo "Error: edk2 repository directory not found"
            echo "Cannot apply ConformanceProfiles patch"
            exit
        fi
        if git -C "$EDK2_DIR" apply --check $git_apply_args "$EBBR_SCT_PATCH2"; then
            echo "Applying EBBR ConformanceProfiles 2.3 GUID patch"
            echo "Repository: $EDK2_DIR"
            git -C "$EDK2_DIR" \
                apply \
                --ignore-whitespace \
                --ignore-space-change \
                "$EBBR_SCT_PATCH2"
        else
            echo "Error applying EBBR ConformanceProfiles 2.3 GUID patch"
            echo "Repository: $EDK2_DIR"
            exit
        fi
    fi

    # Apply BBSR patch for Systemready
    if [[ $BUILD_TYPE != S ]]; then
        bbsr_patch_name="0001-BBSR-Patch-for-UEFI-SCT-Build.patch"
        patch_file="$BBR_DIR/bbsr/patches/$bbsr_patch_name"
        if git apply --check "$patch_file"; then
            echo "Applying BBSR SCT patch..."
            git apply $git_apply_args "$patch_file"
        else
            echo "Error applying BBSR SCT patch..."
            exit
        fi

        secureboot_patch_name="0001-UEFI-SCT-SecureBoot-change-attribute-\
check-to-warning.patch"
        patch_file="$BBR_DIR/bbsr/patches/$secureboot_patch_name"
        if git apply --check "$patch_file"; then
            echo "Applying BBSR SecureBoot change patch..."
            git apply $git_apply_args "$patch_file"
        else
            echo "Error applying BBSR SecureBoot patch..."
            exit
        fi
    fi

    pushd uefi-sct
    if [[ $BUILD_TYPE != S ]]; then
        ./SctPkg/build_bbr.sh "$TARGET_ARCH" GCC "$UEFI_BUILD_MODE" \
            ENABLE_SECUREBOOT_TESTS -n "$PARALLELISM"
    else
        ./SctPkg/build_bbr.sh "$TARGET_ARCH" GCC "$UEFI_BUILD_MODE" \
            -n "$PARALLELISM"
    fi
    popd
}

do_clean()
{
    pushd $TOP_DIR/$SCT_PATH/uefi-sct
    export PACKAGES_PATH=$TOP_DIR/$UEFI_PATH
    export PYTHON_COMMAND=/usr/bin/python3
    export WORKSPACE=$TOP_DIR/$SCT_PATH/uefi-sct


    source $TOP_DIR/$UEFI_PATH/edksetup.sh
    make -C $TOP_DIR/$UEFI_PATH/BaseTools clean
    rm -rf Build
    rm -rf ${TARGET_ARCH}_SCT
    if [[ $BUILD_TYPE = S ]]; then
        rm -rf ${BUILD_PLAT}-SCT
    fi

    popd

}

# sign SCT efi files
SecureBootSign() {
    echo "KEYS_DIR = $KEYS_DIR"

    for f in "$1"/*.efi; do
        echo "sbsign --key $TEST_DB1_KEY --cert $TEST_DB1_CRT \"$f\" --output \"$f\""
        sbsign --key "$TEST_DB1_KEY" --cert "$TEST_DB1_CRT" "$f" \
            --output "$f"
    done
}

# signing SCT test dependency files
SecureBootSignDependency() {
    echo "KEYS_DIR = $KEYS_DIR"

    for f in "$SCT_FRAMEWORK/Dependency/$1BBTest"/*.efi; do
        echo "sbsign --key $TEST_DB1_KEY --cert $TEST_DB1_CRT \"$f\" --output \"$f\""
        sbsign --key "$TEST_DB1_KEY" --cert "$TEST_DB1_CRT" "$f" \
            --output "$f"
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

    build_package_root="Build/bbrSct/${UEFI_BUILD_MODE}_${UEFI_TOOLCHAIN}"
    build_package_dir="$build_package_root/SctPackage${TARGET_ARCH}"

    if [ $BUILD_PLAT = EBBR ]; then
        #EBBR
        cp -r "$build_package_dir/$TARGET_ARCH/"* \
            "${TARGET_ARCH}_SCT/SCT/"
        cp "$build_package_dir/EBBRStartup.nsh" \
            "${TARGET_ARCH}_SCT/SctStartup.nsh"
        sct_dependency_dir="${TARGET_ARCH}_SCT/SCT/Dependency/"
        sct_dependency_dir+="EfiCompliantBBTest"
        mkdir -p "$sct_dependency_dir"
        cp "SctPkg/BBR/EfiCompliant_EBBR.ini" \
            "$sct_dependency_dir/EfiCompliant.ini"
        cp "SctPkg/BBR/EBBR_manual.seq" \
            "${TARGET_ARCH}_SCT/SCT/Sequence/EBBR_manual.seq"

    elif [ $BUILD_PLAT = SBBR ]; then
        # Sign the SCT binaries
        if [ $BUILD_TYPE != S ]; then
            SecureBootSign "$SCT_FRAMEWORK"
            SecureBootSign "$SCT_FRAMEWORK/Support"
            SecureBootSign "$build_package_dir"
            SecureBootSign "$SCT_FRAMEWORK/SCRT"
            SecureBootSign "$SCT_FRAMEWORK/Test"
            SecureBootSign "$SCT_FRAMEWORK/Ents/Support"
            SecureBootSign "$SCT_FRAMEWORK/Ents/Test"
            SecureBootSignDependency LoadedImage
            SecureBootSignDependency ImageServices
            SecureBootSignDependency ProtocolHandlerServices
            SecureBootSignDependency ConfigKeywordHandler
            SecureBootSignDependency PciIo
            #BBSR
            cp "$BBR_DIR/bbsr/config/BBSRStartup.nsh" \
                "${TARGET_ARCH}_SCT/bbsr_SctStartup.nsh"
            cp "$BBR_DIR/bbsr/config/BBSR.seq" \
                "${TARGET_ARCH}_SCT/SCT/Sequence/"
        fi
        #SBBR
        cp -r "$build_package_dir/$TARGET_ARCH/"* \
            "${TARGET_ARCH}_SCT/SCT/"
        sct_dependency_dir="${TARGET_ARCH}_SCT/SCT/Dependency/"
        sct_dependency_dir+="EfiCompliantBBTest"
        mkdir -p "$sct_dependency_dir"
        cp "$build_package_dir/SBBRStartup.nsh" \
            "${TARGET_ARCH}_SCT/SctStartup.nsh"
        cp "SctPkg/BBR/EfiCompliant_SBBR.ini" \
            "$sct_dependency_dir/EfiCompliant.ini"
        cp "SctPkg/BBR/SBBR_extd_run.seq" \
            "${TARGET_ARCH}_SCT/SCT/Sequence/SBBR_extd_run.seq"
    else
        echo "Error: unexpected platform type"
        exit
    fi

    #Common
    #SCRT
    cp SctPkg/BBR/ScrtStartup.nsh ${TARGET_ARCH}_SCT/ScrtStartup.nsh
    cp SctPkg/BBR/SCRT.conf ${TARGET_ARCH}_SCT/SCT/SCRT/SCRT.conf

    if [[ $BUILD_TYPE = S ]]; then
        #Create Standalone folder
        mkdir -p ${BUILD_PLAT}-SCT
        mkdir -p ${BUILD_PLAT}-SCT/EFI/BOOT
        mkdir -p ${BUILD_PLAT}-SCT/SCT
        cp -r ${TARGET_ARCH}_SCT/SCT/* ${BUILD_PLAT}-SCT/SCT/
        cp "$BBR_DIR/common/scripts/StandaloneSctStartup.nsh" \
            "${BUILD_PLAT}-SCT/StandaloneSctStartup.nsh"
        cp "$BBR_DIR/common/scripts/SampleStartup.nsh" \
            "${BUILD_PLAT}-SCT/startup.nsh"
    fi

    pushd $TOP_DIR

}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/framework.sh $@


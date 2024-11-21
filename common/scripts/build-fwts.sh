#!/usr/bin/env bash

# Copyright (c) 2021-2024, Arm Limited or its affiliates. All rights reserved.
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

TOP_DIR=`pwd`
arch=$(uname -m)
FWTS_PATH=fwts
FWTS_BINARY=fwts_output
RAMDISK_PATH=ramdisk
FWTS_DEP=$RAMDISK_PATH/fwts_build_dep
if [[ $arch != "aarch64" ]]; then
    CROSS_COMPILE=$TOP_DIR/$GCC
fi

BUILD_PLAT=$1
BUILD_TYPE=$2

if ! [[ $BUILD_TYPE = S ]] && ! [[  $BUILD_TYPE = F  ]] ; then
    echo "Please provide a Build type."
    echo "Usage build-fwts.sh <target> <S/F>"
    echo "S->Standalone BBR,F->Full systemready"
    exit
fi

if [[ $BUILD_TYPE = S ]]; then
    BBR_DIR=$TOP_DIR/../..
else
    BBR_DIR=$TOP_DIR/bbr-acs
fi

init()
{
    if [[ $BUILD_TYPE = S ]]; then
        mkdir -p $TOP_DIR/$RAMDISK_PATH
    fi
    cp -r $BBR_DIR/common/fwts_build_dep $RAMDISK_PATH
}

do_build()
{
    pushd $TOP_DIR/$FWTS_PATH
    if [[ $arch != "aarch64" ]]; then
        CROSS_COMPILE_DIR=$(dirname $CROSS_COMPILE)
        DEF_PATH=$PATH
        PATH=$(getconf PATH) #Reset path to avoid cross compiler mismatch
        PATH="$PATH:$CROSS_COMPILE_DIR"
    fi

    echo $BBR_DIR

    if [ "$BUILD_PLAT" = "EBBR" ]; then
        if ! git apply --check $BBR_DIR/ebbr/patches/ACS_VER_fwts.patch ; then
            echo "FWTS Version patch apply failed"
            exit 1
        else
            git apply $BBR_DIR/ebbr/patches/ACS_VER_fwts.patch
        fi
    fi

    if [ "$BUILD_PLAT" = "SBBR" ]; then
        if ! git apply --check $BBR_DIR/sbbr/patches/ACS_VER_fwts.patch ; then
            echo "FWTS Version patch apply failed"
            exit 1
        else
            git apply $BBR_DIR/sbbr/patches/ACS_VER_fwts.patch
        fi
    fi
    #The below patch is temporary and shall be removed once a permanent solution
    #in FWTS codebase is found
    if ! git apply --check $BBR_DIR/common/patches/0001-Fix-for-FWTS-build-issue.patch; then
        echo "FWTS build patch apply failed"
        exit 1
    else
        git apply $BBR_DIR/common/patches/0001-Fix-for-FWTS-build-issue.patch
    fi

    mkdir -p $FWTS_BINARY
    mkdir -p $FWTS_BINARY/bash
    autoreconf -ivf
    export ac_cv_func_malloc_0_nonnull=yes
    export ac_cv_func_realloc_0_nonnull=yes
    if [[ $arch != "aarch64" ]]; then
        ./configure --host=aarch64-linux-gnu  \
        --enable-static=yes CFLAGS="-g -O2 -I$TOP_DIR/$FWTS_DEP/include" \
        LDFLAGS="-L$TOP_DIR/$FWTS_DEP -Wl,-rpath-link,$TOP_DIR/$FWTS_DEP \
        -Wl,-rpath-link,$TOP_DIR/$FWTS_PATH/src/libfwtsiasl/.libs/" \
        --prefix=$TOP_DIR/$FWTS_PATH/$FWTS_BINARY \
        --exec-prefix=$TOP_DIR/$FWTS_PATH/$FWTS_BINARY --datarootdir=$TOP_DIR/$FWTS_PATH/$FWTS_BINARY \
        --with-bashcompletiondir=$TOP_DIR/$FWTS_PATH/$FWTS_BINARY/bash
    else
        ./configure \
        --enable-static=yes CFLAGS="-g -O2 -I$TOP_DIR/$FWTS_DEP/include" \
        LDFLAGS="-L$TOP_DIR/$FWTS_DEP -Wl,-rpath-link,$TOP_DIR/$FWTS_DEP \
        -Wl,-rpath-link,$TOP_DIR/$FWTS_PATH/src/libfwtsiasl/.libs/" \
        --prefix=$TOP_DIR/$FWTS_PATH/$FWTS_BINARY \
        --exec-prefix=$TOP_DIR/$FWTS_PATH/$FWTS_BINARY --datarootdir=$TOP_DIR/$FWTS_PATH/$FWTS_BINARY \
        --with-bashcompletiondir=$TOP_DIR/$FWTS_PATH/$FWTS_BINARY/bash
    fi

    make install
    if [[ $arch != "aarch64" ]]; then
        PATH=$DEF_PATH #Restore def path
    fi
    popd
}

do_clean()
{
    pushd $TOP_DIR/$FWTS_PATH
    if [[ $arch != "aarch64" ]]; then
        CROSS_COMPILE_DIR=$(dirname $CROSS_COMPILE)
        PATH="$PATH:$CROSS_COMPILE_DIR"
    fi
    if [ -f "$TOP_DIR/$FWTS_PATH/Makefile" ]; then
        make clean
    fi
    if [ -f "$TOP_DIR/$FWTS_PATH/$FWTS_BINARY/bin/fwts" ]; then
        make uninstall
    fi
    rm -rf $TOP_DIR/$RAMDISK_PATH/$FWTS_BINARY
    popd
}

do_package ()
{
    echo "Packaging FWTS... $VARIANT";
    if [[ $BUILD_TYPE = F ]]; then
        sed -i '/ir_bbr_fwts_tests.ini/d' $TOP_DIR/ramdisk/files.txt
        if [ "$BUILD_PLAT" = "EBBR" ]; then
          #Add the entry in file.txt of ramdisk
          echo "file /bin/ir_bbr_fwts_tests.ini         ./fwts_output/bin/ir_bbr_fwts_tests.ini                   766 0 0" >> $TOP_DIR/ramdisk/files.txt
          cp $BBR_DIR/ebbr/config/ir_bbr_fwts_tests.ini $TOP_DIR/$FWTS_PATH/$FWTS_BINARY/bin
        fi
    fi
    cp -R $TOP_DIR/$FWTS_PATH/$FWTS_BINARY ramdisk
    chmod 777 -R $TOP_DIR/$RAMDISK_PATH/$FWTS_BINARY
}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
init
source $DIR/framework.sh $@

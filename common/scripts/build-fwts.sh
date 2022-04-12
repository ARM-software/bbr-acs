#!/usr/bin/env bash

# Copyright (c) 2021-2022, ARM Limited and Contributors. All rights reserved.
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

TOP_DIR=`pwd`
# toolchain
. $TOP_DIR/../../common/scripts/common_cross_toolchain.sh

FWTS_PATH=fwts
FWTS_BINARY=fwts_output
RAMDISK_PATH=ramdisk
FWTS_DEP=$RAMDISK_PATH/fwts_build_dep
BUILD_PLAT=$1
BUILD_TYPE=$2

if [ $BUILD_PLAT = SR ]; then
   BUILD_PLAT=ES
fi

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
    CROSS_COMPILE_DIR=$(dirname $CROSS_COMPILE)
    DEF_PATH=$PATH
    PATH=$(getconf PATH) #Reset path to avoid cross compiler mismatch
    PATH="$PATH:$CROSS_COMPILE_DIR"

    echo $BBR_DIR

    if [ "$BUILD_PLAT" = "IR" ]; then
        if ! patch -R -s -f --dry-run -p1 < $BBR_DIR/ebbr/config/IR_VER.patch ; then
        echo "Applying FWTS Patch ..."
        patch -p1 < $BBR_DIR/ebbr/config/IR_VER.patch
        fi
    fi
    if [ "$BUILD_PLAT" = "ES" ]; then
        if ! patch -R -s -f --dry-run -p1 < $BBR_DIR/sbbr/config/ES_VER.patch ; then
        echo "Applying FWTS Patch ..."
        patch -p1 < $BBR_DIR/sbbr/config/ES_VER.patch
        fi
    fi

    #The below patch is temporary and shall be removed once a permanent solution
    #in FWTS codebase is found
    if ! patch -R -s -f --dry-run -p1 < $BBR_DIR/common/patches/0001-Fix-for-FWTS-build-issue.patch; then
        echo "Applying FWTS build patch ..."
        patch -p1 < $BBR_DIR/common/patches/0001-Fix-for-FWTS-build-issue.patch
    fi

    mkdir -p $FWTS_BINARY
    mkdir -p $FWTS_BINARY/bash
    autoreconf -ivf
    export ac_cv_func_malloc_0_nonnull=yes
    export ac_cv_func_realloc_0_nonnull=yes
    ./configure --host=aarch64-linux-gnu  \
    --enable-static=yes CFLAGS="-g -O2 -I$TOP_DIR/$FWTS_DEP/include" \
    LDFLAGS="-L$TOP_DIR/$FWTS_DEP -Wl,-rpath-link,$TOP_DIR/$FWTS_DEP \
    -Wl,-rpath-link,$TOP_DIR/$FWTS_PATH/src/libfwtsiasl/.libs/" \
    --prefix=$TOP_DIR/$FWTS_PATH/$FWTS_BINARY \
    --exec-prefix=$TOP_DIR/$FWTS_PATH/$FWTS_BINARY --datarootdir=$TOP_DIR/$FWTS_PATH/$FWTS_BINARY \
    --with-bashcompletiondir=$TOP_DIR/$FWTS_PATH/$FWTS_BINARY/bash

    make install
    PATH=$DEF_PATH #Restore def path
    popd
}

do_clean()
{
    pushd $TOP_DIR/$FWTS_PATH
    CROSS_COMPILE_DIR=$(dirname $CROSS_COMPILE)
    PATH="$PATH:$CROSS_COMPILE_DIR"

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
        if [ "$BUILD_PLAT" = "IR" ]; then
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

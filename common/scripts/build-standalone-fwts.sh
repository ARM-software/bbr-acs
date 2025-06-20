#!/usr/bin/env bash

# Copyright (c) 2025, Arm Limited or its affiliates. All rights reserved.
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

# FWTS Build Script for AArch64 using Buildroot on both Ubuntu x86_64 and AArch64 Native Systems
# Usage: ./build-standalone-fwts.sh
set -e

TOP_DIR="$(pwd)"
FWTS_WORKSPACE="$TOP_DIR/fwts_workspace"
BUILDROOT_DIR="$FWTS_WORKSPACE/buildroot"
BUILDROOT_CONFIG_FILE="buildroot_defconfig"
BUILDROOT_CONFIG_URL="https://raw.githubusercontent.com/ARM-software/arm-systemready/main/common/config/buildroot_defconfig"
CONFIG_URL="https://raw.githubusercontent.com/ARM-software/arm-systemready/refs/heads/main/common/config/systemready-band-source.cfg"
# User can set this variable to a specific version supported by ARM SystemReady to build a specific FWTS version
USER_DEFINED_FWTS_VERSION=""

function get_source() {
    echo "==> Installing dependencies..."
    sudo apt install -y build-essential bison flex libncurses-dev \
        unzip zip rsync python3 perl git pkg-config bc wget curl \
        cmake ninja-build libssl-dev

    if [ -d "$TOP_DIR/fwts_workspace" ]; then
        echo "FWTS worksapce directory already exists."
    else
        echo "Creating FWTS worksapce."
        mkdir -p "$TOP_DIR/fwts_workspace"
    fi

    echo "Downloading configuration file from ARM SystemReady GitHub to retrieve the default supported FWTS version..."
    if [ -f "$FWTS_WORKSPACE/systemready-band-source.cfg" ]; then
        . $FWTS_WORKSPACE/systemready-band-source.cfg
    else
        wget "$CONFIG_URL" -O "$FWTS_WORKSPACE/systemready-band-source.cfg"
        . $FWTS_WORKSPACE/systemready-band-source.cfg
    fi

    echo "==> Downloading Buildroot source code. TAG: $BUILDROOT_SRC_VERSION"
    if [ -d "$BUILDROOT_DIR" ]; then
        echo "Buildroot directory already exists. Skipping clone."
    else
        git clone -b "$BUILDROOT_SRC_VERSION" https://gitlab.com/buildroot.org/buildroot.git "$BUILDROOT_DIR"
    fi

    echo "==> Downloading and applying FWTS patch for either the default arm-systemready version or the used-defined FWTS version..."
    if [ -f "$FWTS_WORKSPACE/$PATCH_FILE" ]; then
        echo "Patch file already exists locally. Skipping download."
    else
        if [ "$USER_DEFINED_FWTS_VERSION" = "" ]; then
            PATCH_URL="https://raw.githubusercontent.com/ARM-software/arm-systemready/main/common/patches/build_fwts_version_$FWTS_VERSION.patch"
            PATCH_FILE="build_fwts_version_$FWTS_VERSION.patch"
        else
            PATCH_URL="https://raw.githubusercontent.com/ARM-software/arm-systemready/main/common/patches/build_fwts_version_$USER_DEFINED_FWTS_VERSION.patch"
            PATCH_FILE="build_fwts_version_$USER_DEFINED_FWTS_VERSION.patch"
        fi
        wget "$PATCH_URL" -O "$FWTS_WORKSPACE/$PATCH_FILE"
    fi

    # Check if patch is already applied
    pushd "$BUILDROOT_DIR/package/fwts" > /dev/null
    if git apply --check "$FWTS_WORKSPACE/$PATCH_FILE"; then
        echo "Applying FWTS patch..."
        git apply "$FWTS_WORKSPACE/$PATCH_FILE"
    else
        echo "Patch may already be applied or failed pre-check. Skipping."
    fi
    popd > /dev/null

    if [ -f "$FWTS_WORKSPACE/$BUILDROOT_CONFIG_FILE" ]; then
        echo "Config file already exists locally. Skipping download."
    else
        echo "Downloading Buildroot Config from ARM SystemReady GitHub..."
        wget "$BUILDROOT_CONFIG_URL" -O "$FWTS_WORKSPACE/$BUILDROOT_CONFIG_FILE"
    fi
}

function build_fwts() {
    cd "$BUILDROOT_DIR"
    echo "==> Cleaning previous build"
    make clean

    echo "==> Configuring Buildroot for AArch64 target."
    cp "$FWTS_WORKSPACE/$BUILDROOT_CONFIG_FILE" "$BUILDROOT_DIR/configs"
    make $BUILDROOT_CONFIG_FILE

    echo "==> Building FWTS..."
    if ! make -j"$(nproc)" fwts; then
        echo "FWTS build failed!" >&2
        exit 1
    fi

    echo "==> Build complete."
    echo "FWTS binaries and dependencies can be found in:"
    echo "  fwts_workspace/buildroot/output/target/usr/bin/fwts"
    echo "  fwts_workspace/buildroot/output/target/usr/bin/kernelscan"
    echo "  fwts_workspace/buildroot/output/target/usr/lib64/fwts/"
    echo "  fwts_workspace/buildroot/output/target/usr/lib/fwts/"
    echo "  fwts_workspace/buildroot/output/target/usr/share/fwts/"
}

get_source
build_fwts

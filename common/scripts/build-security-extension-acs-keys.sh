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
# This script uses the following environment variables
#
# TOP_DIR - workspace root directory
# KEYS_DIR - directory where secure boot keys are generated

TOP_DIR=`pwd`
KEYS_DIR=$TOP_DIR/security-extension-acs-keys

do_build()
{
    echo "do_build: security-extension-acs-keys"
    mkdir -p $KEYS_DIR
    pushd $KEYS_DIR

    # generate TestPK1
    NAME=TestPK1
    openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=TEST_PK/  -keyout $NAME.key -out $NAME.crt -nodes -days 4000
    openssl x509 -outform der -in $NAME.crt -out $NAME.der

    # generate TestKEK1
    NAME=TestKEK1
    openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=TEST_KEK/  -keyout $NAME.key -out $NAME.crt -nodes -days 4000
    openssl x509 -outform der -in $NAME.crt -out $NAME.der

    # generate TestDB1
    NAME=TestDB1
    openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=TEST_DB/  -keyout $NAME.key -out $NAME.crt -nodes -days 4000
    openssl x509 -outform der -in $NAME.crt -out $NAME.der
    # Convert TestDB1 to gpg form and import to gpg toolchain for use in signing grub
    cat TestDB1.key | PEM2OPENPGP_USAGE_FLAGS=certify,sign pem2openpgp "TestDB1"  > TestDB1.gpgkey
    gpg --import --allow-secret-key-import TestDB1.gpgkey
    gpg --export > TestDB1.pubgpg

    # generate TestDBX1
    NAME=TestDBX1
    openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=TEST_PK/  -keyout $NAME.key -out $NAME.crt -nodes -days 4000
    openssl x509 -outform der -in $NAME.crt -out $NAME.der

    popd
}

do_clean()
{
    echo "do_clean: security-extension-acs-keys"

    # delete gpg keys from previous runs
    keyname=$(gpg --list-keys "TestDB1" | head -2 | tail -1 | sed 's/^ *//g')
    gpg --batch --yes --delete-secret-keys $keyname
    gpg --batch --yes --delete-keys $keyname

    rm -rf $KEYS_DIR
}

do_package()
{
    echo "do_package: security-extension-acs-keys: nothing to do"
}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/framework.sh $@


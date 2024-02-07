## @file
#
#  Copyright 2006 - 2012 Unified EFI, Inc.<BR>
#  Copyright (c) 2011 - 2012, 2021, 2024, Arm Ltd. All rights reserved.<BR>
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at 
#  http://opensource.org/licenses/bsd-license.php
# 
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
# 
##
#/*++
#
# Module Name:
#
#   makefile
#
# Abstract:
#
#   This is the makefile for creating a private/public keypair for Secure Boot testing.
#
#--*/

BASE_NAME=SecureBoot
TARGET=$(BIN_DIR)/$(BASE_NAME)
SOURCE_DIR=$(WORKSPACE)/SctPkg/TestCase/UEFI/EFI/RuntimeServices/SecureBoot/BlackBoxTest/Dependency/Images
FUTURE_DATE=$(shell date --rfc-3339=date -d "+5 year")
FUTURE_DATE2=$(shell date --rfc-3339=date -d "+1 day")
FUTURE_DATE3=$(shell date --rfc-3339=date -d "+2 day")
FUTURE_DATE4=$(shell date --rfc-3339=date -d "+3 day")
PAST_DATE=$(shell date --rfc-3339=date -d "-1 year")

ifdef KEYS_DIR
TEST_KEK1_CRT=$(KEYS_DIR)/TestKEK1.crt
TEST_DB1_CRT=$(KEYS_DIR)/TestDB1.crt
TEST_DB1_KEY=$(KEYS_DIR)/TestDB1.key
TEST_DBX1_CRT=$(KEYS_DIR)/TestDBX1.crt
TEST_KEK1_KEY=$(KEYS_DIR)/TestKEK1.key
TEST_PK1_CRT=$(KEYS_DIR)/TestPK1.crt
TEST_PK1_KEY=$(KEYS_DIR)/TestPK1.key
else
TEST_KEYS=TestPK1 TestKEK1 TestDB1 TestDBX1
TEST_KEK1_CRT=$(BIN_DIR)/SecureBoot_TestKEK1.crt
TEST_KEK1_KEY=$(BIN_DIR)/SecureBoot_TestKEK1.key
TEST_PK1_CRT=$(BIN_DIR)/SecureBoot_TestPK1.crt
TEST_PK1_KEY=$(BIN_DIR)/SecureBoot_TestPK1.key
endif

all: $(TEST_KEYS) TestKEK2 TestKEK3 KEKSigList1.auth KEKSigList3.auth TestImage1.bin TestImage2.bin TestImage3.bin TestImage4.bin TestImage5.bin TestImage6.bin TestImage7.bin TestImage8.bin TestImage9.bin TestImage10.bin NullKEK.auth NullDB.auth NullDBX.auth TestKEK1.auth TestDB1.auth TestDBX1.auth SignCert1 dbSigList1.auth SignCert2 SignCert3 SignCert4 RevokedCert1 RevokedCert2 RevokedCert3 RevokedCert4 dbSigList2.auth dbSigList3.auth dbSigList4.auth dbxRevokedList1.auth dbSigList5.auth unsignedKeyUpdate TestImage19.bin TestImage20.bin Image19BCert.auth

TestImage1.bin:
	cp $(SOURCE_DIR)/Application1.efi $(TARGET)_$(@)

TestImage2.bin: SignCert1
	cp $(SOURCE_DIR)/Application1.efi $(TARGET)_$(@)
	sbsign --key $(TARGET)_SignCert1.key --cert $(TARGET)_SignCert1.crt $(TARGET)_$(@) --output $(TARGET)_$(@)

TestImage3.bin: SignCert2
	cp $(SOURCE_DIR)/Application2.efi $(TARGET)_$(@)
	sbsign --key $(TARGET)_SignCert2.key --cert $(TARGET)_SignCert2.crt $(TARGET)_$(@) --output $(TARGET)_$(@)

TestImage4.bin: SignCert3
	cp $(SOURCE_DIR)/Application2.efi $(TARGET)_$(@)
	sbsign --key $(TARGET)_SignCert3.key --cert $(TARGET)_SignCert3.crt $(TARGET)_$(@) --output $(TARGET)_$(@)

TestImage5.bin:
	cp $(SOURCE_DIR)/Application2.efi $(TARGET)_$(@)

TestImage6.bin: RevokedCert1
	cp $(SOURCE_DIR)/Application2.efi $(TARGET)_$(@)
	sbsign --key $(TARGET)_RevokedCert1.key --cert $(TARGET)_RevokedCert1.crt $(TARGET)_$(@) --output $(TARGET)_$(@)

TestImage7.bin: RevokedCert2
	cp $(SOURCE_DIR)/Application2.efi $(TARGET)_$(@)
	sbsign --key $(TARGET)_RevokedCert2.key --cert $(TARGET)_RevokedCert2.crt $(TARGET)_$(@) --output $(TARGET)_$(@)

TestImage8.bin: RevokedCert3
	cp $(SOURCE_DIR)/Application2.efi $(TARGET)_$(@)
	sbsign --key $(TARGET)_RevokedCert3.key --cert $(TARGET)_RevokedCert3.crt $(TARGET)_$(@) --output $(TARGET)_$(@)

TestImage9.bin: RevokedCert4
	cp $(SOURCE_DIR)/Application2.efi $(TARGET)_$(@)
	sbsign --key $(TARGET)_RevokedCert4.key --cert $(TARGET)_RevokedCert4.crt $(TARGET)_$(@) --output $(TARGET)_$(@)

TestImage10.bin: SignCert3
	cp $(SOURCE_DIR)/Application3.efi $(TARGET)_Unsigned_$(@).tmp
	cp $(SOURCE_DIR)/Application3.efi $(TARGET)_$(@)
	sbsign --key $(TARGET)_SignCert3.key --cert $(TARGET)_SignCert3.crt $(TARGET)_$(@) --output $(TARGET)_$(@)

TestImage19.bin: Image19ACert Image19BCert
	cp $(SOURCE_DIR)/Application2.efi $(TARGET)_$(@)
	sbsign --key $(TARGET)_Image19ACert.key --cert $(TARGET)_Image19ACert.crt $(TARGET)_$(@) --output $(TARGET)_$(@)
	sbsign --key $(TARGET)_Image19BCert.key --cert $(TARGET)_Image19BCert.crt $(TARGET)_$(@) --output $(TARGET)_$(@)

TestImage20.bin: Image20ACert Image20BCert
	cp $(SOURCE_DIR)/Application2.efi $(TARGET)_$(@)
	sbsign --key $(TARGET)_Image20ACert.key --cert $(TARGET)_Image20ACert.crt $(TARGET)_$(@) --output $(TARGET)_$(@)
	sbsign --key $(TARGET)_Image20BCert.key --cert $(TARGET)_Image20BCert.crt $(TARGET)_$(@) --output $(TARGET)_$(@)

TestPK1:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_TEST_PK/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000
	openssl x509 -outform der -in $(TARGET)_$(@).crt -out $(TARGET)_$(@).der

TestKEK1:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_TEST_PK/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000
	openssl x509 -outform der -in $(TARGET)_$(@).crt -out $(TARGET)_$(@).der

TestDB1:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_TEST_DB/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000
	openssl x509 -outform der -in $(TARGET)_$(@).crt -out $(TARGET)_$(@).der

TestDBX1:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_TEST_DB/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000
	openssl x509 -outform der -in $(TARGET)_$(@).crt -out $(TARGET)_$(@).der

TestKEK2:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_TEST_KEK/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

TestKEK3:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_TEST_KEK3/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

SignCert1:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_SignCert1/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

SignCert2:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_SignCert2/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

SignCert3:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_SignCert3/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

SignCert4:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_SignCert4/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

Sign_Cert5:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_Sign_Cert3/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

RevokedCert1:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_RevokedCert1/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

RevokedCert2:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_RevokedCert2/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

RevokedCert3:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_RevokedCert3/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

RevokedCert4:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_RevokedCert4/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

Image19ACert:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_Image19ACert/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

Image19BCert:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_Image19BCert/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

Image20ACert:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_Image20ACert/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

Image20BCert:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_Image20BCert/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000

KEKSigList1.auth: TestKEK2
	cert-to-efi-sig-list $(TEST_KEK1_CRT) $(BIN_DIR)/SecureBoot_TestKEK1.esl
	cert-to-efi-sig-list $(BIN_DIR)/SecureBoot_TestKEK2.crt $(BIN_DIR)/SecureBoot_TestKEK2.esl
	cat $(BIN_DIR)/SecureBoot_TestKEK1.esl $(BIN_DIR)/SecureBoot_TestKEK2.esl > $(BIN_DIR)/SecureBoot_KEKSigList1.esl
	sign-efi-sig-list -c $(TEST_PK1_CRT) -k $(TEST_PK1_KEY) KEK $(BIN_DIR)/SecureBoot_KEKSigList1.esl $(BIN_DIR)/SecureBoot_KEKSigList1.auth

NullKEK.auth:
	cat /dev/null > $(BIN_DIR)/SecureBoot_NullKEK.esl
	sign-efi-sig-list -t $(FUTURE_DATE) -c $(TEST_PK1_CRT) -k $(TEST_PK1_KEY) KEK $(BIN_DIR)/SecureBoot_NullKEK.esl $(BIN_DIR)/SecureBoot_NullKEK.auth

NullDB.auth:
	cat /dev/null > $(BIN_DIR)/SecureBoot_NullDB.esl
	sign-efi-sig-list -t $(FUTURE_DATE) -c $(TEST_PK1_CRT) -k $(TEST_PK1_KEY) db $(BIN_DIR)/SecureBoot_NullDB.esl $(BIN_DIR)/SecureBoot_NullDB.auth

NullDBX.auth:
	cat /dev/null > $(BIN_DIR)/SecureBoot_NullDBX.esl
	sign-efi-sig-list -t $(FUTURE_DATE) -c $(TEST_PK1_CRT) -k $(TEST_PK1_KEY) dbx $(BIN_DIR)/SecureBoot_NullDBX.esl $(BIN_DIR)/SecureBoot_NullDBX.auth

TestKEK1.auth:
	cert-to-efi-sig-list $(TEST_KEK1_CRT) $(BIN_DIR)/SecureBoot_TestKEK1.esl
	sign-efi-sig-list -t $(PAST_DATE) -c $(TEST_PK1_CRT) -k $(TEST_PK1_KEY) KEK $(BIN_DIR)/SecureBoot_TestKEK1.esl $(BIN_DIR)/SecureBoot_TestKEK1.auth

TestDB1.auth:
	cert-to-efi-sig-list $(TEST_DB1_CRT) $(BIN_DIR)/SecureBoot_TestDB1.esl
	sign-efi-sig-list -t $(PAST_DATE) -c $(TEST_PK1_CRT) -k $(TEST_PK1_KEY) db $(BIN_DIR)/SecureBoot_TestDB1.esl $(BIN_DIR)/SecureBoot_TestDB1.auth

TestDBX1.auth:
	cert-to-efi-sig-list $(TEST_DBX1_CRT) $(BIN_DIR)/SecureBoot_TestDBX1.esl
	sign-efi-sig-list -t $(PAST_DATE) -c $(TEST_PK1_CRT) -k $(TEST_PK1_KEY) dbx $(BIN_DIR)/SecureBoot_TestDBX1.esl $(BIN_DIR)/SecureBoot_TestDBX1.auth

KEKSigList3.auth: TestKEK3
	cert-to-efi-sig-list $(BIN_DIR)/SecureBoot_TestKEK3.crt $(BIN_DIR)/SecureBoot_KEKSigList3.esl
	sign-efi-sig-list -a -t $(FUTURE_DATE2) -c $(TEST_PK1_CRT) -k $(TEST_PK1_KEY) KEK $(BIN_DIR)/SecureBoot_KEKSigList3.esl $(BIN_DIR)/SecureBoot_KEKSigList3.auth

dbSigList1.auth: SignCert1
	cert-to-efi-sig-list $(BIN_DIR)/SecureBoot_SignCert1.crt $(BIN_DIR)/SecureBoot_TestDB1.esl
	sign-efi-sig-list -c $(TEST_PK1_CRT) -k $(TEST_PK1_KEY) db $(BIN_DIR)/SecureBoot_TestDB1.esl $(BIN_DIR)/SecureBoot_DBSigList1.auth

dbSigListLong: SignCert2 SignCert3 RevokedCert1 RevokedCert2 RevokedCert3 RevokedCert4 TestImage5.bin Image19ACert Image20ACert
	cert-to-efi-sig-list -g 11111111-2222-3333-4444-000000000002 $(BIN_DIR)/SecureBoot_SignCert2.crt $(BIN_DIR)/SecureBoot_TestDB2.esl
	cert-to-efi-sig-list -g 11111111-2222-3333-4444-000000000003 $(BIN_DIR)/SecureBoot_SignCert3.crt $(BIN_DIR)/SecureBoot_TestDB3.esl
	cert-to-efi-sig-list -g 11111111-2222-3333-4444-0000000000C1 $(BIN_DIR)/SecureBoot_RevokedCert1.crt $(BIN_DIR)/SecureBoot_TestDB4.esl
	cert-to-efi-sig-list -g 11111111-2222-3333-4444-0000000000C2 $(BIN_DIR)/SecureBoot_RevokedCert2.crt $(BIN_DIR)/SecureBoot_TestDB5.esl
	cert-to-efi-sig-list -g 11111111-2222-3333-4444-0000000000C3 $(BIN_DIR)/SecureBoot_RevokedCert3.crt $(BIN_DIR)/SecureBoot_TestDB6.esl
	cert-to-efi-sig-list -g 11111111-2222-3333-4444-0000000000C4 $(BIN_DIR)/SecureBoot_RevokedCert4.crt $(BIN_DIR)/SecureBoot_TestDB7.esl
	cert-to-efi-sig-list -g 11111111-2222-3333-4444-0000000000C4 $(BIN_DIR)/SecureBoot_Image19ACert.crt $(BIN_DIR)/SecureBoot_Image19ACert.esl
	cert-to-efi-sig-list -g 11111111-2222-3333-4444-0000000000C4 $(BIN_DIR)/SecureBoot_Image20ACert.crt $(BIN_DIR)/SecureBoot_Image20ACert.esl
	hash-to-efi-sig-list $(BIN_DIR)/SecureBoot_TestImage5.bin $(BIN_DIR)/SecureBoot_Hash1.esl
	cat $(BIN_DIR)/SecureBoot_TestDB2.esl $(BIN_DIR)/SecureBoot_TestDB3.esl $(BIN_DIR)/SecureBoot_TestDB4.esl $(BIN_DIR)/SecureBoot_TestDB5.esl $(BIN_DIR)/SecureBoot_TestDB6.esl  $(BIN_DIR)/SecureBoot_TestDB7.esl $(BIN_DIR)/SecureBoot_Image19ACert.esl $(BIN_DIR)/SecureBoot_Image20ACert.esl $(BIN_DIR)/SecureBoot_Hash1.esl  > $(BIN_DIR)/SecureBoot_DBSigListLong.esl

dbSigList2.auth: dbSigListLong
	sign-efi-sig-list -c $(TEST_KEK1_CRT) -k $(TEST_KEK1_KEY) -t "$(FUTURE_DATE2)" db $(BIN_DIR)/SecureBoot_DBSigListLong.esl $(BIN_DIR)/SecureBoot_DBSigList2.auth

dbSigList3.auth: dbSigListLong
	sign-efi-sig-list -c $(TEST_KEK1_CRT) -k $(TEST_KEK1_KEY) -t "$(FUTURE_DATE2)" dbx $(BIN_DIR)/SecureBoot_DBSigListLong.esl $(BIN_DIR)/SecureBoot_DBSigList3.auth

dbSigList4.auth: SignCert4
	cert-to-efi-sig-list $(BIN_DIR)/SecureBoot_SignCert4.crt $(BIN_DIR)/SecureBoot_TestDB9.esl
	sign-efi-sig-list -c $(BIN_DIR)/SecureBoot_TestKEK2.crt -k $(BIN_DIR)/SecureBoot_TestKEK2.key -t "$(FUTURE_DATE3)" db $(BIN_DIR)/SecureBoot_TestDB9.esl $(BIN_DIR)/SecureBoot_DBSigList4.auth

dbxRevokedList1.auth: RevokedCert1 RevokedCert2 RevokedCert3 RevokedCert4 Image20BCert
	cert-to-efi-hash-list -s 256 $(BIN_DIR)/SecureBoot_RevokedCert1.crt $(BIN_DIR)/SecureBoot_RevokedCert1.esl
	cert-to-efi-hash-list -s 384 $(BIN_DIR)/SecureBoot_RevokedCert2.crt $(BIN_DIR)/SecureBoot_RevokedCert2.esl
	cert-to-efi-hash-list -s 512 $(BIN_DIR)/SecureBoot_RevokedCert3.crt $(BIN_DIR)/SecureBoot_RevokedCert3.esl
	cert-to-efi-hash-list -s 256 $(BIN_DIR)/SecureBoot_Image20BCert.crt $(BIN_DIR)/SecureBoot_Image20BCert.esl
	cert-to-efi-sig-list $(BIN_DIR)/SecureBoot_RevokedCert4.crt $(BIN_DIR)/SecureBoot_RevokedCert4.esl
	hash-to-efi-sig-list $(BIN_DIR)/SecureBoot_Unsigned_TestImage10.bin.tmp $(BIN_DIR)/SecureBoot_RevokedHash1.esl
	cat $(BIN_DIR)/SecureBoot_RevokedCert1.esl $(BIN_DIR)/SecureBoot_RevokedCert2.esl $(BIN_DIR)/SecureBoot_RevokedCert3.esl  $(BIN_DIR)/SecureBoot_RevokedCert4.esl $(BIN_DIR)/SecureBoot_RevokedHash1.esl $(BIN_DIR)/SecureBoot_Image20BCert.esl > $(BIN_DIR)/SecureBoot_dbxRevokedList1.esl
	sign-efi-sig-list -c $(TEST_KEK1_CRT) -k $(TEST_KEK1_KEY) -t "$(FUTURE_DATE2)" dbx $(BIN_DIR)/SecureBoot_dbxRevokedList1.esl $(BIN_DIR)/SecureBoot_dbxRevokedList1.auth

dbSigList5.auth: Sign_Cert5
	cert-to-efi-sig-list $(BIN_DIR)/SecureBoot_Sign_Cert5.crt $(BIN_DIR)/SecureBoot_TestDB10.esl
	sign-efi-sig-list -a -c $(BIN_DIR)/SecureBoot_TestKEK3.crt -k $(BIN_DIR)/SecureBoot_TestKEK3.key -t "$(FUTURE_DATE4)" db $(BIN_DIR)/SecureBoot_TestDB10.esl $(BIN_DIR)/SecureBoot_DBSigList5.auth

Image19BCert.auth: Image19BCert
	cert-to-efi-sig-list $(BIN_DIR)/SecureBoot_Image19BCert.crt $(BIN_DIR)/SecureBoot_Image19BCert.esl
	sign-efi-sig-list -c $(TEST_KEK1_CRT) -k $(TEST_KEK1_KEY) -t "$(FUTURE_DATE3)" db $(BIN_DIR)/SecureBoot_Image19BCert.esl $(BIN_DIR)/SecureBoot_Image19BCert.auth

unsignedKeyUpdate:
	openssl req -x509 -sha256 -newkey rsa:2048 -subj /CN=ACS_UNSIGNED_KEY/ -keyout $(TARGET)_$(@).key -out $(TARGET)_$(@).crt -nodes -days 4000
	cert-to-efi-sig-list $(TARGET)_$(@).crt $(TARGET)_$(@).esl
	# exporting a .auth file without signing it with PK or KEK for testing purposes
	mv $(TARGET)_$(@).esl $(TARGET)_$(@).auth

clean:
	$(RM) $(BIN_DIR)/$(TARGET)_*.key
	$(RM) $(BIN_DIR)/$(TARGET)_*.crt
	$(RM) $(BIN_DIR)/$(TARGET)_*.der

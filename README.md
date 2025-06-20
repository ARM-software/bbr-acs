# Base Boot Requirements - Architecture Compliance Suite
BBR specification complements the BSA specification by defining the base firmware requirements
required for out-of-box support of any BSA compatible operating system or hypervisor. These requirements are comprehensive enough
to enable booting multi-core Arm platforms while remaining minimal enough to allow for OEM and ODM innovation, and
market differentiation.

For more information, see the [BBR specification](https://developer.arm.com/documentation/den0044/g/?lang=en).

The BBR test suites check for compliance against the SBBR, EBBR and BBSR specifications. These tests are also delivered through two runtime executable environments:
  - UEFI Self Certification Tests (SCT)
  - Firmware Test Suite (FWTS)

## Release details

- SBBR test component
  - Version: v2.1.1
  - Code Quality: BET
  - The SBBR tests are written for the SBBR recipe of the BBR v2.1 specification.
 
- EBBR test component
  - Version: v2.2.1
  - Code Quality: BET
  - The EBBR tests are written for the EBBR recipe of the BBR v2.1 specification.
 
- BBSR test component
  - Version: v1.3.0
  - Code Quality: EAC
  - The BBSR tests are written for the BBSR v1.3 specification.

- The compliance suite is not a substitute for design verification.
- To review the ACS logs, Arm licensees can contact Arm directly through their partner managers.

  NOTE: [SystemReady ACS](https://github.com/ARM-software/arm-systemready) depends on the bbr acs for packaging the SCT tests in the SystemReady ACS.
  Mapping of BBR ACS tags with SystemReady Release version are captured [here](#bbr-acs-tag-mapping-to-systemready-acs-releases)

## UEFI Self Certification Tests
UEFI SCT tests the UEFI implementation requirements defined by SBBR/EBBR.

**Prerequisite**: Ensure that the system time is correct before starting SCT tests.

## Building SCT
BBR is automatically built and packaged into ACS, but it can also be built independently.

#### 1.  Get BBR repository
`git clone https://github.com/ARM-software/bbr-acs.git`

#### 2. Getting the required Source codes and Tools
Navigate to the `bbr-acs/<ebbr/sbbr>/scripts` directory

Get source by running the
`./build-scripts/get_<ebbr/sbbr>_source.sh`

This will download `edk2-test, edk2 and tools`

#### 3. Building SBBR & EBBR
 Run
`./build-scripts/build_<ebbr/sbbr>.sh`
to build BBR components and SCT

The script applies patches to create an "EBBR or SBBR" build recipe in the SCT build system.

The binaries of SCT are generated here
    `bbr-acs/<ebbr/sbbr>/scripts/edk2-test/uefi-sct/<ARCH>_SCT #(i.e. AARCH64_SCT)`

NOTE: The UEFI application, CapsuleApp.efi is also built and can be found at the location bbr-acs/<ebbr/sbbr>/scripts/edk2/Build/MdeModule/DEBUG_GCC5/AARCH64 <br />

### Running SCT
BBR SCT tests are built as part of the test suite. <br />

Running BBR SCT tests is now automated. You can choose to skip the automated SCT tests by pressing any key when the UEFI shell prompts.

- Shell>Press any key to stop the EFI SCT running

To run SCT manually, follow these steps:


1. `Shell>FS(X):`
- `FS(X):>cd acs_tests\bbr\SCT`
- To run EBBR or SBBR tests
 `FS(X):acs_tests\bbr\SCT>SCT -s <ebbr.seq/sbbr.seq>`
 - To run all tests
 `FS(X):acs_tests\bbr\SCT>SCT -a -v`
 
You can also select and run tests individually. For more information on running the tests, see the [SCT User Guide](http://www.uefi.org/testtools).

#### Note:

> For BBR ACS UEFI-SCT testlists for SBBR, EBBR and BBSR recipes, see the [Documentation directory](./docs/).

### Manual intervention tests
Some SCT tests for the ACS require manual intervention or interaction.
To run the tests, follow these steps.

1. Move or copy the SCT logs into the result partition so they do not get overwritten.

	- `Shell>FS(Y):`
	- `FS(Y):> cd \acs_results\`
	- `FS(Y):\acs_results\> mv sct_results sct_results_orginal`

2. To run manual tests for ebbr, use the ebbr_manual.seq file.

	- `FS(X):acs_tests\bbr\SCT>SCT -s ebbr_manual.seq`
 
3. While the system runs the reset tests, you may have to manually reset the system if it hangs.

	**Note:** The logs for the manual tests will overwrite the logs for the original test run which is the reason to have a copy of the original test. This new folder contains the logs from the new tests run in the manual sequence file. You may to concatenate some 	of the logs to view together.

4. To run additional SCT tests (e.g., Simple Filesystems, Block I/O, etc.) to improve more Compliances, use the EBBR_extd_run.seq or SBBR_extd_run.seq file.
   
	- `FS(X):acs_tests\bbr\SCT>SCT -s <EBBR_extd_run.seq/SBBR_extd_run.seq>`


## Firmware Test Suite
FWTS is a package hosted by Canonical. FWTS provides tests for ACPI, SMBIOS and UEFI.
Several SBBR/EBBR assertions are tested through FWTS.

## Building Standalone FWTS
Run	`./common/scripts/build-standalone-fwts.sh` to build FWTS component

The FWTS binaries and dependencies can be found here
-	`fwts_workspace/buildroot/output/target/usr/bin/fwts`
-	`fwts_workspace/buildroot/output/target/usr/bin/kernelscan`
-	`fwts_workspace/buildroot/output/target/usr/lib64/fwts/`
-	`fwts_workspace/buildroot/output/target/usr/lib/fwts/`
-	`fwts_workspace/buildroot/output/target/usr/share/fwts/`

### Run FWTS Binary

1.  Boot to Target OS

2.  Create fwts workspace on Target OS
    -	`mkdir -p ~/fwts_workspace/bin`
    -	`mkdir -p ~/fwts_workspace/lib`

3.  Copy FWTS Binary and Dependencies to fwts workspace
    -   Copy the fwts binary to ~/fwts_workspace/bin
    -   Copy all required shared libraries (e.g., libfwts.so, etc.) to ~/fwts_workspace/lib

4.  Set Library Path
    -	`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/fwts_workspace/lib`

5.  Run fwts to check version
    -	`~/fwts_workspace/bin/fwts --version`

6.  Run full FWTS tests
    -	`~/fwts_workspace/bin/fwts`

### Notes
- By default, build-standalone-fwts.sh script builds the latest FWTS version supprted by ARM SystemReady, as defined in the upstream configuration.

- To build a specific version of FWTS supported by Arm SystemReady, you can set the USER_DEFINED_FWTS_VERSION variable in the build-standalone-fwts.sh script.
    The currently supported versions are:
    - 25.10.00
    - 24.09.00
    - 24.03.00
    - 24.01.00
    - 23.07.00
    - 22.11.00

- To apply custom patches to FWTS, place your patch files in the following directory:
  ```plaintext
  buildroot/package/fwts/
  ```

## BBR ACS Tag mapping to SystemReady ACS Releases
---------------------------------------------------------------------
|    BBR ACS TAG        |     BBR Recipe      | SystemReady Release |
|-----------------------|:-------------------:|:-------------------:|
|   v25.04_SR_3.0.1     |       SBBR          |  v25.04_SR_3.1.0    |
---------------------------------------------------------------------

## License
 
Arm BBR ACS is distributed under Apache v2.0 License.


## Feedback, contributions and support

 - For feedback, use the GitHub Issue Tracker that is associated with this repository.
 - For support, send an email to "support-systemready-acs@arm.com" with details.
 - Arm licensees may contact Arm directly through their partner managers.
 - Arm welcomes code contributions through GitHub pull requests. See the GitHub documentation on how to raise pull requests.

--------------

*Copyright (c) 2021-2025, Arm Limited and Contributors. All rights reserved.*

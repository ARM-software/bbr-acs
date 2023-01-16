# Base Boot Requirements - Architecture Compliance Suite
BBR specification complements the BSA specification by defining the base firmware requirements
required for out-of-box support of any BSA compatible operating system or hypervisor. These requirements are comprehensive enough
to enable booting multi-core Arm platforms while remaining minimal enough to allow for OEM and ODM innovation, and
market differentiation.

For more information, see the [BBR specification](https://developer.arm.com/documentation/den0044/f/?lang=en).

The BBR test suites check for compliance against the SBBR and EBBR specifications. These tests are also delivered through two runtime executable environments:
  - UEFI Self Certification Tests (SCT)
  - Firmware Test Suite (FWTS)

## Release details
- Code Quality: v1.0.3
- The BBR tests are written for version 1.0 of the BBR specification.
- The compliance suite is not a substitute for design verification.
- To review the ACS logs, Arm licensees can contact Arm directly through their partner managers.

## UEFI Self Certification Tests
UEFI SCT tests the UEFI implementation requirements defined by SBBR/EBBR.

**Prerequisite**: Ensure that the system time is correct before starting SCT tests.

### Running SCT
BBR SCT tests are built as part of the test suite. <br />

Running BBR SCT tests is now automated. You can choose to skip the automated SCT tests by pressing any key when the UEFI shell prompts.

- Shell>Press any key to stop the EFI SCT running

To run SCT manually, follow these steps:


1. `Shell>FS(X):`
- `FS(X):>cd EFI\BOOT\bbr\SCT`
- To run EBBR or SBBR tests
 `FS(X):EFI\BOOT\bbr\SCT>SCT -s <ebbr.seq/sbbr.seq>`
 - To run all tests
 `FS(X):EFI\BOOT\bbr\SCT>SCT -a -v`
 

You can also select and run tests individually. For more information on running the tests, see the [SCT User Guide](http://www.uefi.org/testtools).

### Manual intervention tests
Some SCT tests for the ACS require manual intervention or interaction.
To run the tests, follow these steps.

1. Move or copy the SCT logs into the result partition so they do not get overwritten.

	- `Shell>FS(Y):`
	- `FS(Y):> cd \acs_results\`
	- `FS(Y):\acs_results\> mv sct_results sct_results_orginal`



2. Run manual tests.

	- `FS(X):EFI\BOOT\bbr\SCT>SCT -s <ebbr_manual.seq/sbbr_manual.seq>`
 
3. While the system runs the reset tests, you may have to manually reset the system if it hangs.

**Note:** The logs for the manual tests will overwrite the logs for the original test run which is the reason to have a copy of the original test. This new folder contains the logs from the new tests run in the manual sequence file. You may to concatenate some of the logs to view together.



## SBBR based on Firmware Test Suite
FWTS is a package hosted by Canonical. FWTS provides tests for ACPI, SMBIOS and UEFI.
Several SBBR assertions are tested through FWTS.

### Running FWTS tests

From the UEFI shell, you can choose to boot Linux OS by entering the command:

`Shell>exit`

This command loads the grub menu. Press enter to choose the option 'Linux BusyBox' that boots the OS and runs FWTS tests and OS context BSA tests automatically. <br />

Logs are stored in the results partition, which can be viewed on any machine after the tests are run.


## Building BBR
BBR is automatically built and packaged into ACS, but it can also be built independently.

#### 1.  Get BBR repository
`git clone https://github.com/ARM-software/bbr-acs.git`

#### 2. Getting the required Source codes and Tools
Navigate to the `bbr-acs/<ebbr/sbbr>/scripts` directory

Get source by running the
`./build-scripts/get_<ebbr/sbbr>_source.sh`

This will download `edk2-test, edk2, fwts and tools`

#### 3. Building SBBR & EBBR
 Run
`./build-scripts/build_<ebbr/sbbr>.sh`
to build BBR components, SCT and FWTS.

The script applies patches to create an "EBBR or SBBR" build recipe in the SCT and FWTS build system.

The binaries of SCT are generated here
    `bbr-acs/<ebbr/sbbr>/scripts/edk2-test/uefi-sct/<ARCH>_SCT #(i.e. AARCH64_SCT)`

The binaries of FWTS are generated here
    `bbr-acs/<ebbr/sbbr>/scripts/fwts/fwts_output`

NOTE: The UEFI application, CapsuleApp.efi is also built and can be found at the location bbr-acs/<ebbr/sbbr>/scripts/edk2/Build/MdeModule/DEBUG_GCC5/AARCH64 <br />

## License
 
Arm BBR ACS is distributed under Apache v2.0 License.


## Feedback, contributions and support

 - For feedback, use the GitHub Issue Tracker that is associated with this repository.
 - For support, send an email to "support-systemready-acs@arm.com" with details.
 - Arm licensees may contact Arm directly through their partner managers.
 - Arm welcomes code contributions through GitHub pull requests. See the GitHub documentation on how to raise pull requests.

--------------

*Copyright (c) 2021-2023, Arm Limited and Contributors. All rights reserved.*

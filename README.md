## Table of Contents
- [Overview](#base-boot-requirements--architecture-compliance-suite-bbr-acs)
- [SBBR](#sbbr)
- [EBBR](#ebbr)
- [BBSR](#bbsr)
- [Latest Release Details](#latest-release-details)
- [UEFI Self Certification Tests](#uefi-self-certification-tests)
- [Building SCT](#building-sct)
- [Running SCT Manually Using Recipe Sequence File](#running-sct-manually-using-recipe-sequence-file)
  - [Additional SCT Tests](#additional-sct-tests)
  - [Manual Intervention Tests for EBBR](#manual-intervention-tests-for-ebbr)
- [Running Standalone SCT](#running-standalone-sct)
- [Firmware Test Suite (FWTS)](#firmware-test-suite-fwts)
- [Building Standalone FWTS](#building-standalone-fwts)
- [Run FWTS Binary](#run-fwts-binary)
  - [Running FWTS for Different Recipes](#running-fwts-for-different-recipes)
- [License](#license)
- [Feedback, Contributions, and Support](#feedback-contributions-and-support)

# Base Boot Requirements – Architecture Compliance Suite (BBR-ACS)

The **Base Boot Requirements (BBR)** specification complements the **Base System Architecture (BSA)** by defining the **base firmware** requirements needed for out‑of‑box support of any BSA‑compatible operating system or hypervisor. These requirements are comprehensive enough to enable booting multi‑core Arm platforms, while remaining minimal to allow OEM/ODM innovation and market differentiation.

The **BBR‑ACS** test suites check for compliance against the **SBBR**, **EBBR**, and **BBSR** specifications. The tests are delivered via two runtime executable environments:

- **UEFI Self Certification Tests (UEFI‑SCT)**
- **Firmware Test Suite (FWTS)**

UEFI‑SCT and FWTS are leveraged, customized with Arm‑specific checks, and automated to run within **Arm SystemReady ACS**. Collectively, these form the **Arm BBR‑ACS** within the Arm compliance test ecosystem.

---

## SBBR

The **SBBR** specification defines a standard firmware and boot environment for Arm server‑class platforms. It ensures that a single operating system image can run on all compliant hardware, promoting out‑of‑box compatibility across vendors and implementations.

SBBR builds on the **Arm Base System Architecture (BSA)** and specifies standardized interfaces such as:

- **UEFI** (Unified Extensible Firmware Interface)
- **ACPI** (Advanced Configuration and Power Interface)
- **SMBIOS** (System Management BIOS)
- **PSCI** and **SMCCC** (for power and system control/management)

SBBR platforms typically use UEFI firmware (e.g., EDK2), but compliance does not require a specific implementation. Operating systems such as Windows Server, RHEL, SUSE Linux Enterprise Server, Ubuntu, and VMware ESXi support SBBR compliance.

For more information, see the **SBBR** section in the **[BBR specification](https://developer.arm.com/documentation/den0044/latest)**.

---

## EBBR

The **EBBR** specification defines a streamlined, embedded‑friendly firmware environment for Arm platforms. It provides a reduced UEFI implementation suitable for embedded systems while maintaining a consistent boot interface for OSes and hypervisors.

EBBR platforms typically:

- Implement a minimal UEFI environment (often using U‑Boot, though EDK2 is also valid)
- Support UEFI runtime and boot services needed for embedded use
- May include an optional device description via **Devicetree (DT)**

EBBR ensures embedded devices can benefit from standard firmware interfaces while remaining lightweight. Typical supported operating systems include Fedora, Debian, openSUSE/SLES, and Ubuntu.

For more information, see the **[BBR specification](https://developer.arm.com/documentation/den0044/latest)** and **[BBSR specification](https://developer.arm.com/documentation/den0107/latest/)**.

---

## BBSR

The **BBSR** specification describes security requirements that a device must implement to comply with industry‑standard security interfaces, including:

- UEFI authenticated variables
- UEFI Secure Boot
- UEFI capsule updates
- TPM 2.0 and measured boot

> **Note:** Interface compliance does not by itself guarantee that the underlying platform is secure. System‑level threat modeling should be performed to evaluate threats, risks, and mitigations.

For more information, see the **[BBSR specification](https://developer.arm.com/documentation/den0107/latest/)**.

---

## Latest Release Details

| Recipe       | Version | Code Quality | Specification |
|-------------------|:----------:|:-------------------:| :-------------------: |
| SBBR | v2.1.2 | BET | Based on SBBR recipe of BBR v2.1 |
| EBBR | v2.2.2 | BET | Based on EBBR recipe of BBR v2.1 and EBBR v2.2|
| BBSR | v1.3.1 | EAC | Written for BBSR v1.3 |

- The compliance suite is **not** a substitute for design verification.
- To review ACS logs, Arm licensees can contact Arm directly via their partner managers.

**Note:** [SystemReady ACS](https://github.com/ARM-software/arm-systemready) depends on BBR‑ACS for packaging SCT tests. Mapping of BBR‑ACS tags to SystemReady release versions is captured below.

### BBR‑ACS Tag Mapping to SystemReady ACS Releases

| BBR‑ACS Tag       | BBR Recipe | SystemReady Release |
|-------------------|:----------:|:-------------------:|
| `v25.12_SBBR_2.1.2` |   SBBR     |    `v25.12_DT_3.1.1` |
| `v25.12_EBBR_2.2.2` |   EBBR     |    `v25.12_DT_3.1.1` |
| `v25.12_BBSR_1.3.1` |   BBSR     |    `v25.12_DT_3.1.1` |

---

## UEFI Self Certification Tests

- **UEFI‑SCT** tests the UEFI implementation requirements defined by the SBBR/EBBR recipes in the BBR specification and the rules in BBSR.
- UEFI‑SCT is customized with additional tests that perform Arm‑specific checks and assertions. The suite is executed using specific sequence files.
- A **sequence file** is a configuration consumed by UEFI‑SCT that defines the selection of tests to run.

| **Recipe** | **Sequence File** | **Documentation** |
| --- | --- | --- |
| SBBR | [`SBBR.seq`](https://github.com/ARM-software/bbr-acs/blob/main/sbbr/config/SBBR.seq) | [`SBBR_recipe_testlist.md`](https://github.com/ARM-software/bbr-acs/blob/main/docs/SBBR_recipe_testlist.md) |
| EBBR | [`EBBR.seq`](https://github.com/ARM-software/bbr-acs/blob/main/ebbr/config/EBBR.seq) | [`EBBR_recipe_testlist.md`](https://github.com/ARM-software/bbr-acs/blob/main/docs/EBBR_recipe_testlist.md) |
| BBSR | [`BBSR.seq`](https://github.com/ARM-software/bbr-acs/blob/main/bbsr/config/BBSR.seq) | [`BBSR_recipe_testlist.md`](https://github.com/ARM-software/bbr-acs/blob/main/docs/BBSR_recipe_testlist.md) |

> **Prerequisite:** Ensure that the system time is correct before starting SCT tests.

---

## Building SCT

BBR‑ACS is automatically built and packaged into the **SystemReady ACS**, but it can also be built independently.

### 1) Get the BBR‑ACS repository
```bash
git clone https://github.com/ARM-software/bbr-acs.git
```

### 2) Get the required source codes and tools
Navigate to the scripts directory:
```bash
cd bbr-acs/<ebbr|sbbr>/scripts
```
Fetch sources:
```bash
./build-scripts/get_<ebbr/sbbr>_source.sh
```
This downloads `edk2-test`, `edk2`, and required tools.

### 3) Build SBBR & EBBR
Run:
```bash
./build-scripts/build_<ebbr/sbbr>.sh
```
This applies patches to create the **EBBR** or **SBBR** build recipe in the SCT build system and builds BBR‑ACS components and SCT.

SCT binaries are generated at:
```text
bbr-acs/<ebbr|sbbr>/scripts/edk2-test/uefi-sct/<SBBR|EBBR>-SCT   # e.g. SBBR-SCT or EBBR-SCT
```

**Notes:**
- The UEFI application `CapsuleApp.efi` is also built and can be found at:  
  `bbr-acs/<ebbr|sbbr>/scripts/edk2/Build/MdeModule/DEBUG_GCC5/AARCH64`
- The standalone build does not include the BBSR SCT suite, as some BBSR tests depend on the KEYS_DIR. These tests are only supported when built through the arm-systemready repository.

---

## Running Standalone SCT
The Standalone SCT can be launched from any UEFI-accessible storage device such as a USB drive, NVMe device, or SD card.  
This device must contain the SCT binaries, configuration files, and startup scripts required for automated execution.

### Prepare a UEFI-accessible storage device (such as a USB drive, NVMe, or any other supported medium).
1. Connect the storage device (ex:usb) to your computer and format it as FAT32.  
   On Linux, you can create a GPT + FAT32 ESP using the following commands:
   ```bash
   sudo parted /dev/sdX --script mklabel gpt
   sudo parted /dev/sdX --script mkpart BOOT fat32 0MiB 100%
   sudo parted /dev/sdX --script set 1 esp on
   sudo parted /dev/sdX --script set 1 boot on
   sudo mkfs.vfat -F32 /dev/sdX1
   ```
   **Note**:  
   - Replace <device> with the correct drive identifier (for example, /dev/sdb).
   - If the platform already provides access to a UEFI shell, this step may not be required, you can simply use the platform-provided bootaa64.efi to load the SCT directly.
2. On the newly formatted device, create two directories in the root:
   - SCT
   - EFI/BOOT
3. Copy the SCT binaries, required scripts (StandaloneSctStartup.nsh and startup.nsh), and the UEFI bootloader (EFI/BOOT/bootaa64.efi) onto the device.
   These files can be found in the build output at:
   ```text
   bbr-acs/<ebbr|sbbr>/scripts/edk2-test/uefi-sct/<SBBR|EBBR>-SCT   # e.g. SBBR-SCT or EBBR-SCT
   ```
    **Note**:  
   - The provided startup scripts may be used directly or modified as needed to create a Partner-specific startup.nsh
   - <SBBR|EBBR>-SCT also contains Shell.efi (used as bootaa64.efi), which is required to load the standalone SCT into system memory.
4. After copying, the expected directory layout on the target device is:
   ```text
   ├── EFI/
   │   └── BOOT/
   │       └── bootaa64.efi
   ├── SCT
   ├── StandaloneSctStartup.nsh
   └── startup.nsh
   ```
### Run Standalone SCT Using U-Boot
1. Insert the USB/NVMe device into the SUT (System Under Test) that contains:
   - EFI/BOOT/bootaa64.efi
   - startup.nsh
   - StandaloneSctStartup.nsh
   - SCT binaries
2. Set `bootcmd`, so U-Boot loads bootaa64.efi and executes startup.nsh:
   ```bash
   u-boot=> setenv bootcmd 'usb reset; if load usb 0:1 ${loadaddr} EFI/BOOT/bootaa64.efi; then bootefi ${loadaddr} ${fdtcontroladdr}; fi'
   u-boot=> saveenv
   ```
   **Note**:
   - Use usb start or usb reset as appropriate for the platform.
   - Adjust the USB device index (usb 0:1) to match the device containing the SCT files.
3. System boots into UEFI Shell  
   - U-Boot loads bootaa64.efi
   - UEFI Shell starts
   - Shell automatically runs startup.nsh
   - Standalone SCT execution begins
4. Default behavior: startup.nsh invokes StandaloneSctStartup.nsh to run the **SBBR** compliance test suite automatically.  
   **Note**: The system may reboot during SCT execution - this is expected. After each reboot, SCT will continue automatically without user interaction.
5. To run **EBBR** tests instead of SBBR:  
   Modify the `bbr_recipe` value in startup.nsh so that the script calls the **EBBR** variant of StandaloneSctStartup.nsh.
6. The SCT framework writes results to the sct_results directory in the root of the device. The final summary report is available at:  
   ```text
   fsX:\sct_results\Overall\summary.log
   ```
7. Restore the normal boot behavior (after testing).
   ```bash
   u-boot=> env default -a
   u-boot=> saveenv
   ```
   This resets U-Boot to factory default boot order.

### Run Standalone SCT Using UEFI Firmware
1. Insert the USB/NVMe device into the SUT (System Under Test) that contains:
   - EFI/BOOT/bootaa64.efi
   - startup.nsh
   - StandaloneSctStartup.nsh
   - SCT binaries
2. In UEFI Setup (BIOS menu), set the boot priority so that the system boots from the device containing:
   ```bash
   EFI/BOOT/bootaa64.efi
   ```
   This ensures:
   - UEFI Shell launches automatically
   - startup.nsh runs without user input
   - Standalone SCT execution begins
3. Default behavior: startup.nsh invokes StandaloneSctStartup.nsh to run the **SBBR** compliance test suite automatically.  
   **Note**: The system may reboot during SCT execution - this is expected. After each reboot, SCT will continue automatically without user interaction.
4. To run **EBBR** tests instead of SBBR:  
   Modify the `bbr_recipe` value in startup.nsh so that the script calls the **EBBR** variant of StandaloneSctStartup.nsh.
5. The SCT framework writes results to the sct_results directory in the root of the device. The final summary report is available at:  
   ```text
   fsX:\sct_results\Overall\summary.log
   ```
6. Restore Default UEFI Boot Order (after testing)  
   After SCT completes, return to UEFI Setup and restore the original boot order so the system boots normally.

---

## Running SCT Manually Using Recipe Sequence File

BBR SCT tests are included as part of the build. The BBR SCT can be run with two different selections **SBBR** and **EBBR** by passing the related sequence file to `SCT.efi` using the `-s` option.

> For BBR‑ACS UEFI‑SCT test lists for SBBR, EBBR, and BBSR recipes, see the **[Documentation directory](./docs/)**.

**Steps:**

- Prepare a UEFI-accessible storage device (refer to the Prepare a UEFI-accessible storage device section for detailed steps).
- On the formatted target device, create a directory named `SCT` at the Root Directory level.
- Copy the SCT binaries from:
   ```text
   bbr-acs/<ebbr|sbbr>/scripts/edk2-test/uefi-sct/<SBBR|EBBR>-SCT   # e.g. SBBR-SCT or EBBR-SCT
   ```
- Boot the system under test (SUT) to the UEFI Shell.
- At the UEFI Shell:
  ```text
  Shell> fsX:
  fsX:> cd \path\to\SCT
  ```
- To run SBBR or EBBR tests:
  ```text
  fsX:\path\to\SCT> SCT -s <sbbr.seq|ebbr.seq>
  ```

### Additional commands

- Run all tests (verbose):
  ```text
  fsX:\path\to\SCT> SCT -a -v
  ```
- Continue from the previous session:
  ```text
  fsX:\path\to\SCT> SCT -c
  ```

**Notes:**
- The system may reboot multiple times during SCT execution.
- Ensure the system is configured to automatically reboot to the disk containing the BBR SCT so testing can continue seamlessly.
- You can select and run individual tests. For detailed usage and options, see the **[SCT User Guide](https://github.com/tianocore/edk2-test/blob/master/uefi-sct/Doc/UserGuide/UEFI_SCT_II_UserGuide_2_6_A.pdf)**.

---

### Additional SCT Tests

To run additional SCT tests (e.g., Simple Filesystem, Block I/O, and others) that extend compliance coverage, use the **extended sequence files**:

```text
fsX:\path\to\SCT> SCT -s <SBBR_extd_run.seq|EBBR_extd_run.seq>
```

These extended runs help validate optional or advanced firmware interfaces beyond the standard compliance scope.

---

### Manual Intervention Tests for EBBR

Some EBBR SCT tests in the BBR‑ACS suite require manual interaction.

1. Move or copy the SCT logs into the results partition so they are not overwritten:
   ```text
   Shell> fsX:
   fsX:\acs_results\> mv sct_results sct_results_original
   ```
2. To run manual tests for EBBR, use the `ebbr_manual.seq` file:
   ```text
   fsX:acs_tests\bbr\SCT> SCT -s ebbr_manual.seq
   ```
3. During reset tests, you may need to **manually reset** the system if it hangs.

> **Note:** Logs for manual tests will overwrite the logs from the original test run—hence the need to copy them first. You may need to concatenate some logs to view them together.

---

## Firmware Test Suite (FWTS)

The **Firmware Test Suite (FWTS)** is a validation toolset developed and maintained by Canonical. It provides comprehensive tests for **ACPI**, **SMBIOS**, and **UEFI** interfaces. Several SBBR and EBBR compliance assertions are validated through FWTS as part of the Arm BBR‑ACS framework.

---

## Building Standalone FWTS

To build FWTS as a standalone component:

```bash
git clone https://github.com/ARM-software/bbr-acs.git
cd bbr-acs
./common/scripts/build-standalone-fwts.sh
```

After the build completes, FWTS binaries and dependencies will be located at:

- `fwts_workspace/buildroot/output/target/usr/bin/fwts`
- `fwts_workspace/buildroot/output/target/usr/bin/kernelscan`
- `fwts_workspace/buildroot/output/target/usr/lib64/fwts/`
- `fwts_workspace/buildroot/output/target/usr/lib/fwts/`
- `fwts_workspace/buildroot/output/target/usr/share/fwts/`

---

## Run FWTS Binary

Follow these steps to execute FWTS tests on the target system.

1. **Boot to the target OS.**
2. **Create FWTS workspace on the target OS:**
   ```bash
   mkdir -p ~/fwts_workspace/bin
   mkdir -p ~/fwts_workspace/lib
   ```
3. **Copy FWTS binary and dependencies to the workspace:**
   - Copy the `fwts` binary to `~/fwts_workspace/bin`.
   - Copy all required shared libraries (e.g., `libfwts.so`, etc.) to `~/fwts_workspace/lib`.
4. **Set library path:**
   ```bash
   export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/fwts_workspace/lib
   ```
5. **Run full FWTS tests:**
   ```bash
   ~/fwts_workspace/bin/fwts
   ```

### Running FWTS for Different Recipes

Use the following commands to run FWTS tests for **SBBR**, **EBBR**, or **BBSR** compliance.

- **SBBR**
  ```bash
  ~/fwts_workspace/bin/fwts -r stdout -q --uefi-set-var-multiple=1 --uefi-get-mn-count-multiple=1 --sbbr aest cedt slit srat hmat pcct pdtt bgrt bert einj erst hest sdei nfit iort mpam ibft ras2
  ```

- **EBBR**
  ```bash
  ~/fwts_workspace/bin/fwts --ebbr -r stdout
  ```

- **BBSR**  
  **For ACPI-Based System Image:**
  ```bash
  ~/fwts_workspace/bin/fwts uefirtauthvar tpm2
  ```
  **For Device Tree-Based System Image:**
  ```bash
  ~/fwts_workspace/bin/fwts uefirtauthvar
  ```

**Notes:**

- By default, `build-standalone-fwts.sh` builds the latest FWTS version supported by **Arm SystemReady**, as defined in the upstream configuration.
- To build a specific FWTS version supported by Arm SystemReady, set the `USER_DEFINED_FWTS_VERSION` variable in `build-standalone-fwts.sh`. Currently supported versions:
  - `25.09.00`
  - `25.01.00`
  - `24.09.00`
  - `24.03.00`
  - `24.01.00`
  - `23.07.00`
  - `22.11.00`
- To apply custom patches to FWTS, place patch files in:
  ```text
  buildroot/package/fwts/
  ```

---

## License

Arm BBR‑ACS is distributed under the **Apache v2.0 License**.

---

## Feedback, Contributions, and Support

- For feedback, use the GitHub Issue Tracker associated with this repository.
- For support, email **support-systemready-acs@arm.com** with details.
- Arm licensees may also contact Arm directly through their partner managers.
- Arm welcomes code contributions via GitHub pull requests. See GitHub docs for guidance on raising pull requests.

---

*Copyright (c) 2021–2025, Arm Limited and Contributors. All rights reserved.*


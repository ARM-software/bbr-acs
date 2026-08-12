# PCIe Option ROM Architecture Audit

Tiny EDK II UEFI application that scans live PCIe devices via `EFI_PCI_IO_PROTOCOL`, inspects `RomImage` / `RomSize`, and reports devices whose option ROM contains one or more UEFI driver images but no AArch64 UEFI driver.

It also supports a full report mode that lists every PCIe device scanned, whether it has an option ROM, and whether it contains IA32, X64, AARCH64, EBC, or other UEFI driver images.

For every device block that is printed, the app also tries to show:

- the controller device path as text
- a human-readable controller name from `EFI_COMPONENT_NAME2_PROTOCOL`, with fallback to `EFI_COMPONENT_NAME_PROTOCOL`

## Files

- `sbbr/uefi_app/PcieOptionRomArchAudit.c`
- `sbbr/uefi_app/PcieOptionRomArchAudit.inf`
- `sbbr/uefi_app/PcieOptionRomArchAudit.dsc`

## Build

The application is integrated into the SBBR UEFI application build. Run the SBBR build script from `sbbr/scripts`:

```bash
./build-scripts/build_sbbr.sh       # Standalone (S) build
./build-scripts/build_sbbr.sh F     # Full SystemReady (F) build
```

The resulting binary is written to:

```text
sbbr/scripts/edk2/Build/MdeModule/DEBUG_GCC5/AARCH64/PcieOptionRomArchAudit.efi
```

Full (`F`) builds sign the generated application. The `F` build must be run in the SystemReady ACS environment, where the signing keys and full-build configuration are available.

## Log output

During a SystemReady band ACS run, `PcieOptionRomArchAudit.efi` creates or opens its log at `acs_results_template/acs_results/uefi_dump/PcieOptionRomArchAudit.log`.

If the preferred path cannot be found, such as when the application is run standalone without the `acs_results_template/acs_results/uefi_dump` directory, the application does not create that directory. Instead, it reports the fallback on the console and creates or opens `PcieOptionRomArchAudit.log` in the current working directory.

## Run

Copy the resulting `PcieOptionRomArchAudit.efi` to the EFI System Partition and launch it from the UEFI shell or boot manager.

### Default mode

```text
Shell> PcieOptionRomArchAudit.efi
```

Default mode scans live PCIe devices with option ROMs and reports `HIT`s.

### Report mode

```text
Shell> PcieOptionRomArchAudit.efi --report
```

You can also use `-r`.

Report mode prints every scanned PCIe device, even if it has no option ROM. Devices that do have option ROMs also include per-image details from the ROM parser.

## Result interpretation

### Exit status

- `EFI_SUCCESS`: all inspected option ROMs are compliant.
- `EFI_UNSUPPORTED`: one or more option ROMs contain UEFI driver images but no AARCH64 UEFI driver.
- `EFI_COMPROMISED_DATA`: an option ROM is malformed or cannot be parsed.
- `EFI_DEVICE_ERROR`: a PCI device could not be inspected.

- `HIT`: the option ROM contains one or more UEFI driver images, and does not contain an AARCH64 UEFI driver image.
- `legacy-x86`: a PC-AT compatible legacy option ROM image was found.
- `ia32-uefi`, `x64-uefi`, `arm64-uefi`, `ebc-uefi`, `other-uefi`: presence of UEFI **driver** images for those machine types within the option ROM.
- `other-uefi`: any UEFI driver image whose machine type is not IA32, X64, AARCH64, or EBC. The per-image output prints the numeric machine type and may print a known label such as `RISCV64` when that constant is available in the EDK II headers.
- `DevicePath`: the controller handle's device path converted to text.
- `DeviceName`: controller name returned by the managing driver's Component Name protocol, when available in English.

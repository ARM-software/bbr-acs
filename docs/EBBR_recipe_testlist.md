## BBR ACS UEFI-SCT Test-list for EBBR Recipe

#### Legend:
Enabled:✅ <br>
Partially Enabled:🔲 <br>
Disabled:❌ <br>

> **Note**: X<sup>[arm]</sup> Represents *Arm-specific Tests* with Arm-specific checks and assertions, integrated on top of the standard UEFI-SCT and built into the BBR-ACS SCT. The code for these tests resides in the Arm BBR-ACS repository.

This document corresponds to the SCT sequence file [`EBBR.seq`](../ebbr/config/EBBR.seq), which determines what tests will be run during compliance testing.

Refer to [UEFI SCT Test Case Specification](https://uefi-sct-testcasespec.readthedocs.io/en/latest/) for detailed information for each assertion in the UEFI SCT fundamental service and protocol tests. <br>

<table border="1">
<tr>
<th rowspan=1 colspan=1>Test Suite</th>
<th rowspan=1 colspan=1>Sub Test Suite</th>
<th rowspan=1 colspan=1>Testcase</th>
</tr>
<tr>
<td rowspan=8 colspan=1>🔲 GenericTest</td>
<td rowspan=3 colspan=1>✅ EFICompliantTest</td>
<td rowspan=1 colspan=1>✅ PlatformSpecificElements</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ RequiredElements</td>
<tr>
<td rowspan=1 colspan=1>✅ Conformance Profiles Table BB Test</td>
</tr>
<td rowspan=1 colspan=1>❌ SbbrRequiredUefiProtocols<sup>[arm]</sup></td>
<td rowspan=1 colspan=1>❌ MediaIoProtocols<sup>[arm]</sup></td>
</tr>
<td rowspan=1 colspan=1>✅ SbbrEfiSpecVerLvl<sup>[arm]</sup></td>
<td rowspan=1 colspan=1>✅ TestEfiSpecVerLvl<sup>[arm]</sup></td>
</tr>
<td rowspan=1 colspan=1>✅ SbbrSysEnvConfig<sup>[arm]</sup></td>
<td rowspan=1 colspan=1>✅ BootExcLevel<sup>[arm]</sup></td>
</tr>
<td rowspan=1 colspan=1>✅ EfiConformanceProfileTableTest<sup>[arm]</sup></td>
<td rowspan=1 colspan=1>✅ EfiConformanceProfileTableTest_func<sup>[arm]</sup></td>
</tr>
<td rowspan=1 colspan=1>❌ PlatformResetAttackMitigationPsciTest<sup>[arm]</sup></td>
<td rowspan=1 colspan=1>❌ PlatformResetAttackMitigationPsciTest_<sup>[arm]</sup></td>
<tr>
<tr>
<td rowspan=81 colspan=1>🔲 BootServicesTest</td>
<td rowspan=14 colspan=1>✅ EventTimerandPriorityServicesTest</td>
<td rowspan=1 colspan=1>✅ CheckEvent_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ CheckEvent_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ CloseEvent_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ CreateEventEx_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ CreateEventEx_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ CreateEvent_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ CreateEvent_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ RaiseTPL_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ RestoreTPL_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetTimer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetTimer_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SignalEvent_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ WaitForEvent_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ WaitForEvent_Func</td>
</tr>
<td rowspan=8 colspan=1>✅ MemoryAllocationServicesTest</td>
<td rowspan=1 colspan=1>✅ AllocatePages_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ AllocatePages_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ AllocatePool_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ AllocatePool_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ FreePages_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ FreePages_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetMemoryMap_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetMemoryMap_Func</td>
</tr>
<td rowspan=36 colspan=1>✅ ProtocolHandlerServicesTest</td>
<td rowspan=1 colspan=1>✅ CloseProtocol_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ CloseProtocol_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ ConnectController_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ ConnectController_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ DisconnectController_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ DisconnectController_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ HandleProtocol_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ HandleProtocol_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ InstallMultipleProtocolInterfaces_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ InstallMultipleProtocolInterfaces_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ InstallProtocolInterface_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ InstallProtocolInterface_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ LocateDevicePath_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ LocateDevicePath_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ LocateHandleBuffer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ LocateHandleBuffer_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ LocateHandle_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ LocateHandle_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ LocateProtocol_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ LocateProtocol_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ OpenProtocolInformation_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ OpenProtocolInformation_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ OpenProtocol_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ OpenProtocol_Func_1</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ OpenProtocol_Func_2</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ OpenProtocol_Func_3</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ ProtocolsPerHandle_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ ProtocolsPerHandle_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ RegisterProtocolNotify_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ RegisterProtocolNotify_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ ReinstallProtocolInterface_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ ReinstallProtocolInterface_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ UninstallMultipleProtocolInterfaces_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ UninstallMultipleProtocolInterfaces_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ UninstallProtocolInterface_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ UninstallProtocolInterface_Func</td>
</tr>
<td rowspan=9 colspan=1>✅ ImageServicesTest</td>
<td rowspan=1 colspan=1>✅ ExitBootServices_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Exit_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Exit_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ LoadImage_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ LoadImage_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ StartImage_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ StartImage_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ UnloadImage_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ UnloadImage_Func</td>
</tr>
<td rowspan=11 colspan=1>✅ MiscBootServicesTest</td>
<td rowspan=1 colspan=1>✅ CalculateCrc32_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ CalculateCrc32_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ CopyMem_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetNextMonotonicCount_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetNextMonotonicCount_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ InstallConfigurationTable_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ InstallConfigurationTable_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetMem_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetWatchdogTimer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetWatchdogTimer_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Stall_Func</td>
</tr>
<td rowspan=3 colspan=1>❌ SbbrBootServices<sup>[arm]</sup></td>
<td rowspan=1 colspan=1>❌ AcpiTable<sup>[arm]</sup></td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MemoryMap<sup>[arm]</sup></td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SmbiosTable<sup>[arm]</sup></td>
</tr>
<tr>
<td rowspan=32 colspan=1>🔲 RuntimeServicesTest</td>
<td rowspan=3 colspan=1>❌ SecureBootTest</td>
<td rowspan=1 colspan=1>❌ ImageLoading</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ VariableAttributes</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ VariableUpdates</td>
</tr>
<td rowspan=12 colspan=1>🔲 VariableServicesTest</td>
<td rowspan=1 colspan=1>❌ AuthVar_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ AuthVar_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetNextVariableName_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetNextVariableName_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetVariable_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetVariable_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ HardwareErrorRecord_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ HardwareErrorRecord_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ QueryVariableInfo_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ QueryVariableInfo_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetVariable_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetVariable_Func</td>
</tr>
<td rowspan=1 colspan=1>❌ BBSRVariableSizeTest<sup>[arm]</sup></td>
<td rowspan=1 colspan=1>❌ BBSRVariableSizeTest_func<sup>[arm]</sup></td>
</tr>
<td rowspan=8 colspan=1>✅ TimeServicesTest</td>
<td rowspan=1 colspan=1>✅ GetTime_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetTime_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetWakeupTime_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetWakeupTime_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetTime_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetTime_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetWakeupTime_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetWakeupTime_Func</td>
</tr>
<td rowspan=4 colspan=1>🔲 MiscRuntimeServicesTest</td>
<td rowspan=1 colspan=1>✅ QueryCapsuleCapabilities_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ QueryCapsuleCapabilities_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ResetSystem_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ UpdateCapsule_Conf</td>
</tr>
<td rowspan=3 colspan=1>🔲 SBBRRuntimeServicesTest</td>
<td rowspan=1 colspan=1>✅ Non-volatile Variable Reset Test</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ResetSystem Shutdown Test</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Runtime Services Test</td>
</tr>
<td rowspan=1 colspan=1>❌ TCGMemoryOverwriteRequestTest</td>
<td rowspan=1 colspan=1>❌ Test MOR and MORLOCK</td>
</tr>
<tr>
<td rowspan=2 colspan=1>✅ LoadedImageProtocolTest</td>
<td rowspan=1 colspan=1>✅ LoadedImageProtocolTest1</td>
<td rowspan=1 colspan=1>✅ LoadedImageProtocolTest1</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ LoadedImageProtocolTest2</td>
<td rowspan=1 colspan=1>✅ LoadedImageProtocolTest2</td>
</tr>
<tr>
<td rowspan=27 colspan=1>🔲 DevicePathProcotols</td>
<td rowspan=1 colspan=1>✅ DevicePathProcotolTest</td>
<td rowspan=1 colspan=1>✅ PathNode_Conf</td>
</tr>
<td rowspan=15 colspan=1>✅ DevicePathUtilitiesProcotolTest</td>
<td rowspan=1 colspan=1>✅ AppendDeviceNode_Conformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ AppendDeviceNode_Functionality</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ AppendDevicePathInstance_Conformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ AppendDevicePathInstance_Functionality</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ AppendDevicePath_Conformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ AppendDevicePath_Functionality</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ CreatDeviceNode_Functionality</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ CreateDeviceNode_Conformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ DuplicateDevicePath_Conformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ DuplicateDevicePath_Functionality</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetDevicePathSize_Conformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetDevicePathSize_Functionality</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetNextDevicePathInstance_Conformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetNextDevicePathInstance_Functionality</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ IsDevicePathMultiInstance_Functionality</td>
</tr>
<td rowspan=5 colspan=1>❌ DevicePathToTextProcotolTest</td>
<td rowspan=1 colspan=1>❌ ConvertDeviceNodeToText_Conformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ConvertDeviceNodeToText_Coverage</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ConvertDeviceNodeToText_Functionality</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ConvertDevicePathToText_Conformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ConvertDevicePathToText_Functionality</td>
</tr>
<td rowspan=6 colspan=1>❌ DevicePathFromTextProcotolTest</td>
<td rowspan=1 colspan=1>❌ ConvertTextToDeviceNode_Conformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ConvertTextToDeviceNode_Coverage</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ConvertTextToDeviceNode_Functionality</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ConvertTextToDevicePath_Conformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ConvertTextToDevicePath_Coverage</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ConvertTextToDevicePath_Functionality</td>
</tr>
<tr>
<td rowspan=4 colspan=1>❌ ACPITableProtocolTest</td>
<td rowspan=1 colspan=1>❌ InstallAcpiTableConformance</td>
<td rowspan=1 colspan=1>❌ InstallAcpiTableConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ InstallAcpiTableFunction</td>
<td rowspan=1 colspan=1>❌ InstallAcpiTableFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UninstallAcpiTableConformance</td>
<td rowspan=1 colspan=1>❌ UninstallAcpiTableConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UninstallAcpiTableFunction</td>
<td rowspan=1 colspan=1>❌ UninstallAcpiTableFunction</td>
</tr>
<tr>
<td rowspan=28 colspan=1>❌ DriverModelTest</td>
<td rowspan=6 colspan=1>❌ PlatformDriverOverrideProtocolTest</td>
<td rowspan=1 colspan=1>❌ DriverLoaded_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ DriverLoaded_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDriverPath_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDriverPath_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDriver_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDriver_Func</td>
</tr>
<td rowspan=1 colspan=1>❌ BusSpecificDriverOverrideProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetDriver_Conf</td>
</tr>
<td rowspan=2 colspan=1>❌ DriverDiagnostics2ProtocolTest</td>
<td rowspan=1 colspan=1>❌ RunDiagnostics_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RunDiagnostics_Func</td>
</tr>
<td rowspan=4 colspan=1>❌ ComponentName2ProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetControllerName_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetControllerName_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDriverName_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDriverName_Func</td>
</tr>
<td rowspan=6 colspan=1>❌ AdapterInformationProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetInformationConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetInformationFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetSupportedTypesConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetSupportedTypesFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetInformationConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetInformationFunction</td>
</tr>
<td rowspan=9 colspan=1>❌ PlatformToDriverConfigurationProtocolTest</td>
<td rowspan=1 colspan=1>❌ CLPCommand</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ CLPErrorValue</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ CLPMessageCode</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ CLPRetuenString</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ CLPReturnStatus</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Query_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Query_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Response_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Response_Func</td>
</tr>
<tr>
<td rowspan=91 colspan=1>❌ MediaAccessTest</td>
<td rowspan=18 colspan=1>❌ SimpleFileSystemProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetPosition_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetPosition_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ OpenEx_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ OpenEx_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ OpenVolume_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Open_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Open_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ReadEx_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ReadEx_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Read_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetInfo_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetInfo_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetPosition_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetPosition_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ WriteEx_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ WriteEx_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Write_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Write_Func</td>
</tr>
<td rowspan=2 colspan=1>❌ DiskIOProtocolTest</td>
<td rowspan=1 colspan=1>❌ ReadDisk_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ReadDisk_Func</td>
</tr>
<td rowspan=7 colspan=1>❌ DiskIO2ProtocolTest</td>
<td rowspan=1 colspan=1>❌ Cancel_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ FlushDiskEx_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ FlushDiskEx_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ReadDiskEx_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ReadDiskEx_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ WriteDiskEx_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ WriteDiskEx_Func</td>
</tr>
<td rowspan=6 colspan=1>❌ BlockIOProtocolTest</td>
<td rowspan=1 colspan=1>❌ FlushBlocks_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ FlushBlocks_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MediaInfo_Integrity</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ReadBlocks_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ReadBlocks_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Reset_Func</td>
</tr>
<td rowspan=8 colspan=1>❌ BlockIO2ProtocolTest</td>
<td rowspan=1 colspan=1>❌ FlushBlocksEx_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ FlushBlocksEx_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MediaInfo_Integrity</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ReadBlocksEx_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ReadBlocksEx_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Reset_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ WriteBlocksEx_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ WriteBlocksEx_Func</td>
</tr>
<td rowspan=2 colspan=1>❌ StorageSecurityCommandProtocolTest</td>
<td rowspan=1 colspan=1>❌ ReceiveData_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SendData_Conf</td>
</tr>
<td rowspan=4 colspan=1>❌ RamDiskProtocolTest</td>
<td rowspan=1 colspan=1>❌ RegisterRamDiskConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RegisterRamDiskFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UnregisterRamDiskConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UnregisterRamDiskFunction</td>
</tr>
<td rowspan=8 colspan=1>❌ NVMEPassThruTest</td>
<td rowspan=1 colspan=1>❌ BuildDevicePathConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ BuildDevicePathFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNamespaceConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNamespaceFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextNamespaceConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextNamespaceFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PassThruConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PassThruFunction</td>
</tr>
<td rowspan=2 colspan=1>❌ EraseBlock</td>
<td rowspan=1 colspan=1>❌ EraseBlocksConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ EraseBlocksFunction</td>
</tr>
<td rowspan=10 colspan=1>❌ SdMmcPassThru</td>
<td rowspan=1 colspan=1>❌ BuildDevicePathConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ BuildDevicePathFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextSlotConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextSlotFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetSlotNumberConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetSlotNumberFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PassThruConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PassThruFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ResetDeviceConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ResetDeviceFunction</td>
</tr>
<td rowspan=1 colspan=1>❌ PartitionInfo</td>
<td rowspan=1 colspan=1>❌ PartitionInfoFunction</td>
</tr>
<td rowspan=3 colspan=1>❌ UFSDeviceConfig</td>
<td rowspan=1 colspan=1>❌ RwUfsAttributeConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RwUfsDescriptorConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RwUfsFlagConformance</td>
</tr>
<td rowspan=14 colspan=1>❌ ATAPassThruProtocolTest</td>
<td rowspan=1 colspan=1>❌ BuildDevicePath_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ BuildDevicePath_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDevice_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDevice_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextDevice_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextDevice_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextPort_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextPort_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Mode_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PassThru_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PassThru_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ResetDevice_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ResetDevice_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ResetPort_Func</td>
</tr>
<td rowspan=6 colspan=1>❌ TapeIoProtocolTest</td>
<td rowspan=1 colspan=1>❌ TapeTest_I</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ TapeTest_II</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ TapeTest_III</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ TapeTest_IV</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ TapeTest_V</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ TapeTest_VI</td>
</tr>
<tr>
<td rowspan=10 colspan=1>❌ StringServiceTest</td>
<td rowspan=6 colspan=1>❌ UnicodeCollation2ProtocolTest</td>
<td rowspan=1 colspan=1>❌ FatToStr_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MetaiMatch_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StriColl_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StrLwr_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StrToFat_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StrUpr_Func</td>
</tr>
<td rowspan=4 colspan=1>❌ RegularExpressionProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetInfo_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetInfo_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MatchString_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MatchString_Func</td>
</tr>
<tr>
<td rowspan=90 colspan=1>🔲 HIITest</td>
<td rowspan=20 colspan=1>✅ HIIDatabaseProtocolTest</td>
<td rowspan=1 colspan=1>✅ ExportPackageListsConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ ExportPackageListsFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ FindKeyboardLayoutsConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ FindKeyboardLayoutsFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetKeyboardLayoutConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetKeyboardLayoutFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetPackageListHandleConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetPackageListHandleFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ ListPackageListsConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ ListPackageListsFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ NewPackageListConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ NewPackageListFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ RegisterPackageNotifyConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ RemovePackageListConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ RemovePackageListFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetKeyboardLayoutConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetKeyboardLayoutFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ UnregisterPackageNotifyConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ UpdatePackageListConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ UpdatePackageListFunction</td>
</tr>
<td rowspan=10 colspan=1>❌ HIIStringProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetLanguagesConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetLanguagesFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetSecondaryLanguagesConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetSecondaryLanguagesFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetStringConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetStringFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ NewStringConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ NewStringFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetStringConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetStringFunction</td>
</tr>
<td rowspan=10 colspan=1>❌ HIIImageProtocolTest</td>
<td rowspan=1 colspan=1>❌ DrawImageConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ DrawImageFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ DrawImageIdConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ DrawImageIdFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetImageConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetImageFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ NewImageConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ NewImageFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetImageConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetImageFunction</td>
</tr>
<td rowspan=12 colspan=1>❌ HIIImageExProtocolTest</td>
<td rowspan=1 colspan=1>❌ DrawImageExConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ DrawImageExFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ DrawImageIdExConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ DrawImageIdExFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetImageExConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetImageExFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetImageInfoConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetImageInfoFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ NewImageExConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ NewImageExFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetImageExConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetImageExFunction</td>
</tr>
<td rowspan=8 colspan=1>❌ HIIFontProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetFontInfoConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetFontInfoFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetGlyphConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetGlyphFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StringIdToImageConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StringIdToImageFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StringToImageConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StringToImageFunction</td>
</tr>
<td rowspan=10 colspan=1>❌ HIIFontExProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetFontInfoExConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetFontInfoExFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetGlyphExConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetGlyphExFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetGlyphInfoConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetGlyphInfoFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StringIdToImageExConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StringIdToImageExFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StringToImageExConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StringToImageExFunction</td>
</tr>
<td rowspan=4 colspan=1>❌ HIIConfigAccessProtocolTest</td>
<td rowspan=1 colspan=1>❌ ExtractConfigConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ExtractConfigFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RouteConfigConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RouteConfigFunction</td>
</tr>
<td rowspan=12 colspan=1>❌ HIIConfigRoutingProtocolTest</td>
<td rowspan=1 colspan=1>❌ BlockToConfig_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ BlockToConfig_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ConfigToBlock_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ConfigToBlock_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ExportConfig_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ExportConfig_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ExtractConfig_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ExtractConfig_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetAltCfg_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetAltCfg_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RouteConfig_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RouteConfig_Func</td>
</tr>
<td rowspan=4 colspan=1>❌ ConfigKeywordHandlerTest</td>
<td rowspan=1 colspan=1>❌ GetDataConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDataFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetDataConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetDataFunction</td>
</tr>
<tr>
<td rowspan=58 colspan=1>❌ PCIBusSupportTest</td>
<td rowspan=28 colspan=1>❌ PCIRootBridgeIOProtocolTest</td>
<td rowspan=1 colspan=1>❌ AllocateBuffer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ AllocateBuffer_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Configuration_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ CopyMem_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ CopyMem_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Flush_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ FreeBuffer_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetAttributes_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetAttributes_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ IoRead_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ IoRead_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ IoWrite_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ IoWrite_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Map_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MemRead_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MemRead_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MemWrite_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MemWrite_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PciRead_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PciRead_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PciWrite_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PciWrite_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PollIo_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PollIo_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PollMem_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PollMem_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetAttributes_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetAttributes_Func</td>
</tr>
<td rowspan=30 colspan=1>❌ PCIIOProtocolTest</td>
<td rowspan=1 colspan=1>❌ AllocateBuffer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ AllocateBuffer_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Attributes_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ CopyMem_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ CopyMem_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Flush_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ FreeBuffer_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetBarAttributes_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetBarAttributes_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetLocation_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetLocation_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ IoRead_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ IoRead_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ IoWrite_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ IoWrite_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Map_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MemRead_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MemRead_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MemWrite_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ MemWrite_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PciRead_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PciRead_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PciWrite_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PciWrite_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PollIo_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PollIo_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PollMem_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PollMem_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetBarAttributes_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetBarAttributes_Func</td>
</tr>
<tr>
<td rowspan=21 colspan=1>❌ SCSIBusSupportTest</td>
<td rowspan=8 colspan=1>❌ SCSIIoProtocolTest</td>
<td rowspan=1 colspan=1>❌ ExecuteScsiCommand_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ExecuteScsiCommand_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDeviceLocation_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDeviceLocation_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDeviceType_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDeviceType_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ResetBus_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ResetDevice_Func</td>
</tr>
<td rowspan=13 colspan=1>❌ ExtendedSCSIPassThruProtocolTest</td>
<td rowspan=1 colspan=1>❌ BuildDevicePath_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ BuildDevicePath_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextTargetLun_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextTargetLun_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextTarget_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextTarget_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetTargetLun_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetTargetLun_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PassThru_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ PassThru_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ResetChannel_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ResetTargetLun_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ResetTargetLun_Func</td>
</tr>
<tr>
<td rowspan=4 colspan=1>❌ ISCSIBootTest</td>
<td rowspan=4 colspan=1>❌ ISCSIInitiatorNameProtocol</td>
<td rowspan=1 colspan=1>❌ Get_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Get_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Set_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Set_Func</td>
</tr>
<tr>
<td rowspan=25 colspan=1>❌ USBSupportTest</td>
<td rowspan=13 colspan=1>❌ USB2HostControllerProtocolTest</td>
<td rowspan=1 colspan=1>❌ AsyncInterruptTransfer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ AsyncIsochronousTransfer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ BulkTransfer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ClearRootHubPortFeature_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ControlTransfer_Conf_Auto</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetCapability_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetRootHubPortStatus_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetState_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ IsochronousTransfer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Reset_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetRootHubPortFeature_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetState_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SyncInterruptTransfer_Conf</td>
</tr>
<td rowspan=12 colspan=1>❌ USBIOProtocolTest</td>
<td rowspan=1 colspan=1>❌ UsbAsyncInterruptTransfer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UsbAsyncIsochronousTransfer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UsbBulkTransfer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UsbControlTransfer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UsbGetConfigDescriptor_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UsbGetDeviceDescriptor_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UsbGetEndpointDescriptor_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UsbGetInterfaceDescriptor_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UsbGetStringDescriptor_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UsbIsochronousTransfer_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UsbPortReset_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UsbSyncInterruptTransfer_Conf</td>
</tr>
<tr>
<td rowspan=48 colspan=1>🔲 NetworkSupportTest</td>
<td rowspan=24 colspan=1>🔲 SimpleNetworkProtocolTest</td>
<td rowspan=1 colspan=1>✅ GetStatus_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetStatus_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Initialize_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Initialize_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ MCastIpToMac_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ MCastIpToMac_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ NVData_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ NVData_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ReceiveFilter_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ReceiveFilter_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Receive_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Reset_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Reset_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Shutdown_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Shutdown_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Start_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Start_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StationAddress_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ StationAddress_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Statistics_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Statistics_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Stop_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Stop_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Transmit_Conf</td>
</tr>
<td rowspan=6 colspan=1>❌ PXEBaseCodeProtocolTest</td>
<td rowspan=1 colspan=1>❌ Arp_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetIpFilter_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Start_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Start_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Stop_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Stop_Func</td>
</tr>
<td rowspan=6 colspan=1>❌ VLANConfigProtocolTest</td>
<td rowspan=1 colspan=1>❌ FindConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ FindFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RemoveConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RemoveFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetFunction</td>
</tr>
<td rowspan=10 colspan=1>❌ IPsecConfigProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetDataConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetDataFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextSelectorConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetNextSelectorFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RegisterDataNotifyConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RegisterDataNotifyFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetDataConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetDataFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UnregisterDataNotifyConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UnregisterDataNotifyFunction</td>
</tr>
<td rowspan=2 colspan=1>❌ IPsec2ProtocolTest</td>
<td rowspan=1 colspan=1>❌ ProcessExtConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ ProcessExtFunction</td>
</tr>
<tr>
<td rowspan=5 colspan=1>❌ DebuggerSupportTest</td>
<td rowspan=4 colspan=1>❌ DebugSupportProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetMaximumProcessorIndex_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ InvalidateInstructionCache_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Isa_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RegisterPeriodicCallback_Func</td>
</tr>
<td rowspan=1 colspan=1>❌ DebugPortProtocolTest</td>
<td rowspan=1 colspan=1>❌ Reset_Func</td>
</tr>
<tr>
<td rowspan=3 colspan=1>❌ CompressionTest</td>
<td rowspan=3 colspan=1>❌ DecompressProtocolTest</td>
<td rowspan=1 colspan=1>❌ Decompress_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Decompress_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetInfo_Func</td>
</tr>
<tr>
<td rowspan=3 colspan=1>❌ EFIByteCodeTest</td>
<td rowspan=3 colspan=1>❌ EBCInterpreterProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetVersion_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetVersion_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UnloadImage_Conf</td>
</tr>
<tr>
<td rowspan=3 colspan=1>❌ TimeStampProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetPropertiesConformanceAuto</td>
<td rowspan=1 colspan=1>❌ GetPropertiesConformanceAuto</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetPropertiesFunctionAuto</td>
<td rowspan=1 colspan=1>❌ GetPropertiesFunctionAuto</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetTimestampFunctionAuto</td>
<td rowspan=1 colspan=1>❌ GetTimestampFunctionAuto</td>
</tr>
<tr>
<td rowspan=4 colspan=1>❌ ResetNotificationProtocolTest</td>
<td rowspan=1 colspan=1>❌ RegisterResetNotifyConformance</td>
<td rowspan=1 colspan=1>❌ RegisterResetNotifyConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ RegisterResetNotifyFunction</td>
<td rowspan=1 colspan=1>❌ RegisterResetNotifyFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UnregisterResetNotifyConformance</td>
<td rowspan=1 colspan=1>❌ UnregisterResetNotifyConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ UnregisterResetNotifyFunction</td>
<td rowspan=1 colspan=1>❌ UnregisterResetNotifyFunction</td>
</tr>
<tr>
<td rowspan=16 colspan=1>🔲 SecureTechTest</td>
<td rowspan=4 colspan=1>✅ RNGProtocolTest</td>
<td rowspan=1 colspan=1>✅ GetInfo_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetInfo_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetRNG_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetRNG_Func</td>
</tr>
<td rowspan=8 colspan=1>❌ Hash2Test</td>
<td rowspan=1 colspan=1>❌ GetHashSizeConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetHashSizeFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ HashConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ HashFinalConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ HashFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ HashInitConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ HashMultiBlocksFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ HashUpdateConformance</td>
</tr>
<td rowspan=4 colspan=1>❌ PKCS7VerifyTest</td>
<td rowspan=1 colspan=1>❌ VerifyBufferConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ VerifyBufferFunction</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ VerifySignatureConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ VerifySignatureFunction</td>
</tr>
<tr>
<td rowspan=41 colspan=1>🔲 ConsoleSupportTest</td>
<td rowspan=6 colspan=1>✅ SimpleTextInputExProtocolTest</td>
<td rowspan=1 colspan=1>✅ ReadKeyStrokeExConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ ReadKeyStrokeExFunctionAuto</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ RegisterKeyNotifyConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ ResetFunctionAuto</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetStateConformance</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ UnregisterKeyNotifyConformance</td>
</tr>
<td rowspan=1 colspan=1>✅ SimpleInputProtocolTest</td>
<td rowspan=1 colspan=1>✅ Reset_Func</td>
</tr>
<td rowspan=12 colspan=1>✅ SimpleOutputProtocolTest</td>
<td rowspan=1 colspan=1>✅ ClearScreen_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ EnableCursor_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ OutputString_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ QueryMode_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ QueryMode_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ Reset_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetAttribute_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetCursorPosition_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetCursorPosition_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetMode_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SetMode_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ TestString_Func</td>
</tr>
<td rowspan=2 colspan=1>❌ SimplePointerProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetState_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Reset_Func</td>
</tr>
<td rowspan=9 colspan=1>❌ SerialIOProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetControl_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Read_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Read_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Reset_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetAttributes_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetAttributes_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetControl_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetControl_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Write_Func</td>
</tr>
<td rowspan=8 colspan=1>❌ GraphicsOutputProtocolTest</td>
<td rowspan=1 colspan=1>❌ BltVideoBltBuffer_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ BltVideoFill_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ BltVideoToVideo_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Blt_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ QueryMode_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ QueryMode_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetMode_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ SetMode_Func</td>
</tr>
<td rowspan=3 colspan=1>❌ AbsolutePointerProtocolTest</td>
<td rowspan=1 colspan=1>❌ GetState_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ GetState_Func</td>
</tr>
<tr>
<td rowspan=1 colspan=1>❌ Reset_Func</td>
</tr>
<tr>
  <td rowspan="8" colspan="1">❌ FirmwareManagementTest</td>
  <td rowspan="8" colspan="1">❌ FirmwareManagementProtocol</td>
  <td rowspan="1" colspan="1">❌ Conformance_Test-CheckImage</td>
</tr>
<tr>
  <td rowspan="1" colspan="1">❌ Conformance_Test-GetImage</td>
</tr>
<tr>
  <td rowspan="1" colspan="1">❌ Conformance_Test-GetImageInfo</td>
</tr>
<tr>
  <td rowspan="1" colspan="1">❌ Conformance_Test-SetImage</td>
</tr>
<tr>
  <td rowspan="1" colspan="1">❌ Conformance_Test-SetPackageInfo</td>
</tr>
<tr>
  <td rowspan="1" colspan="1">❌ Function_Test-GetImage</td>
</tr>
<tr>
  <td rowspan="1" colspan="1">❌ Function_Test-GetImageInfo</td>
</tr>
<tr>
  <td rowspan="1" colspan="1">❌ Function_Test-GetPackageInfo</td>
</tr>
<td rowspan=4 colspan=1>✅ TCG2ProtocolTest</td>
<td rowspan=1 colspan=1>✅ GetActivePcrBanks_Conf</td>
<td rowspan=1 colspan=1>✅ GetActivePcrBanks_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ GetCapability_Conf</td>
<td rowspan=1 colspan=1>✅ GetCapability_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ HashLogExtendEvent_Conf</td>
<td rowspan=1 colspan=1>✅ HashLogExtendEvent_Conf</td>
</tr>
<tr>
<td rowspan=1 colspan=1>✅ SubmitCommand_Conf</td>
<td rowspan=1 colspan=1>✅ SubmitCommand_Conf</td>
</tr>
</tr>
</table>



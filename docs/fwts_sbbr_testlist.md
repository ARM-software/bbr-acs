# SBBR FWTS test-list

The table below lists the test modules executed for the SBBR recipe.

><p><b>Note:</b> X<sup>[sbbr]</sup> indicates that the FWTS test X belongs to the <b>SBBR</b> test category (executed as part of the <code>--sbbr</code> test-list).</p>

<table>
<thead><tr><th>Test</th><th>Minor Tests</th></tr></thead>
<tbody>

<tr><td rowspan=4 colspan=1>acpi_sbbr<sup>[sbbr]</sup></td><td>1. Test that processors only exist in the _SB namespace</td></tr>
<tr><td>2. Test DSDT and SSDT tables are implemented</td></tr>
<tr><td>3. Check for mandatory and recommended ACPI tables</td></tr>
<tr><td>4. Check the existence of SPCR console devices</td></tr>

<tr><td rowspan=2 colspan=1>acpitables<sup>[sbbr]</sup></td><td>1. Test ACPI headers</td></tr>
<tr><td>2. Test ACPI spec versus table revisions</td></tr>

<tr><td rowspan=1 colspan=1>aest</td><td>1. Validate AEST table</td></tr>

<tr><td rowspan=1 colspan=1>bert</td><td>1. BERT Boot Error Record Table test</td></tr>

<tr><td rowspan=1 colspan=1>bgrt</td><td>1. BGRT Boot Graphics Resource Table test</td></tr>

<tr><td rowspan=1 colspan=1>cedt</td><td>1. Validate CEDT table</td></tr>

<tr><td rowspan=2 colspan=1>dbg2<sup>[sbbr]</sup></td><td>1. DBG2 (Debug Port Table 2) test</td></tr>
<tr><td>2. DBG2 ARM BSA compliant UART test</td></tr>

<tr><td rowspan=4 colspan=1>dmicheck<sup>[sbbr]</sup></td><td>1. Find and test SMBIOS Table Entry Points</td></tr>
<tr><td>2. Test DMI/SMBIOS tables for errors</td></tr>
<tr><td>3. Test DMI/SMBIOS3 tables for errors</td></tr>
<tr><td>4. Test ARM SBBR SMBIOS structure requirements</td></tr>

<tr><td rowspan=1 colspan=1>einj</td><td>1. EINJ Error Injection Table test</td></tr>

<tr><td rowspan=1 colspan=1>erst</td><td>1. ERST Error Record Serialization Table test</td></tr>

<tr><td rowspan=1 colspan=1>esrt<sup>[sbbr]</sup></td><td>1. Sanity check UEFI ESRT Table</td></tr>

<tr><td rowspan=3 colspan=1>fadt_sbbr<sup>[sbbr]</sup></td><td>1. FADT Revision Test</td></tr>
<tr><td>2. FADT Reduced HW Test</td></tr>
<tr><td>3. FADT PSCI Compliant Test</td></tr>

<tr><td rowspan=1 colspan=1>gtdt<sup>[sbbr]</sup></td><td>1. GTDT Generic Timer Description Table test</td></tr>

<tr><td rowspan=1 colspan=1>hest</td><td>1. HEST Hardware Error Source Table test</td></tr>

<tr><td rowspan=1 colspan=1>hmat</td><td>1. Validate HMAT table</td></tr>

<tr><td rowspan=1 colspan=1>ibft</td><td>1. Validate iBFT table</td></tr>

<tr><td rowspan=1 colspan=1>iort</td><td>1. IORT IO Remapping Table test</td></tr>

<tr><td rowspan=5 colspan=1>madt<sup>[sbbr]</sup></td><td>1. MADT checksum test</td></tr>
<tr><td>2. MADT revision test</td></tr>
<tr><td>3. MADT architecture minimum revision test</td></tr>
<tr><td>4. MADT flags field reserved bits test</td></tr>
<tr><td>5. MADT subtable tests</td></tr>

<tr><td rowspan=2 colspan=1>mcfg<sup>[sbbr]</sup></td><td>1. Validate MCFG table</td></tr>
<tr><td>2. Validate MCFG PCI config space</td></tr>

<tr><td rowspan=233 colspan=1>method<sup>[sbbr]</sup></td><td>1. Test Method Names</td></tr>
<tr><td>2. Test _PR  (Processor)</td></tr>
<tr><td>3. Test _Wxx (Wake Event)</td></tr>
<tr><td>4. Test _AEI</td></tr>
<tr><td>5. Test _EVT (Event Method)</td></tr>
<tr><td>6. Test _DLM (Device Lock Mutex)</td></tr>
<tr><td>7. Test _GL  (Global Lock)</td></tr>
<tr><td>8. Test _OS  (Operating System)</td></tr>
<tr><td>9. Test _REV (Revision)</td></tr>
<tr><td>10. Test _PIC (Inform AML of Interrupt Model)</td></tr>
<tr><td>11. Test _CID (Compatible ID)</td></tr>
<tr><td>12. Test _CLS (Class Code)</td></tr>
<tr><td>13. Test _DDN (DOS Device Name)</td></tr>
<tr><td>14. Test _HID (Hardware ID)</td></tr>
<tr><td>15. Test _HRV (Hardware Revision Number)</td></tr>
<tr><td>16. Test _MLS (Multiple Language String)</td></tr>
<tr><td>17. Test _PLD (Physical Device Location)</td></tr>
<tr><td>18. Test _SUB (Subsystem ID)</td></tr>
<tr><td>19. Test _SUN (Slot User Number)</td></tr>
<tr><td>20. Test _STR (String)</td></tr>
<tr><td>21. Test _UID (Unique ID)</td></tr>
<tr><td>22. Test _CDM (Clock Domain)</td></tr>
<tr><td>23. Test _VDM (Voltage Domain)</td></tr>
<tr><td>24. Test _CRS (Current Resource Settings)</td></tr>
<tr><td>25. Test _DSD (Device Specific Data)</td></tr>
<tr><td>26. Test _DIS (Disable)</td></tr>
<tr><td>27. Test _DMA (Direct Memory Access)</td></tr>
<tr><td>28. Test _FIX (Fixed Register Resource Provider)</td></tr>
<tr><td>29. Test _GSB (Global System Interrupt Base)</td></tr>
<tr><td>30. Test _HPP (Hot Plug Parameters)</td></tr>
<tr><td>31. Test _HPX (Hot Plug Extensions)</td></tr>
<tr><td>32. Test _MAT (Multiple APIC Table Entry)</td></tr>
<tr><td>33. Test _PRS (Possible Resource Settings)</td></tr>
<tr><td>34. Test _PRT (PCI Routing Table)</td></tr>
<tr><td>35. Test _PXM (Proximity)</td></tr>
<tr><td>36. Test _SLI (System Locality Information)</td></tr>
<tr><td>37. Test _SRS (Set Resource Settings)</td></tr>
<tr><td>38. Test _CCA (Cache Coherency Attribute)</td></tr>
<tr><td>39. Test _EDL (Eject Device List)</td></tr>
<tr><td>40. Test _EJD (Ejection Dependent Device)</td></tr>
<tr><td>41. Test _EJ0 (Eject)</td></tr>
<tr><td>42. Test _EJ1 (Eject)</td></tr>
<tr><td>43. Test _EJ2 (Eject)</td></tr>
<tr><td>44. Test _EJ3 (Eject)</td></tr>
<tr><td>45. Test _EJ4 (Eject)</td></tr>
<tr><td>46. Test _LCK (Lock)</td></tr>
<tr><td>47. Test _OST (OSPM Status Indication)</td></tr>
<tr><td>48. Test _RMV (Remove)</td></tr>
<tr><td>49. Test _STA (Status)</td></tr>
<tr><td>50. Test _DEP (Operational Region Dependencies)</td></tr>
<tr><td>51. Test _FIT (Firmware Interface Table)</td></tr>
<tr><td>52. Test _BDN (BIOS Dock Name)</td></tr>
<tr><td>53. Test _BBN (Base Bus Number)</td></tr>
<tr><td>54. Test _DCK (Dock)</td></tr>
<tr><td>55. Test _INI (Initialize)</td></tr>
<tr><td>56. Test _GLK (Global Lock)</td></tr>
<tr><td>57. Test _REG (Region)</td></tr>
<tr><td>58. Test _SEG (Segment)</td></tr>
<tr><td>59. Test _LSI (Label Storage Information)</td></tr>
<tr><td>60. Test _CBR (CXL Host Bridge Register)</td></tr>
<tr><td>61. Test _OFF (Set resource off)</td></tr>
<tr><td>62. Test _ON_ (Set resource on)</td></tr>
<tr><td>63. Test _DSW (Device Sleep Wake)</td></tr>
<tr><td>64. Test _IRC (In Rush Current)</td></tr>
<tr><td>65. Test _PRE (Power Resources for Enumeration)</td></tr>
<tr><td>66. Test _PR0 (Power Resources for D0)</td></tr>
<tr><td>67. Test _PR1 (Power Resources for D1)</td></tr>
<tr><td>68. Test _PR2 (Power Resources for D2)</td></tr>
<tr><td>69. Test _PR3 (Power Resources for D3)</td></tr>
<tr><td>70. Test _PRW (Power Resources for Wake)</td></tr>
<tr><td>71. Test _PS0 (Power State 0)</td></tr>
<tr><td>72. Test _PS1 (Power State 1)</td></tr>
<tr><td>73. Test _PS2 (Power State 2)</td></tr>
<tr><td>74. Test _PS3 (Power State 3)</td></tr>
<tr><td>75. Test _PSC (Power State Current)</td></tr>
<tr><td>76. Test _PSE (Power State for Enumeration)</td></tr>
<tr><td>77. Test _PSW (Power State Wake)</td></tr>
<tr><td>78. Test _S1D (S1 Device State)</td></tr>
<tr><td>79. Test _S2D (S2 Device State)</td></tr>
<tr><td>80. Test _S3D (S3 Device State)</td></tr>
<tr><td>81. Test _S4D (S4 Device State)</td></tr>
<tr><td>82. Test _S0W (S0 Device Wake State)</td></tr>
<tr><td>83. Test _S1W (S1 Device Wake State)</td></tr>
<tr><td>84. Test _S2W (S2 Device Wake State)</td></tr>
<tr><td>85. Test _S3W (S3 Device Wake State)</td></tr>
<tr><td>86. Test _S4W (S4 Device Wake State)</td></tr>
<tr><td>87. Test _RST (Device Reset)</td></tr>
<tr><td>88. Test _PRR (Power Resource for Reset)</td></tr>
<tr><td>89. Test _DSC (Deepest State for Configuration)</td></tr>
<tr><td>90. Test _S0_ (S0 System State)</td></tr>
<tr><td>91. Test _S1_ (S1 System State)</td></tr>
<tr><td>92. Test _S2_ (S2 System State)</td></tr>
<tr><td>93. Test _S3_ (S3 System State)</td></tr>
<tr><td>94. Test _S4_ (S4 System State)</td></tr>
<tr><td>95. Test _S5_ (S5 System State)</td></tr>
<tr><td>96. Test _SWS (System Wake Source)</td></tr>
<tr><td>97. Test _PSS (Performance Supported States)</td></tr>
<tr><td>98. Test _CPC (Continuous Performance Control)</td></tr>
<tr><td>99. Test _CSD (C State Dependencies)</td></tr>
<tr><td>100. Test _CST (C States)</td></tr>
<tr><td>101. Test _PCT (Performance Control)</td></tr>
<tr><td>102. Test _PDC (Processor Driver Capabilities)</td></tr>
<tr><td>103. Test _PDL (P-State Depth Limit)</td></tr>
<tr><td>104. Test _PPC (Performance Present Capabilities)</td></tr>
<tr><td>105. Test _PPE (Polling for Platform Error)</td></tr>
<tr><td>106. Test _PSD (Power State Dependencies)</td></tr>
<tr><td>107. Test _PTC (Processor Throttling Control)</td></tr>
<tr><td>108. Test _TDL (T-State Depth Limit)</td></tr>
<tr><td>109. Test _TPC (Throttling Present Capabilities)</td></tr>
<tr><td>110. Test _TSD (Throttling State Dependencies)</td></tr>
<tr><td>111. Test _TSS (Throttling Supported States)</td></tr>
<tr><td>112. Test _LPI (Low Power Idle States)</td></tr>
<tr><td>113. Test _RDI (Resource Dependencies for Idle)</td></tr>
<tr><td>114. Test _PUR (Processor Utilization Request)</td></tr>
<tr><td>115. Test _MSG (Message)</td></tr>
<tr><td>116. Test _SST (System Status)</td></tr>
<tr><td>117. Test _ALC (Ambient Light Colour Chromaticity)</td></tr>
<tr><td>118. Test _ALI (Ambient Light Illuminance)</td></tr>
<tr><td>119. Test _ALT (Ambient Light Temperature)</td></tr>
<tr><td>120. Test _ALP (Ambient Light Polling)</td></tr>
<tr><td>121. Test _ALR (Ambient Light Response)</td></tr>
<tr><td>122. Test _LID (Lid Status)</td></tr>
<tr><td>123. Test _GTF (Get Task File)</td></tr>
<tr><td>124. Test _GTM (Get Timing Mode)</td></tr>
<tr><td>125. Test _SDD (Set Device Data)</td></tr>
<tr><td>126. Test _STM (Set Timing Mode)</td></tr>
<tr><td>127. Test _FDE (Floppy Disk Enumerate)</td></tr>
<tr><td>128. Test _FDI (Floppy Drive Information)</td></tr>
<tr><td>129. Test _FDM (Floppy Drive Mode)</td></tr>
<tr><td>130. Test _MBM (Memory Bandwidth Monitoring Data)</td></tr>
<tr><td>131. Test _MSM (Memory Set Monitoring)</td></tr>
<tr><td>132. Test _UPC (USB Port Capabilities)</td></tr>
<tr><td>133. Test _PDO (USB Power Data Object)</td></tr>
<tr><td>134. Test _DSM (Device Specific Method)</td></tr>
<tr><td>135. Test _UPD (User Presence Detect)</td></tr>
<tr><td>136. Test _UPP (User Presence Polling)</td></tr>
<tr><td>137. Test _GCP (Get Capabilities)</td></tr>
<tr><td>138. Test _GRT (Get Real Time)</td></tr>
<tr><td>139. Test _GWS (Get Wake Status)</td></tr>
<tr><td>140. Test _CWS (Clear Wake Status)</td></tr>
<tr><td>141. Test _SRT (Set Real Time)</td></tr>
<tr><td>142. Test _STP (Set Expired Timer Wake Policy)</td></tr>
<tr><td>143. Test _STV (Set Timer Value)</td></tr>
<tr><td>144. Test _TIP (Expired Timer Wake Policy)</td></tr>
<tr><td>145. Test _TIV (Timer Values)</td></tr>
<tr><td>146. Test _NBS (NVDIMM Boot Status)</td></tr>
<tr><td>147. Test _NCH (NVDIMM Current Health Information)</td></tr>
<tr><td>148. Test _NIC (NVDIMM Health Error Injection Capabilities)</td></tr>
<tr><td>149. Test _NIH (NVDIMM Inject/Clear Health Errors)</td></tr>
<tr><td>150. Test _NIG (NVDIMM Inject Health Error Status)</td></tr>
<tr><td>151. Test _SBS (Smart Battery Subsystem)</td></tr>
<tr><td>152. Test _BCT (Battery Charge Time)</td></tr>
<tr><td>153. Test _BIF (Battery Information)</td></tr>
<tr><td>154. Test _BIX (Battery Information Extended)</td></tr>
<tr><td>155. Test _BMA (Battery Measurement Averaging)</td></tr>
<tr><td>156. Test _BMC (Battery Maintenance Control)</td></tr>
<tr><td>157. Test _BMD (Battery Maintenance Data)</td></tr>
<tr><td>158. Test _BMS (Battery Measurement Sampling Time)</td></tr>
<tr><td>159. Test _BPC (Battery Power Characteristics)</td></tr>
<tr><td>160. Test _BPS (Battery Power State)</td></tr>
<tr><td>161. Test _BPT (Battery Power Threshold)</td></tr>
<tr><td>162. Test _BST (Battery Status)</td></tr>
<tr><td>163. Test _BTP (Battery Trip Point)</td></tr>
<tr><td>164. Test _BTH (Battery Throttle Limit)</td></tr>
<tr><td>165. Test _BTM (Battery Time)</td></tr>
<tr><td>166. Test _BLT (Battery Level Threshold)</td></tr>
<tr><td>167. Test _PCL (Power Consumer List)</td></tr>
<tr><td>168. Test _PCS (Power Source Current Status)</td></tr>
<tr><td>169. Test _PIF (Power Source Information)</td></tr>
<tr><td>170. Test _PRL (Power Source Redundancy List)</td></tr>
<tr><td>171. Test _PSR (Power Source)</td></tr>
<tr><td>172. Test _PST (Power Status Threshold)</td></tr>
<tr><td>173. Test _GAI (Get Averaging Level)</td></tr>
<tr><td>174. Test _GHL (Get Hardware Limit)</td></tr>
<tr><td>175. Test _PAI (Power Averaging Interval)</td></tr>
<tr><td>176. Test _PMC (Power Meter Capabilities)</td></tr>
<tr><td>177. Test _PMD (Power Meter Devices)</td></tr>
<tr><td>178. Test _PMM (Power Meter Measurement)</td></tr>
<tr><td>179. Test _PTP (Power Trip Points)</td></tr>
<tr><td>180. Test _SHL (Set Hardware Limit)</td></tr>
<tr><td>181. Test _WPC (Wireless Power Calibration)</td></tr>
<tr><td>182. Test _WPP (Wireless Power Polling)</td></tr>
<tr><td>183. Test _FIF (Fan Information)</td></tr>
<tr><td>184. Test _FPS (Fan Performance States)</td></tr>
<tr><td>185. Test _FSL (Fan Set Level)</td></tr>
<tr><td>186. Test _FST (Fan Status)</td></tr>
<tr><td>187. Test _ACx (Active Cooling)</td></tr>
<tr><td>188. Test _ART (Active Cooling Relationship Table)</td></tr>
<tr><td>189. Test _ALx (Active List)</td></tr>
<tr><td>190. Test _CRT (Critical Trip Point)</td></tr>
<tr><td>191. Test _CR3 (Warm/Standby Temperature)</td></tr>
<tr><td>192. Test _DTI (Device Temperature Indication)</td></tr>
<tr><td>193. Test _HOT (Hot Temperature)</td></tr>
<tr><td>194. Test _MTL (Minimum Throttle Limit)</td></tr>
<tr><td>195. Test _NTT (Notification Temp Threshold)</td></tr>
<tr><td>196. Test _PSL (Passive List)</td></tr>
<tr><td>197. Test _PSV (Passive Temp)</td></tr>
<tr><td>198. Test _RTV (Relative Temp Values)</td></tr>
<tr><td>199. Test _SCP (Set Cooling Policy)</td></tr>
<tr><td>200. Test _TC1 (Thermal Constant 1)</td></tr>
<tr><td>201. Test _TC2 (Thermal Constant 2)</td></tr>
<tr><td>202. Test _TFP (Thermal fast Sampling Period)</td></tr>
<tr><td>203. Test _TMP (Thermal Zone Current Temp)</td></tr>
<tr><td>204. Test _TPT (Trip Point Temperature)</td></tr>
<tr><td>205. Test _TRT (Thermal Relationship Table)</td></tr>
<tr><td>206. Test _TSN (Thermal Sensor Device)</td></tr>
<tr><td>207. Test _TSP (Thermal Sampling Period)</td></tr>
<tr><td>208. Test _TST (Temperature Sensor Threshold)</td></tr>
<tr><td>209. Test _TZD (Thermal Zone Devices)</td></tr>
<tr><td>210. Test _TZM (Thermal Zone member)</td></tr>
<tr><td>211. Test _TZP (Thermal Zone Polling)</td></tr>
<tr><td>212. Test _GPE (General Purpose Events)</td></tr>
<tr><td>213. Test _EC_ (EC Offset Query)</td></tr>
<tr><td>214. Test _PTS (Prepare to Sleep)</td></tr>
<tr><td>215. Test _TTS (Transition to State)</td></tr>
<tr><td>216. Test _WAK (System Wake)</td></tr>
<tr><td>217. Test _ADR (Return Unique ID for Device)</td></tr>
<tr><td>218. Test _BCL (Query List of Brightness Control Levels Supported)</td></tr>
<tr><td>219. Test _BCM (Set Brightness Level)</td></tr>
<tr><td>220. Test _BQC (Brightness Query Current Level)</td></tr>
<tr><td>221. Test _DCS (Return the Status of Output Device)</td></tr>
<tr><td>222. Test _DDC (Return the EDID for this Device)</td></tr>
<tr><td>223. Test _DSS (Device Set State)</td></tr>
<tr><td>224. Test _DGS (Query Graphics State)</td></tr>
<tr><td>225. Test _DOD (Enumerate All Devices Attached to Display Adapter)</td></tr>
<tr><td>226. Test _DOS (Enable/Disable Output Switching)</td></tr>
<tr><td>227. Test _GPD (Get POST Device)</td></tr>
<tr><td>228. Test _ROM (Get ROM Data)</td></tr>
<tr><td>229. Test _SPD (Set POST Device)</td></tr>
<tr><td>230. Test _VPO (Video POST Options)</td></tr>
<tr><td>231. Test _CBA (Configuration Base Address)</td></tr>
<tr><td>232. Test _IFT (IPMI Interface Type)</td></tr>
<tr><td>233. Test _SRV (IPMI Interface Revision)</td></tr>

<tr><td rowspan=1 colspan=1>mpam</td><td>1. Validate MPAM table</td></tr>

<tr><td rowspan=1 colspan=1>nfit</td><td>1. Validate NFIT table</td></tr>

<tr><td rowspan=1 colspan=1>pcct</td><td>1. Validate PCC table</td></tr>

<tr><td rowspan=1 colspan=1>pdtt</td><td>1. Validate PDTT table</td></tr>

<tr><td rowspan=1 colspan=1>pptt<sup>[sbbr]</sup></td><td>1. Validate PPTT table</td></tr>

<tr><td rowspan=1 colspan=1>ras2</td><td>1. Validate RAS2 table</td></tr>

<tr><td rowspan=1 colspan=1>rsdp_sbbr<sup>[sbbr]</sup></td><td>1. RSDP Root System Description Pointer test</td></tr>

<tr><td rowspan=1 colspan=1>sdei</td><td>1. Validate SDEI table</td></tr>

<tr><td rowspan=1 colspan=1>slit</td><td>1. SLIT System Locality Distance Information test</td></tr>

<tr><td rowspan=7 colspan=1>smccc</td><td>1. Test PCI_VERSION</td></tr>
<tr><td>2. Test PCI_FEATURES</td></tr>
<tr><td>3. Test PCI_GET_SEG_INFO</td></tr>
<tr><td>4. Test ARM_SMCCC_VERSION</td></tr>
<tr><td>5. Test ARM_SMCCC_ARCH_FEATURES</td></tr>
<tr><td>6. Test ARM_SMCCC_ARCH_SOC_ID for Soc_ID_type 0</td></tr>
<tr><td>7. Test ARM_SMCCC_ARCH_SOC_ID for Soc_ID_type 1</td></tr>

<tr><td rowspan=3 colspan=1>spcr<sup>[sbbr]</sup></td><td>1. SPCR Serial Port Console Redirection Table test</td></tr>
<tr><td>2. SPCR Revision Test</td></tr>
<tr><td>3. SPCR GSIV Interrupt Test</td></tr>

<tr><td rowspan=1 colspan=1>srat</td><td>1. SRAT System Resource Affinity Table test</td></tr>

<tr><td rowspan=1 colspan=1>uefibootpath<sup>[sbbr]</sup></td><td>1. Test UEFI Boot Path Boot####</td></tr>

<tr><td rowspan=4 colspan=1>uefirtmisc<sup>[sbbr]</sup></td><td>1. Test for UEFI miscellaneous runtime service interfaces</td></tr>
<tr><td>2. Stress test for UEFI miscellaneous runtime service interfaces</td></tr>
<tr><td>3. Test GetNextHighMonotonicCount with invalid NULL parameter</td></tr>
<tr><td>4. Test UEFI miscellaneous runtime services unsupported status</td></tr>

<tr><td rowspan=38 colspan=1>uefirttime<sup>[sbbr]</sup></td><td>1. Test UEFI RT service get time interface</td></tr>
<tr><td>2. Test UEFI RT service get time interface, NULL time parameter</td></tr>
<tr><td>3. Test UEFI RT service get time interface, NULL time and NULL capabilities parameters</td></tr>
<tr><td>4. Test UEFI RT service set time interface</td></tr>
<tr><td>5. Test UEFI RT service set time interface, invalid year 1899</td></tr>
<tr><td>6. Test UEFI RT service set time interface, invalid year 10000</td></tr>
<tr><td>7. Test UEFI RT service set time interface, invalid month 0</td></tr>
<tr><td>8. Test UEFI RT service set time interface, invalid month 13</td></tr>
<tr><td>9. Test UEFI RT service set time interface, invalid day 0</td></tr>
<tr><td>10. Test UEFI RT service set time interface, invalid day 32</td></tr>
<tr><td>11. Test UEFI RT service set time interface, invalid hour 24</td></tr>
<tr><td>12. Test UEFI RT service set time interface, invalid minute 60</td></tr>
<tr><td>13. Test UEFI RT service set time interface, invalid second 60</td></tr>
<tr><td>14. Test UEFI RT service set time interface, invalid nanosecond 1000000000</td></tr>
<tr><td>15. Test UEFI RT service set time interface, invalid timezone -1441</td></tr>
<tr><td>16. Test UEFI RT service set time interface, invalid timezone 1441</td></tr>
<tr><td>17. Test UEFI RT service set time interface, invalid daylight 0xfc</td></tr>
<tr><td>18. Test UEFI RT service get wakeup time interface</td></tr>
<tr><td>19. Test UEFI RT service get wakeup time interface, NULL enabled parameter</td></tr>
<tr><td>20. Test UEFI RT service get wakeup time interface, NULL pending parameter</td></tr>
<tr><td>21. Test UEFI RT service get wakeup time interface, NULL time parameter</td></tr>
<tr><td>22. Test UEFI RT service get wakeup time interface, NULL enabled, pending and time parameters</td></tr>
<tr><td>23. Test UEFI RT service set wakeup time interface</td></tr>
<tr><td>24. Test UEFI RT service set wakeup time interface, NULL time parameter</td></tr>
<tr><td>25. Test UEFI RT service set wakeup time interface, invalid year 1899</td></tr>
<tr><td>26. Test UEFI RT service set wakeup time interface, invalid year 10000</td></tr>
<tr><td>27. Test UEFI RT service set wakeup time interface, invalid month 0</td></tr>
<tr><td>28. Test UEFI RT service set wakeup time interface, invalid month 13</td></tr>
<tr><td>29. Test UEFI RT service set wakeup time interface, invalid day 0</td></tr>
<tr><td>30. Test UEFI RT service set wakeup time interface, invalid day 32</td></tr>
<tr><td>31. Test UEFI RT service set wakeup time interface, invalid hour 24</td></tr>
<tr><td>32. Test UEFI RT service set wakeup time interface, invalid minute 60</td></tr>
<tr><td>33. Test UEFI RT service set wakeup time interface, invalid second 60</td></tr>
<tr><td>34. Test UEFI RT service set wakeup time interface, invalid nanosecond 1000000000</td></tr>
<tr><td>35. Test UEFI RT service set wakeup time interface, invalid timezone -1441</td></tr>
<tr><td>36. Test UEFI RT service set wakeup time interface, invalid timezone 1441</td></tr>
<tr><td>37. Test UEFI RT service set wakeup time interface, invalid daylight 0xfc</td></tr>
<tr><td>38. Test UEFI RT time services unsupported status</td></tr>

<tr><td rowspan=9 colspan=1>uefirtvariable<sup>[sbbr]</sup></td><td>1. Test UEFI RT service get variable interface</td></tr>
<tr><td>2. Test UEFI RT service get next variable name interface</td></tr>
<tr><td>3. Test UEFI RT service set variable interface</td></tr>
<tr><td>4. Test UEFI RT service query variable info interface</td></tr>
<tr><td>5. Test UEFI RT service variable interface stress test</td></tr>
<tr><td>6. Test UEFI RT service set variable interface stress test</td></tr>
<tr><td>7. Test UEFI RT service query variable info interface stress test</td></tr>
<tr><td>8. Test UEFI RT service get variable interface, invalid parameters</td></tr>
<tr><td>9. Test UEFI RT variable services unsupported status</td></tr>

<tr><td rowspan=1 colspan=1>xsdt<sup>[sbbr]</sup></td><td>1. XSDT Extended System Description Table test</td></tr>

</tbody>
</table>

# EBBR FWTS test-list

The table below lists the test modules executed for the EBBR recipe.

><p><b>Note:</b> X<sup>[ebbr]</sup> indicates that the FWTS test X belongs to the <b>EBBR</b> test category (executed as part of the <code>--ebbr</code> test-list).</p>

<table>
<thead><tr><th>Test</th><th>Minor Tests</th></tr></thead>
<tbody>

<tr><td rowspan=3 colspan=1>dt_base</td><td>1. Check device tree presence</td></tr>
<tr><td>2. Check device tree baseline validity</td></tr>
<tr><td>3. Check device tree warnings</td></tr>

<tr><td rowspan=1 colspan=1>esrt<sup>[ebbr]</sup></td><td>1. Sanity check UEFI ESRT Table</td></tr>

<tr><td rowspan=7 colspan=1>smccc</td><td>1. Test PCI_VERSION</td></tr>
<tr><td>2. Test PCI_FEATURES</td></tr>
<tr><td>3. Test PCI_GET_SEG_INFO</td></tr>
<tr><td>4. Test ARM_SMCCC_VERSION</td></tr>
<tr><td>5. Test ARM_SMCCC_ARCH_FEATURES</td></tr>
<tr><td>6. Test ARM_SMCCC_ARCH_SOC_ID for Soc_ID_type 0</td></tr>
<tr><td>7. Test ARM_SMCCC_ARCH_SOC_ID for Soc_ID_type 1</td></tr>

<tr><td rowspan=1 colspan=1>uefibootpath<sup>[ebbr]</sup></td><td>1. Test UEFI Boot Path Boot####</td></tr>

<tr><td rowspan=4 colspan=1>uefirtmisc<sup>[ebbr]</sup></td><td>1. Test for UEFI miscellaneous runtime service interfaces</td></tr>
<tr><td>2. Stress test for UEFI miscellaneous runtime service interfaces</td></tr>
<tr><td>3. Test GetNextHighMonotonicCount with invalid NULL parameter</td></tr>
<tr><td>4. Test UEFI miscellaneous runtime services unsupported status</td></tr>

<tr><td rowspan=38 colspan=1>uefirttime<sup>[ebbr]</sup></td><td>1. Test UEFI RT service get time interface</td></tr>
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

<tr><td rowspan=9 colspan=1>uefirtvariable<sup>[ebbr]</sup></td><td>1. Test UEFI RT service get variable interface</td></tr>
<tr><td>2. Test UEFI RT service get next variable name interface</td></tr>
<tr><td>3. Test UEFI RT service set variable interface</td></tr>
<tr><td>4. Test UEFI RT service query variable info interface</td></tr>
<tr><td>5. Test UEFI RT service variable interface stress test</td></tr>
<tr><td>6. Test UEFI RT service set variable interface stress test</td></tr>
<tr><td>7. Test UEFI RT service query variable info interface stress test</td></tr>
<tr><td>8. Test UEFI RT service get variable interface, invalid parameters</td></tr>
<tr><td>9. Test UEFI RT variable services unsupported status</td></tr>

<tr><td rowspan=1 colspan=1>uefivarinfo<sup>[ebbr]</sup></td><td>1. UEFI variable info query</td></tr>

</tbody>
</table>

# BBSR FWTS test-list

The table below lists the test modules executed for the BBSR recipe.
<table>
<thead><tr><th>Test</th><th>Minor Tests</th></tr></thead>
<tbody>

<tr><td rowspan=11 colspan=1>uefirtauthvar</td><td>1. Create authenticated variable test</td></tr>
<tr><td>2. Authenticated variable test with the same authenticated variable</td></tr>
<tr><td>3. Authenticated variable test with another valid authenticated variable</td></tr>
<tr><td>4. Append authenticated variable test</td></tr>
<tr><td>5. Update authenticated variable test</td></tr>
<tr><td>6. Authenticated variable test with old authenticated variable</td></tr>
<tr><td>7. Delete authenticated variable test</td></tr>
<tr><td>8. Authenticated variable test with invalid modified data</td></tr>
<tr><td>9. Authenticated variable test with invalid modified timestamp</td></tr>
<tr><td>10. Authenticated variable test with different guid</td></tr>
<tr><td>11. Set and delete authenticated variable created by different key test</td></tr>

<tr><td rowspan=1 colspan=1>tpm2</td><td>1. Validate TPM2 table</td></tr>

</tbody>
</table>

> **Note:** The `tpm2` FWTS test runs only on **ACPI-based systems**.


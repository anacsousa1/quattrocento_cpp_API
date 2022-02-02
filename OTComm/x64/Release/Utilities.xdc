<?xml version="1.0"?><doc>
<members>
<member name="T:OTComm.Utilities" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\utilities.h" line="8">
<summary>Class to help user to get info from OTComm</summary>
</member>
<member name="M:OTComm.Utilities.GetImageFromEmbeddedResource(System.String)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\utilities.h" line="13">
<summary>Get image from embedded resources</summary>
<param name="ResourceName">name of resource</param>
<returns>image associated to ResourceName</returns>
</member>
<member name="M:OTComm.Utilities.GetImageFromEmbeddedResource(System.String,System.Boolean)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\utilities.h" line="17">
<summary>Get image from embedded resources</summary>
<param name="ResourceName">name of resource</param>
<param name="LoadDefault">set to true if load default image when load ResourceNAme fails</param>
<returns>image associated to ResourceName</returns>
</member>
<member name="M:OTComm.Utilities.GetDefaultImage" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\utilities.h" line="22">
<summary>Get default image from embedded resources</summary>
<returns>default image</returns>
</member>
<member name="M:OTComm.Utilities.WriteToFile(System.String,System.String)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\utilities.h" line="25">
<summary>Write text to file</summary>
<param name="Text">text to write</param>
<param name="FileName">path to file to overwrite</param>
<returns>-1 if fails, 0 if everything is fine</returns>
</member>
<member name="M:OTComm.Utilities.GetEmbeddedResourceNames" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\utilities.h" line="30">
<summary>Get embedded resources</summary>
<returns>list of embedded resources</returns>
</member>
<member name="T:OTComm.DebugLog" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\debuglog.h" line="4">
<summary>Function to print messages to console during Debug</summary>
</member>
<member name="M:OTComm.DebugLog.Print(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\debuglog.h" line="8">
<summary>Function to print messages to console during Debug</summary>
<param name="str">Pointer to char to print</param>
</member>
<member name="M:OTComm.DebugLog.Print(System.String)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\debuglog.h" line="11">
<summary>Function to print messages to console during Debug</summary>
<param name="str">String to print</param>
</member>
<member name="M:OTComm.DebugLog.PrintLastError" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\debuglog.h" line="14">
<summary>Function to print last error to console during Debug</summary>
</member>
<member name="M:OTComm.DebugLog.PrintLastError(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\debuglog.h" line="16">
<summary>Function to print error to console during Debug</summary>
<param name="description">description</param>
<param name="function">function</param>
</member>
</members>
</doc>
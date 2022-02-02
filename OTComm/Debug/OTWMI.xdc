<?xml version="1.0"?><doc>
<members>
<member name="T:OTComm.OTWMI" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otwmi.h" line="5">
<summary>Functions helpfull for windows management instrumentation</summary>
</member>
<member name="M:OTComm.OTWMI.WindowsVersion" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otwmi.h" line="10">
<summary>Obtain Windows version</summary>
<returns>Windows version</returns>
</member>
<member name="M:OTComm.OTWMI.GetUSBSerialPort(System.String)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otwmi.h" line="13">
<summary>Obtain COMPORT connected to target devices connected via USB</summary>
<param name="target">Identity of device to discover</param>
<returns>list of COMPORT connected to devices specified by target</returns>
</member>
<member name="M:OTComm.OTWMI.GetUSBStandardSerialPort" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otwmi.h" line="17">
<summary>Obtain COMPORT connected to Forza or Quattro devices connected via USB</summary>
<returns>list of COMPORT connected to devices Quattro or Forza</returns>
</member>
<member name="M:OTComm.OTWMI.GetUSBDueProSerialPort" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otwmi.h" line="20">
<summary>Obtain COMPORT connected to DUEPRO devices connected via USB</summary>
<returns>list of COMPORT connected to devices DUEPRO</returns>
</member>
<member name="M:OTComm.OTWMI.GetPairedQuattro" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otwmi.h" line="23">
<summary>Obtain COMPORT or name of Quattro devices paired via BT</summary>
<returns>list of COMPORT or name of Quattro devices paired via BT</returns>
</member>
<member name="M:OTComm.OTWMI.GetPairedForza" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otwmi.h" line="26">
<summary>Obtain COMPORT or name of Forza devices paired via BT</summary>
<returns>list of COMPORT or name of Forza devices paired via BT</returns>
</member>
<member name="M:OTComm.OTWMI.GetPairedNOD" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otwmi.h" line="29">
<summary>Obtain COMPORT or name of NOD devices paired via BT</summary>
<returns>list of COMPORT or name of NOD devices paired via BT</returns>
</member>
<member name="M:OTComm.OTWMI.GetCaptionFromPort(System.String)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otwmi.h" line="32">
<summary>Get device name from COMPORT</summary>
<param name="port">COMPORT of device</param>
<returns>caption associated to COMPORT</returns>
</member>
<member name="M:OTComm.OTWMI.GetPortFromCaption(System.String)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otwmi.h" line="36">
<summary>Get device COMPORT from name</summary>
<param name="caption">name of device</param>
<returns>COMPORT associated to name</returns>
</member>
</members>
</doc>
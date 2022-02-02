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
<member name="T:OTComm.Muscles" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="7">
<summary>Muscles available</summary>
</member>
<member name="F:OTComm.Muscles.NotDefined" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="12">
<summary> Index:  0 - Muscle: Not defined</summary>
</member>
<member name="F:OTComm.Muscles.TemporalisAnterior" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="14">
<summary> Index:  1 - Muscle: Temporalis Anterior</summary>
</member>
<member name="F:OTComm.Muscles.SuperficialMasseter" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="16">
<summary> Index:  2 - Muscle: Superficial Masseter</summary>
</member>
<member name="F:OTComm.Muscles.SpleniusCapitis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="18">
<summary> Index:  3 - Muscle: Splenius Capitis</summary>
</member>
<member name="F:OTComm.Muscles.UpperTrapezius" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="20">
<summary> Index:  4 - Muscle: Upper Trapezius</summary>
</member>
<member name="F:OTComm.Muscles.MiddleTrapezius" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="22">
<summary> Index:  5 - Muscle: Middle Trapezius</summary>
</member>
<member name="F:OTComm.Muscles.LowerTrapezius" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="24">
<summary> Index:  6 - Muscle: Lower Trapezius</summary>
</member>
<member name="F:OTComm.Muscles.RhomboideusMajor" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="26">
<summary> Index:  7 - Muscle: Rhomboideus Major</summary>
</member>
<member name="F:OTComm.Muscles.RhomboideusMinor" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="28">
<summary> Index:  8 - Muscle: Rhomboideus Minor</summary>
</member>
<member name="F:OTComm.Muscles.AnteriorDeltoid" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="30">
<summary> Index:  9 - Muscle: Anterior Deltoid</summary>
</member>
<member name="F:OTComm.Muscles.PosteriorDeltoid" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="32">
<summary> Index: 10 - Muscle: Posterior Deltoid</summary>
</member>
<member name="F:OTComm.Muscles.LateralDeltoid" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="34">
<summary> Index: 11 - Muscle: Lateral Deltoid</summary>
</member>
<member name="F:OTComm.Muscles.Infraspinatus" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="36">
<summary> Index: 12 - Muscle: Infraspinatus</summary>
</member>
<member name="F:OTComm.Muscles.TeresMajor" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="38">
<summary> Index: 13 - Muscle: Teres Major</summary>
</member>
<member name="F:OTComm.Muscles.ErectorSpinae" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="40">
<summary> Index: 14 - Muscle: Erector Spinae</summary>
</member>
<member name="F:OTComm.Muscles.LatissimusDorsi" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="42">
<summary> Index: 15 - Muscle: Latissimus Dorsi</summary>
</member>
<member name="F:OTComm.Muscles.BicBrLongHead" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="44">
<summary> Index: 16 - Muscle: Biceps Brachii Long Head</summary>
</member>
<member name="F:OTComm.Muscles.BicBrShortHead" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="46">
<summary> Index: 17 - Muscle: Biceps Brachii Short Head</summary>
</member>
<member name="F:OTComm.Muscles.TricBrLatHead" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="48">
<summary> Index: 18 - Muscle: Triceps Brachii Lateral Head</summary>
</member>
<member name="F:OTComm.Muscles.TricBrMedHead" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="50">
<summary> Index: 19 - Muscle: Triceps Brachii Medial Head</summary>
</member>
<member name="F:OTComm.Muscles.PronatorTeres" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="52">
<summary> Index: 20 - Muscle: Pronator Teres</summary>
</member>
<member name="F:OTComm.Muscles.FlexCarpiRadial" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="54">
<summary> Index: 21 - Muscle: Flexor Carpi Radialis</summary>
</member>
<member name="F:OTComm.Muscles.FlexCarpiUlnaris" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="56">
<summary> Index: 22 - Muscle: Flexor Carpi Ulnaris</summary>
</member>
<member name="F:OTComm.Muscles.PalmarisLongus" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="58">
<summary> Index: 23 - Muscle: Palmaris Longus</summary>
</member>
<member name="F:OTComm.Muscles.ExtCarpiRadialis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="60">
<summary> Index: 24 - Muscle: Extensor Carpi Radialis</summary>
</member>
<member name="F:OTComm.Muscles.ExtCarpiUlnaris" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="62">
<summary> Index: 25 - Muscle: Extensor Carpi Ulnaris</summary>
</member>
<member name="F:OTComm.Muscles.ExtDigCommunis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="64">
<summary> Index: 26 - Muscle: Extensor Digitorum Communis</summary>
</member>
<member name="F:OTComm.Muscles.Brachioradialis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="66">
<summary> Index: 27 - Muscle: Brachioradialis</summary>
</member>
<member name="F:OTComm.Muscles.AbductorPollicisBrev" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="68">
<summary> Index: 28 - Muscle: Abductor Pollicis Brevis</summary>
</member>
<member name="F:OTComm.Muscles.AbductorPollicisLong" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="70">
<summary> Index: 29 - Muscle: Abductor Pollicis Longus</summary>
</member>
<member name="F:OTComm.Muscles.OpponensPollicis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="72">
<summary> Index: 30 - Muscle: Opponens Pollicis</summary>
</member>
<member name="F:OTComm.Muscles.AdductorPollicis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="74">
<summary> Index: 31 - Muscle: Adductor Pollicis</summary>
</member>
<member name="F:OTComm.Muscles.FlexPollBrevis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="76">
<summary> Index: 32 - Muscle: Flexor Pollicis Brevis</summary>
</member>
<member name="F:OTComm.Muscles.AbdDigitiMinimi" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="78">
<summary> Index: 33 - Muscle: Adbuctor Digiti Minimi</summary>
</member>
<member name="F:OTComm.Muscles.FlexDigitiMinimi" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="80">
<summary> Index: 34 - Muscle: Flexor Digiti Minimi</summary>
</member>
<member name="F:OTComm.Muscles.OppDigitiMinimi" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="82">
<summary> Index: 35 - Muscle: Opponens Digiti Minimi</summary>
</member>
<member name="F:OTComm.Muscles.DorsalInterossei" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="84">
<summary> Index: 36 - Muscle: Dorsal Interossei</summary>
</member>
<member name="F:OTComm.Muscles.PalmarInterossei" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="86">
<summary> Index: 37 - Muscle: Palmar Interossei</summary>
</member>
<member name="F:OTComm.Muscles.Lumbrical" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="88">
<summary> Index: 38 - Muscle: Lumbrical</summary>
</member>
<member name="F:OTComm.Muscles.RectusAbdominis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="90">
<summary> Index: 39 - Muscle: Rectus Abdominis</summary>
</member>
<member name="F:OTComm.Muscles.ExtAbdomObliq" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="92">
<summary> Index: 40 - Muscle: Abdominal External Oblique</summary>
</member>
<member name="F:OTComm.Muscles.SerratusAnterior" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="94">
<summary> Index: 41 - Muscle: Serratus Anterior</summary>
</member>
<member name="F:OTComm.Muscles.PectoralisMajor" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="96">
<summary> Index: 42 - Muscle: Pectoralis Major</summary>
</member>
<member name="F:OTComm.Muscles.SternocSterHead" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="98">
<summary> Index: 43 - Muscle: Sternocostal Head</summary>
</member>
<member name="F:OTComm.Muscles.SternocClavHead" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="100">
<summary> Index: 44 - Muscle: Sternoclavicular Head</summary>
</member>
<member name="F:OTComm.Muscles.AnteriorScalenus" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="102">
<summary> Index: 45 - Muscle: Anterior Scalenus</summary>
</member>
<member name="F:OTComm.Muscles.TensorFasciaLatae" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="104">
<summary> Index: 46 - Muscle: Tensor Fascia Latae</summary>
</member>
<member name="F:OTComm.Muscles.GastrocnLateralis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="106">
<summary> Index: 47 - Muscle: Gastrocnemio Lateralis</summary>
</member>
<member name="F:OTComm.Muscles.GastrocnMedialis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="108">
<summary> Index: 48 - Muscle: Gastrocnemio Medialis</summary>
</member>
<member name="F:OTComm.Muscles.BicepsFemoris" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="110">
<summary> Index: 49 - Muscle: Biceps Femoris</summary>
</member>
<member name="F:OTComm.Muscles.Soleus" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="112">
<summary> Index: 50 - Muscle: Soleus</summary>
</member>
<member name="F:OTComm.Muscles.Semitendinosus" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="114">
<summary> Index: 51 - Muscle: Semitendinosus</summary>
</member>
<member name="F:OTComm.Muscles.GluteusMaximus" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="116">
<summary> Index: 52 - Muscle: Gluteus Maximus</summary>
</member>
<member name="F:OTComm.Muscles.GluteusMedius" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="118">
<summary> Index: 53 - Muscle: Gluteus Medius</summary>
</member>
<member name="F:OTComm.Muscles.VastusLateralis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="120">
<summary> Index: 54 - Muscle: Vastus Lateralis</summary>
</member>
<member name="F:OTComm.Muscles.VastusMedialis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="122">
<summary> Index: 55 - Muscle: Vastus Medialis</summary>
</member>
<member name="F:OTComm.Muscles.RectusFemoris" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="124">
<summary> Index: 56 - Muscle: Rectus Femoris</summary>
</member>
<member name="F:OTComm.Muscles.TibialisAnterior" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="126">
<summary> Index: 57 - Muscle: Tibialis Anterior</summary>
</member>
<member name="F:OTComm.Muscles.PeroneusLongus" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="128">
<summary> Index: 58 - Muscle: Peroneus Longus</summary>
</member>
<member name="F:OTComm.Muscles.Semimembranosus" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="130">
<summary> Index: 59 - Muscle: Semimembranosus</summary>
</member>
<member name="F:OTComm.Muscles.Gracilis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="132">
<summary> Index: 60 - Muscle: Gracilis</summary>
</member>
<member name="F:OTComm.Muscles.ExtAnalSphincter" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="134">
<summary> Index: 61 - Muscle: External Anal Sphincter</summary>
</member>
<member name="F:OTComm.Muscles.Puborectalis" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="136">
<summary> Index: 62 - Muscle: Puborectalis</summary>
</member>
<member name="F:OTComm.Muscles.UrethralSphincter" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="138">
<summary> Index: 63 - Muscle: Urethral Sphincter</summary>
</member>
<member name="F:OTComm.Muscles.NotaMuscle" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="140">
<summary> Index: 64 - Muscle: Not a Muscle</summary>
</member>
<member name="M:OTComm.Muscles.Description(System.UInt32!System.Runtime.CompilerServices.IsConst)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="142">
<summary>Muscle Description</summary>
<param name="ID">index of muscle to translate</param>
<returns>description of muscle</returns>
</member>
<member name="M:OTComm.Muscles.ID(System.String)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="146">
<summary>Muscle ID</summary>
<param name="description">description of muscle to translate</param>
<returns>index of muscle</returns>
</member>
<member name="T:OTComm.MuscleSide" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="151">
<summary>Muscle sides available</summary>
</member>
<member name="F:OTComm.MuscleSide.Left" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="155">
<summary> Index: L - Side: Left</summary>
</member>
<member name="F:OTComm.MuscleSide.Undefined" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="157">
<summary> Index: - - Side: Undefined</summary>
</member>
<member name="F:OTComm.MuscleSide.Right" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="159">
<summary> Index: R - Side: Right</summary>
</member>
<member name="M:OTComm.MuscleSide.Description(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="161">
<summary>Side Description</summary>
<param name="ID">char of muscle to translate</param>
<returns>side of muscle</returns>
</member>
<member name="M:OTComm.MuscleSide.ID(System.String)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\muscles.h" line="165">
<summary>Muscle ID</summary>
<param name="description">side of muscle to translate</param>
<returns>char of muscle</returns>
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
<member name="T:OTComm.ErrorDescription" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\errordescription.h" line="6">
<summary>Description of Error</summary>
</member>
<member name="M:OTComm.ErrorDescription.GetMessage" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\errordescription.h" line="13">
<summary>Gets message of error</summary>
<returns>Return message of error</returns>
</member>
<member name="M:OTComm.ErrorDescription.GetFunction" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\errordescription.h" line="16">
<summary>Gets function of error</summary>
<returns>Return function of error</returns>
</member>
<member name="M:OTComm.ErrorDescription.GetCode" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\errordescription.h" line="19">
<summary>Gets code of error</summary>
<returns>Return code of error</returns>
</member>
<member name="M:OTComm.ErrorDescription.ToString" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\errordescription.h" line="22">
<summary>Gets message of error</summary>
<returns>Return message of error and code</returns>
</member>
<member name="T:OTComm.GenericChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="10">
<summary>Generic Channel</summary>
</member>
<member name="P:OTComm.GenericChannel.Gain" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="27">
<summary>Gain of the channel</summary>
</member>
<member name="P:OTComm.GenericChannel.LowPassFilter" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="32">
<summary>Low pass filter of the channel</summary>
</member>
<member name="P:OTComm.GenericChannel.HighPassFilter" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="37">
<summary>High pass filter of the channel</summary>
</member>
<member name="P:OTComm.GenericChannel.MuscleID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="42">
<summary>Muscle ID of the channel</summary>
</member>
<member name="P:OTComm.GenericChannel.EnableTrigger" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="49">
<summary>Aux channel as trigger</summary>
</member>
<member name="P:OTComm.GenericChannel.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="55">
<summary>Description of the channel</summary>
</member>
<member name="P:OTComm.GenericChannel.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="60">
<summary>ID of the channel</summary>
</member>
<member name="P:OTComm.GenericChannel.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="65">
<summary>Dimension of the channel</summary>
</member>
<member name="P:OTComm.GenericChannel.MuscleDescription" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="70">
<summary>Muscle description</summary>
</member>
<member name="P:OTComm.GenericChannel.MuscleSideDescription" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="76">
<summary>Muscle side description</summary>
</member>
<member name="P:OTComm.GenericChannel.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="82">
<summary>Image of the channel</summary>
</member>
<member name="P:OTComm.GenericChannel.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="84">
<summary>Image of the channel</summary>
</member>
<member name="P:OTComm.GenericChannel.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="86">
<summary>Image of the channel</summary>
</member>
<member name="P:OTComm.GenericChannel.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="88">
<summary>Image of the channel</summary>
</member>
<member name="P:OTComm.GenericChannel.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="90">
<summary>Remap of the sensor</summary>
</member>
<member name="P:OTComm.GenericChannel.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="92">
<summary>Remap of the sensor</summary>
</member>
<member name="M:OTComm.GenericChannel.#ctor(System.UInt32)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="97">
<summary>Default Constructor</summary>
<param name="ID">Muscle ID</param>
</member>
<member name="M:OTComm.GenericChannel.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="100">
<summary>Constructor with Gain=1 and without filter information</summary>
<param name="ID">Muscle ID</param>
<param name="side">Muscle side</param>
</member>
<member name="M:OTComm.GenericChannel.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte,System.Double)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="104">
<summary>Constructor without filter information</summary>
<param name="ID">Muscle ID</param>
<param name="side">Muscle side</param>
<param name="Gain">Gain</param>
</member>
<member name="M:OTComm.GenericChannel.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte,System.Double,System.Double,System.Double)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericchannel.h" line="109">
<param name="ID">Muscle ID</param>
<param name="side">Muscle side</param>
<param name="Gain">Gain</param>
<param name="LowPass">LowPass</param>
<param name="HighPass">HighPass</param>
</member>
<member name="T:OTComm.Sensors.SensorGeneralPurpose" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="15">
<summary>Sensor for generic device</summary>
</member>
<member name="P:OTComm.Sensors.SensorGeneralPurpose.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="20">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGeneralPurpose.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="23">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGeneralPurpose.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="26">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGeneralPurpose.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="29">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGeneralPurpose.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="32">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGeneralPurpose.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="35">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorGeneralPurpose.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="38">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGeneralPurpose.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="41">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGeneralPurpose.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="52">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorGeneralPurpose.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="62">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorGeneralPurpose.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="64">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorFormeter" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="69">
<summary>Sensor for Formeter device</summary>
</member>
<member name="P:OTComm.Sensors.SensorFormeter.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="73">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorFormeter.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="76">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorFormeter.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="79">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorFormeter.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="82">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorFormeter.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="85">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorFormeter.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="88">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorFormeter.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="91">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorFormeter.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="94">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorFormeter.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="105">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorFormeter.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="115">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorFormeter.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="117">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorOtto" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="122">
<summary>Sensor for Formeter device</summary>
</member>
<member name="P:OTComm.Sensors.SensorOtto.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="126">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorOtto.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="129">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorOtto.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="132">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorOtto.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="135">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorOtto.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="138">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorOtto.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="141">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorOtto.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="144">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorOtto.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="147">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorOtto.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="158">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorOtto.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="168">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorOtto.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="170">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorCoDe1_0" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="176">
<summary>Sensor CoDe 1</summary>
</member>
<member name="P:OTComm.Sensors.SensorCoDe1_0.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="180">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe1_0.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="183">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe1_0.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="186">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe1_0.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="189">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe1_0.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="192">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe1_0.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="195">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe1_0.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="198">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe1_0.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="201">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe1_0.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="212">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorCoDe1_0.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="222">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorCoDe1_0.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="224">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorCoDe2_0" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="228">
<summary>Sensor CoDe 2</summary>
</member>
<member name="P:OTComm.Sensors.SensorCoDe2_0.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="232">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe2_0.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="235">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe2_0.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="238">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe2_0.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="244">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe2_0.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="247">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe2_0.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="250">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe2_0.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="253">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCoDe2_0.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="264">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorCoDe2_0.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="274">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorCoDe2_0.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="276">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorDe1" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="280">
<summary>Sensor De 1</summary>
</member>
<member name="P:OTComm.Sensors.SensorDe1.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="284">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorDe1.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="287">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorDe1.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="290">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorDe1.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="293">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorDe1.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="296">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorDe1.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="299">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorDe1.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="302">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorDe1.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="305">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorDe1.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="316">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorDe1.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="326">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorDe1.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="328">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorCDE" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="332">
<summary>Sensor CDE</summary>
</member>
<member name="P:OTComm.Sensors.SensorCDE.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="336">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCDE.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="339">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCDE.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="342">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCDE.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="345">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCDE.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="348">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCDE.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="351">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorCDE.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="354">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCDE.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="357">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorCDE.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="368">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorCDE.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="378">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorCDE.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="380">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensoriEMGmonopolar" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="384">
<summary>Sensor intramuscolar monopolar</summary>
</member>
<member name="P:OTComm.Sensors.SensoriEMGmonopolar.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="388">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGmonopolar.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="391">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGmonopolar.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="394">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGmonopolar.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="397">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGmonopolar.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="400">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGmonopolar.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="403">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGmonopolar.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="406">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGmonopolar.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="409">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGmonopolar.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="420">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensoriEMGmonopolar.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="430">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensoriEMGmonopolar.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="432">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensoriEMGbipolar" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="436">
<summary>Sensor intramuscolar monopolar</summary>
</member>
<member name="P:OTComm.Sensors.SensoriEMGbipolar.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="440">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGbipolar.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="443">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGbipolar.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="446">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGbipolar.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="449">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGbipolar.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="452">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGbipolar.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="455">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGbipolar.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="458">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGbipolar.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="461">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensoriEMGbipolar.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="472">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensoriEMGbipolar.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="482">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensoriEMGbipolar.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="484">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH004" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="537">
<summary>Sensor ELSCH004</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH004.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="541">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH004.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="544">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH004.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="547">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH004.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="550">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH004.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="553">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH004.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="556">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH004.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="559">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH004.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="562">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH004.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="573">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH004.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="583">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH004.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="585">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH008" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="589">
<summary>Sensor ELSCH008</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH008.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="593">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH008.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="596">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH008.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="599">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH008.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="602">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH008.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="605">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH008.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="608">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH008.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="611">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH008.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="614">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH008.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="625">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH008.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="635">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH008.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="637">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorIMU" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="641">
<summary>Sensor ELSCH008</summary>
</member>
<member name="P:OTComm.Sensors.SensorIMU.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="645">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorIMU.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="648">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorIMU.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="651">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorIMU.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="654">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorIMU.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="657">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorIMU.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="660">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorIMU.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="663">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorIMU.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="666">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorIMU.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="677">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorIMU.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="687">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorIMU.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="689">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorSA8_10" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="693">
<summary>Sensor SA8_10</summary>
</member>
<member name="P:OTComm.Sensors.SensorSA8_10.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="697">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA8_10.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="700">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA8_10.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="703">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA8_10.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="706">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA8_10.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="709">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA8_10.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="712">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA8_10.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="715">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA8_10.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="718">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA8_10.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="729">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorSA8_10.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="739">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorSA8_10.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="741">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH016" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="745">
<summary>Sensor ELSCH016</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH016.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="749">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH016.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="752">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH016.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="755">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH016.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="758">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH016.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="761">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH016.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="764">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH016.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="767">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH016.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="770">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH016.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="781">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH016.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="791">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH016.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="793">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorSA16_2_5" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="797">
<summary>Sensor SA16_2_5</summary>
</member>
<member name="P:OTComm.Sensors.SensorSA16_2_5.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="801">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_2_5.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="804">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_2_5.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="807">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_2_5.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="810">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_2_5.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="813">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_2_5.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="816">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_2_5.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="819">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_2_5.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="822">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_2_5.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="833">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorSA16_2_5.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="843">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorSA16_2_5.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="845">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorSA16_5" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="849">
<summary>Sensor SA16_5</summary>
</member>
<member name="P:OTComm.Sensors.SensorSA16_5.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="853">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_5.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="856">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_5.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="859">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_5.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="862">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_5.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="865">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_5.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="868">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_5.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="871">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_5.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="874">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_5.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="885">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorSA16_5.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="895">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorSA16_5.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="897">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorSA16_10" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="901">
<summary>Sensor SA16_10</summary>
</member>
<member name="P:OTComm.Sensors.SensorSA16_10.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="905">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_10.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="908">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_10.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="911">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_10.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="914">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_10.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="917">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_10.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="920">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_10.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="923">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_10.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="926">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorSA16_10.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="937">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorSA16_10.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="947">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorSA16_10.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="949">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH032NM6" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="953">
<summary>Sensor ELSCH032NM6</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH032NM6.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="957">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH032NM6.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="960">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH032NM6.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="963">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH032NM6.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="966">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH032NM6.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="969">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH032NM6.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="972">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH032NM6.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="976">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH032NM6.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="992">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH032NM6.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1002">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH032NM6.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1004">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH064NM7" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1008">
<summary>Sensor ELSCH064NM5</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM7.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1012">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM7.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1015">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM7.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1018">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM7.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1021">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM7.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1024">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM7.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1027">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM7.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1030">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM7.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1033">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM7.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1058">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064NM7.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1067">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064NM7.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1069">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH031NM6" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1073">
<summary>Sensor ELSCH031NM6</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH031NM6.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1077">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH031NM6.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1080">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH031NM6.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1083">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH031NM6.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1086">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH031NM6.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1089">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH031NM6.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1092">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH031NM6.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1095">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH031NM6.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1098">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH031NM6.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1114">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH031NM6.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1124">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH031NM6.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1126">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH064R3S" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1130">
<summary>Sensor ELSCH064R3S</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064R3S.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1134">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064R3S.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1137">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064R3S.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1140">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064R3S.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1143">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064R3S.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1146">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064R3S.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1149">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064R3S.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1152">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064R3S.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1155">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064R3S.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1189">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064R3S.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1199">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064R3S.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1201">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="P:OTComm.Sensors.SensorGR08MM1305.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1209">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR08MM1305.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1212">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR08MM1305.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1215">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR08MM1305.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1218">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR08MM1305.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1221">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR08MM1305.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1224">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR08MM1305.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1227">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR08MM1305.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1230">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR08MM1305.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1255">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorGR08MM1305.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1265">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorGR08MM1305.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1267">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0808.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1275">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0808.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1278">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0808.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1281">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0808.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1284">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0808.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1287">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0808.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1290">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0808.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1293">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0808.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1296">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0808.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1315">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorGR10MM0808.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1325">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorGR10MM0808.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1327">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="P:OTComm.Sensors.SensorGR04MM1305.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1335">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR04MM1305.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1338">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR04MM1305.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1341">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR04MM1305.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1344">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR04MM1305.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1347">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR04MM1305.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1350">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR04MM1305.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1353">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR04MM1305.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1356">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR04MM1305.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1381">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorGR04MM1305.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1391">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorGR04MM1305.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1393">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0804.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1401">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0804.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1404">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0804.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1407">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0804.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1410">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0804.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1413">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0804.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1416">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0804.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1420">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorGR10MM0804.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1436">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorGR10MM0804.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1446">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorGR10MM0804.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1448">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH064NM5" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1454">
<summary>Sensor ELSCH064NM5</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM5.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1458">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM5.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1461">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM5.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1464">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM5.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1467">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM5.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1470">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM5.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1473">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM5.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1476">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM5.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1479">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM5.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1504">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064NM5.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1514">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064NM5.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1516">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH064NM4" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1520">
<summary>Sensor ELSCH064NM4</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM4.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1524">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM4.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1527">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM4.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1530">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM4.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1533">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM4.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1536">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM4.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1539">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM4.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1542">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM4.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1545">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM4.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1570">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064NM4.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1580">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064NM4.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1582">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH064NM3" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1586">
<summary>Sensor ELSCH064NM3</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM3.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1590">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM3.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1593">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM3.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1596">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM3.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1599">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM3.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1602">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM3.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1605">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM3.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1608">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM3.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1611">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM3.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1630">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064NM3.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1640">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064NM3.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1642">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH064NM2" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1646">
<summary>Sensor ELSCH064NM2</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM2.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1650">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM2.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1653">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM2.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1656">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM2.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1659">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM2.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1662">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM2.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1665">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM2.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1668">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM2.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1671">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064NM2.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1696">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064NM2.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1706">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064NM2.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1708">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.AUXAcc" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1712">
<summary>Sensor AUXAcc</summary>
</member>
<member name="P:OTComm.Sensors.AUXAcc.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1716">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXAcc.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1719">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXAcc.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1722">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXAcc.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1725">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXAcc.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1728">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXAcc.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1731">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.AUXAcc.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1734">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXAcc.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1737">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXAcc.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1748">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.AUXAcc.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1758">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.AUXAcc.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1760">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.AUXTor" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1764">
<summary>Sensor AUXTor</summary>
</member>
<member name="P:OTComm.Sensors.AUXTor.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1768">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTor.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1771">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTor.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1774">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTor.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1777">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTor.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1780">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTor.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1783">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.AUXTor.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1786">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTor.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1789">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTor.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1800">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.AUXTor.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1810">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.AUXTor.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1812">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.AUXFor" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1816">
<summary>Sensor AUXFor</summary>
</member>
<member name="P:OTComm.Sensors.AUXFor.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1820">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXFor.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1823">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXFor.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1826">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXFor.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1829">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXFor.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1832">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXFor.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1835">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.AUXFor.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1838">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXFor.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1841">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXFor.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1852">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.AUXFor.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1862">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.AUXFor.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1864">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.AUXTrig" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1868">
<summary>Sensor AUXFor</summary>
</member>
<member name="P:OTComm.Sensors.AUXTrig.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1872">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTrig.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1875">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTrig.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1878">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTrig.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1881">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTrig.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1884">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTrig.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1887">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.AUXTrig.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1890">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTrig.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1893">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.AUXTrig.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1904">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.AUXTrig.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1914">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.AUXTrig.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1916">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.RampChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1920">
<summary>Sensor AUXAcc</summary>
</member>
<member name="P:OTComm.Sensors.RampChannel.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1924">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.RampChannel.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1927">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.RampChannel.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1930">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.RampChannel.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1933">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.RampChannel.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1936">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.RampChannel.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1939">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.RampChannel.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1942">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.RampChannel.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1945">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.RampChannel.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1956">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.RampChannel.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1966">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.RampChannel.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1968">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.BufferChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1972">
<summary>Sensor AUXAcc</summary>
</member>
<member name="P:OTComm.Sensors.BufferChannel.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1976">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.BufferChannel.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1979">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.BufferChannel.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1982">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.BufferChannel.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1985">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.BufferChannel.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1988">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.BufferChannel.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1991">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.BufferChannel.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1994">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.BufferChannel.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="1997">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.BufferChannel.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2008">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.BufferChannel.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2018">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.BufferChannel.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2020">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH020EEG" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2024">
<summary>Sensor ELSCH064NM2</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH020EEG.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2028">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH020EEG.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2031">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH020EEG.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2034">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH020EEG.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2037">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH020EEG.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2040">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH020EEG.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2043">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH020EEG.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2046">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH020EEG.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2049">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH020EEG.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2066">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH020EEG.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2076">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH020EEG.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2078">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH064EEG" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2082">
<summary>Sensor ELSCH064NM2</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064EEG.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2086">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064EEG.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2089">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064EEG.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2092">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064EEG.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2095">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064EEG.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2098">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064EEG.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2101">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064EEG.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2104">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064EEG.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2107">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH064EEG.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2129">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064EEG.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2139">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH064EEG.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2141">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.Sensors.SensorELSCH024TEST" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2145">
<summary>Sensor ELSCH064NM2</summary>
</member>
<member name="P:OTComm.Sensors.SensorELSCH024TEST.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2149">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH024TEST.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2152">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH024TEST.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2155">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH024TEST.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2158">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH024TEST.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2161">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH024TEST.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2164">
<summary>Image of Sensor Matrix Channel</summary>
<returns>Image of Sensor Matrix Channel</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH024TEST.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2167">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH024TEST.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2170">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorELSCH024TEST.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2186">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorELSCH024TEST.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2196">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorELSCH024TEST.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2198">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="P:OTComm.Sensors.SensorMA05MM0804.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2206">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA05MM0804.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2209">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA05MM0804.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2212">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA05MM0804.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2215">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA05MM0804.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2218">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA05MM0804.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2221">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA05MM0804.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2225">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA05MM0804.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2241">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorMA05MM0804.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2251">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorMA05MM0804.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2253">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="P:OTComm.Sensors.SensorMA10MM0804.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2260">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA10MM0804.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2263">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA10MM0804.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2266">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA10MM0804.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2269">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA10MM0804.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2272">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA10MM0804.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2275">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA10MM0804.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2279">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA10MM0804.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2295">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorMA10MM0804.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2305">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorMA10MM0804.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2307">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="P:OTComm.Sensors.SensorMA20MM0804.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2314">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA20MM0804.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2317">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA20MM0804.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2320">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA20MM0804.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2323">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA20MM0804.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2326">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA20MM0804.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2329">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA20MM0804.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2333">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA20MM0804.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2349">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorMA20MM0804.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2359">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorMA20MM0804.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2361">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="P:OTComm.Sensors.SensorMA15MM1602.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2369">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA15MM1602.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2372">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA15MM1602.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2375">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA15MM1602.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2378">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA15MM1602.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2381">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA15MM1602.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2384">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA15MM1602.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2388">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorMA15MM1602.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2404">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorMA15MM1602.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2414">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorMA15MM1602.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2416">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="P:OTComm.Sensors.SensorAR05MM3201.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2424">
<summary>Description of Sensor</summary>
<returns>description of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorAR05MM3201.ChannelID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2427">
<summary>ID of Sensor</summary>
<returns>ID of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorAR05MM3201.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2430">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorAR05MM3201.ImageRotation" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2433">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorAR05MM3201.MapImage" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2436">
<summary>Image of Sensor</summary>
<returns>Image of Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorAR05MM3201.ImageMatrixChannel" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2439">
<summary>number of channel available for Sensor</summary>
<returns>number of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorAR05MM3201.Remap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2443">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="P:OTComm.Sensors.SensorAR05MM3201.MatrixChannelRemap" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2459">
<summary>remap of channel available for Sensor</summary>
<returns>remap of channel available for Sensor</returns>
</member>
<member name="M:OTComm.Sensors.SensorAR05MM3201.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2469">
<param name="ID">muscle ID</param>
</member>
<member name="M:OTComm.Sensors.SensorAR05MM3201.#ctor(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\sensors.h" line="2471">
<param name="ID">muscle ID</param>
<param name="side">muscle side</param>
</member>
<member name="T:OTComm.GenericAdapter" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericadapter.h" line="7">
<summary>Generic Adapter</summary>
</member>
<member name="P:OTComm.GenericAdapter.Channels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericadapter.h" line="14">
<summary>Get channels of adapter</summary>
<returns>the array of GenericChannels available for the adapter</returns>
</member>
<member name="P:OTComm.GenericAdapter.NumConnectedChannels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericadapter.h" line="23">
<summary>Get connected channels of adapter</summary>
<returns>the number of GenericChannels connected to the adapter</returns>
</member>
<member name="P:OTComm.GenericAdapter.Description" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericadapter.h" line="30">
<summary>Get the description of adapter</summary>
<returns>the description of the adapter</returns>
</member>
<member name="P:OTComm.GenericAdapter.ID" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericadapter.h" line="37">
<summary>Get the ID of adapter</summary>
<returns>the ID of the adapter</returns>
</member>
<member name="P:OTComm.GenericAdapter.MaxNumChannels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericadapter.h" line="44">
<summary>Get the maximum number of channels connectable to the adapter</summary>
<returns>the maximum number of channels connectable to the adapter</returns>
</member>
<member name="P:OTComm.GenericAdapter.MinSizeSensor" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericadapter.h" line="51">
<summary>Get the minimum size of sensor connectable to the adapter</summary>
<returns>the minimum size of sensor connectable to the adapter</returns>
</member>
<member name="P:OTComm.GenericAdapter.MaxSizeSensor" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericadapter.h" line="58">
<summary>Get the maximum size of sensor connectable to the adapter</summary>
<returns>the maximum size of sensor connectable to the adapter</returns>
</member>
<member name="P:OTComm.GenericAdapter.AdapterGain" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericadapter.h" line="65">
<summary>Get the gain of the adapter</summary>
<returns>the gain of the adapter</returns>
</member>
<member name="P:OTComm.GenericAdapter.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericadapter.h" line="72">
<summary>Get the image of the adapter</summary>
<returns>the image of the adapter</returns>
</member>
<member name="M:OTComm.GenericAdapter.SetChannel(System.UInt32!System.Runtime.CompilerServices.IsConst,OTComm.GenericChannel)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericadapter.h" line="76">
<summary>Set the channel of the adapter</summary>
<param name="position">coordinate of sensor in the adapter</param>
<param name="channel">sensor to connect to adapter</param>
</member>
<member name="M:OTComm.GenericAdapter.SetChannels(System.UInt32!System.Runtime.CompilerServices.IsConst,System.UInt32!System.Runtime.CompilerServices.IsConst,OTComm.GenericChannel)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericadapter.h" line="81">
<summary>Set the channel of the adapter</summary>
<param name="positionStart">first coordinate of sensor in the adapter</param>
<param name="positionEnd">last coordinate of sensor in the adapter</param>
<param name="channel">sensor to connect to adapter</param>
</member>
<member name="T:OTComm.GenericConnector" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericconnector.h" line="7">
<summary>Generic Connector</summary>
</member>
<member name="P:OTComm.GenericConnector.Adapter" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericconnector.h" line="22">
<summary> Adapter connected to Connector</summary>
</member>
<member name="M:OTComm.GenericConnector.#ctor" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericconnector.h" line="29">
<summary>Default constructor</summary>
</member>
<member name="M:OTComm.GenericConnector.#ctor(System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericconnector.h" line="34">
<summary>Constructor where you can set the number of connectable channels</summary>
<param name="numConnectableChannels">Number of connectable channels</param>
</member>
<member name="M:OTComm.GenericConnector.#ctor(System.UInt32,System.UInt32)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericconnector.h" line="39">
<summary>Constructor where you can set the number of connectable channels</summary>
<param name="numConnectableChannels">Number of connectable channels</param>
</member>
<member name="P:OTComm.GenericConnector.NumConnectableChannels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericconnector.h" line="48">
<summary>Connectable channels</summary>
</member>
<member name="P:OTComm.GenericConnector.NumConnectedChannels" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericconnector.h" line="56">
<summary>Connected channels</summary>
</member>
<member name="P:OTComm.GenericConnector.SourceIndex" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericconnector.h" line="62">
<summary>Connected channels</summary>
</member>
<member name="P:OTComm.GenericConnector.DetectionMode" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericconnector.h" line="68">
<summary>Detection Mode</summary>
</member>
<member name="P:OTComm.GenericConnector.LowPassFilter" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericconnector.h" line="75">
<summary>Low Pass Filter</summary>
</member>
<member name="P:OTComm.GenericConnector.HigPassFilter" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\genericconnector.h" line="82">
<summary>High Pass Filter</summary>
</member>
<member name="T:OTComm.DataTypes" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="109">
<summary>Frequencies available</summary>
</member>
<member name="T:OTComm.OpenDevice" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="124">
<summary>Delegate function when device opened</summary>
</member>
<member name="T:OTComm.Error" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="126">
<summary>Delegate function when error</summary>
</member>
<member name="T:OTComm.InitializeDevice" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="128">
<summary>Delegate function when device initialized</summary>
</member>
<member name="T:OTComm.StartAcquisition" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="130">
<summary>Delegate function when start acquisition</summary>
</member>
<member name="T:OTComm.NewDataAvailable" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="132">
<summary>Delegate function when new data available</summary>
</member>
<member name="T:OTComm.DeviceChanged" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="134">
<summary>Delegate function when device changed</summary>
</member>
<member name="T:OTComm.StopAcquisition" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="136">
<summary>Delegate function when stop acquisition</summary>
</member>
<member name="T:OTComm.StopAcquisitionRequested" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="138">
<summary>Delegate function when requested to stop acquisition is sent to HW</summary>
</member>
<member name="T:OTComm.StopInitialization" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="140">
<summary>Delegate function when stop initialization</summary>
</member>
<member name="T:OTComm.CloseDevice" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="142">
<summary>Delegate function when close device</summary>
</member>
<member name="T:OTComm.MarshalError" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="145">
<summary>Managed delegate function when error</summary>
</member>
<member name="T:OTComm.MarshalOpenDevice" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="147">
<summary>Managed delegate function when open device</summary>
</member>
<member name="T:OTComm.MarshalInitializeDevice" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="149">
<summary>Managed delegate function when initialize device</summary>
</member>
<member name="T:OTComm.MarshalStartAcquisition" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="151">
<summary>Managed delegate function when start acquisition</summary>
</member>
<member name="T:OTComm.MarshalNewDataAvailable" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="153">
<summary>Managed delegate function when new data available</summary>
</member>
<member name="T:OTComm.MarshalDeviceChanged" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="155">
<summary>Managed delegate function when device changed</summary>
</member>
<member name="T:OTComm.MarshalStopAcquisition" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="157">
<summary>Managed delegate function when stop acquisition</summary>
</member>
<member name="T:OTComm.MarshalStopAcquisitionRequested" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="159">
<summary>Managed delegate function when stop acquisition</summary>
</member>
<member name="T:OTComm.MarshalStopInitialization" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="161">
<summary>Managed delegate function when stop initialization</summary>
</member>
<member name="T:OTComm.MarshalCloseDevice" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\otcommtypes.h" line="163">
<summary>Managed delegate function when close device</summary>
</member>
<member name="T:OTComm.DataGeneratorInterface" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="37">
<summary>Abstract interface for DataGenerator</summary>
</member>
<member name="M:OTComm.DataGeneratorInterface.FromXMLConfiguration(System.String)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="133">
<summary> Creates DataGenerator from XML file</summary>
<param name="Filename">Path to XML file</param>
<returns>DataGeneratorInteface corresponding to XML file</returns>
</member>
<member name="P:OTComm.DataGeneratorInterface.AsyncNewDataEventRegroupFactor" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="137">
<summary>Asynchronous regroup factor for New Data Event</summary>
<param name="x">Asynchronous regroup factor</param>
<returns>Asynchronous regroup factor</returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.GetNumberSources" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="146">
<summary>Get the number of independent sources. This number is typically 1, it can be larger than 1 in aggregated devices</summary>
<returns>Total number of sources</returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.GetDeviceTotalChannels" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="149">
<summary>Get the number of channels available in the device</summary>
<returns>Total number of Device</returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.GetDeviceModel" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="152">
<summary>Get the model of the Device</summary>
<returns>Model of the Device</returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.GetAcquiredChannels" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="155">
<summary>Get the number of channels acquired by the device</summary>
<returns>Array with the index of acquired channels</returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.GetSamplingFrequency" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="159">
<summary>Get the sampling frequency of the device</summary>
<returns>Sampling frequency of Device</returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.GetBytesPerChannel" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="162">
<summary>Get the number of bytes per channel acquired by the device</summary>
<returns>the number of bytes per channel acquired by the device</returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.GetDeviceID" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="167">
<summary>Get the ID of the device</summary>
<returns>the ID of the device</returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.IsOpen" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="170">
<summary>Shows if the Device is open</summary>
<returns> true if Device is open, false if it is close </returns>
</member>
<member name="P:OTComm.DataGeneratorInterface.NewDataAvailableEventsPerSecond" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="173">
<summary>Regroup factor for having new data available</summary>
<returns> regroup factor </returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.GetDeviceDescription" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="181">
<summary>Get description of Device</summary>
<returns> device description </returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.GetDeviceXMLConfiguration" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="184">
<summary>Get XML Configuration of Device</summary>
<returns> device XML configuration </returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.GetSavingXMLConfiguration" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="187">
<summary>Get XML configuration of saved data</summary>
<returns> saved data XML configuration </returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.OpenDevice" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="191">
<summary>Open the device</summary>
</member>
<member name="M:OTComm.DataGeneratorInterface.InitializeDevice" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="193">
<summary>Initialize the device</summary>
</member>
<member name="M:OTComm.DataGeneratorInterface.StartAcquisition" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="195">
<summary>Start acquisition from the device</summary>
</member>
<member name="M:OTComm.DataGeneratorInterface.StopAcquisition" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="198">
<summary>Stop acquisition from the device</summary>
</member>
<member name="M:OTComm.DataGeneratorInterface.StopInitialization" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="200">
<summary>Stop initialization the device</summary>
</member>
<member name="M:OTComm.DataGeneratorInterface.CloseDevice" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="202">
<summary>Close the device</summary>
</member>
<member name="M:OTComm.DataGeneratorInterface.AddDataConsumer(OTComm.DataConsumerInterface)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="205">
<summary>
Add data consumer to generator
</summary>
<param name="du">DataConsumerInterface containing the consumer</param>
</member>
<member name="P:OTComm.DataGeneratorInterface.Image" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="212">
<summary>Image of the device</summary>
<returns>image of the device</returns>
</member>
<member name="M:OTComm.DataGeneratorInterface.GetConnectors" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="216">
<summary>Get Connectors of Device</summary>
<returns>Array of GenericConnectors available for the device</returns>
</member>
<member name="P:OTComm.DataGeneratorInterface.IsAcquiring" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datageneratorinterface.h" line="220">
<summary>Get if Data Generator is acquiring</summary>
<returns>True if data generator is acquiring, false if it is not acquiring</returns>
</member>
<member name="T:OTComm.DataValidator" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datavalidator.h" line="34">
<summary>Data validator. Check if a condition is satisfied before perform operations</summary>
</member>
<member name="M:OTComm.DataValidator.Reset" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datavalidator.h" line="44">
<summary>Reset the Data Validator</summary>
</member>
<member name="M:OTComm.DataValidator.Initialize(OTComm.DataGeneratorInterface)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datavalidator.h" line="46">
<summary>Initialize a DataValidator object.</summary>
<returns>True if initialization succeeds, False if not</returns>
</member>
<member name="M:OTComm.DataValidator.Validate(OTComm.DataGeneratorInterface,OTComm.OTData)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\datavalidator.h" line="49">
<summary>Validate Data validator.</summary>
<returns></returns>
</member>
<member name="T:OTComm.DataConsumerInterface" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\dataconsumerinterface.h" line="6">
<summary>
Abstract class to control all consumers
</summary>
</member>
<member name="F:OTComm.DataConsumerInterface.mDataGenerator" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\dataconsumerinterface.h" line="12">
<summary>
DataGenerator associated to the consumer
</summary>
</member>
<member name="M:OTComm.DataConsumerInterface.Register(OTComm.DataGeneratorInterface,System.Int16)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\dataconsumerinterface.h" line="25">
<summary>
Register the consumer to the device
</summary>
<param name="gen">DataGeneratorInterface to register</param>
</member>
<member name="M:OTComm.DataConsumerInterface.Unregister(OTComm.DataGeneratorInterface)" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\dataconsumerinterface.h" line="30">
<summary>
Unregister the consumer to the device
</summary>
<param name="gen">DataGeneratorInterface to unregister</param>
</member>
<member name="M:OTComm.DataConsumerInterface.GetConsumerDescription" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\dataconsumerinterface.h" line="35">
<summary>
Description of consumer
</summary>
<returns> Returns the name of the consumer</returns>
</member>
<member name="M:OTComm.DataConsumerInterface.GetConsumerXMLConfiguration" decl="false" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\dataconsumerinterface.h" line="40">
<summary>
XML Configuration of consumer
</summary>
<returns> Returns the xml configuration of the consumer</returns>
</member>
<member name="M:OTComm.DataConsumerInterface.GetDeviceDescription" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\dataconsumerinterface.h" line="45">
<summary>
Description of device associated to consumer
</summary>
<returns> Returns the name of the device associated to the consumer</returns>
</member>
<member name="M:OTComm.DataConsumerInterface.GetDeviceXMLConfiguration" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\dataconsumerinterface.h" line="50">
<summary>
XML Configuration of Device associated to consumer
</summary>
<returns> Returns the XML Configuration of Device associated to consumer</returns>
</member>
<member name="M:OTComm.DataConsumerInterface.SetDataGeneratorInterface(OTComm.DataGeneratorInterface,System.Int16)" decl="true" source="c:\lavoro\git\__customerservice__\ot biolab+\otcomm\dataconsumerinterface.h" line="58">
<summary>
Set DatageneratorInterface associated to DataConsumerInterface
</summary>
<param name="dgi">DataGeneratorInterface to set</param>
</member>
</members>
</doc>
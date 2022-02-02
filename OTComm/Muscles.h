#pragma once
using namespace System;
namespace OTComm
{
	typedef unsigned int MuscleID;
	typedef char SideID;
	///<summary>Muscles available</summary>
	public ref class  Muscles
	{

	public:
		///<summary> Index:  0 - Muscle: Not defined</summary>
		static const unsigned int NotDefined = 0;
		///<summary> Index:  1 - Muscle: Temporalis Anterior</summary>
		static const unsigned int TemporalisAnterior = 1;
		///<summary> Index:  2 - Muscle: Superficial Masseter</summary>
		static const unsigned int SuperficialMasseter = 2;
		///<summary> Index:  3 - Muscle: Splenius Capitis</summary>
		static const unsigned int SpleniusCapitis = 3;
		///<summary> Index:  4 - Muscle: Upper Trapezius</summary>
		static const unsigned int UpperTrapezius = 4;
		///<summary> Index:  5 - Muscle: Middle Trapezius</summary>
		static const unsigned int MiddleTrapezius = 5;
		///<summary> Index:  6 - Muscle: Lower Trapezius</summary>
		static const unsigned int LowerTrapezius = 6;
		///<summary> Index:  7 - Muscle: Rhomboideus Major</summary>
		static const unsigned int RhomboideusMajor = 7;
		///<summary> Index:  8 - Muscle: Rhomboideus Minor</summary>
		static const unsigned int RhomboideusMinor = 8;
		///<summary> Index:  9 - Muscle: Anterior Deltoid</summary>
		static const unsigned int AnteriorDeltoid = 9;
		///<summary> Index: 10 - Muscle: Posterior Deltoid</summary>
		static const unsigned int PosteriorDeltoid = 10;
		///<summary> Index: 11 - Muscle: Lateral Deltoid</summary>
		static const unsigned int LateralDeltoid = 11;
		///<summary> Index: 12 - Muscle: Infraspinatus</summary>
		static const unsigned int Infraspinatus = 12;
		///<summary> Index: 13 - Muscle: Teres Major</summary>
		static const unsigned int TeresMajor = 13;
		///<summary> Index: 14 - Muscle: Erector Spinae</summary>
		static const unsigned int ErectorSpinae = 14;
		///<summary> Index: 15 - Muscle: Latissimus Dorsi</summary>
		static const unsigned int LatissimusDorsi = 15;
		///<summary> Index: 16 - Muscle: Biceps Brachii Long Head</summary>
		static const unsigned int BicBrLongHead = 16;
		///<summary> Index: 17 - Muscle: Biceps Brachii Short Head</summary>
		static const unsigned int BicBrShortHead = 17;
		///<summary> Index: 18 - Muscle: Triceps Brachii Lateral Head</summary>
		static const unsigned int TricBrLatHead = 18;
		///<summary> Index: 19 - Muscle: Triceps Brachii Medial Head</summary>
		static const unsigned int TricBrMedHead = 19;
		///<summary> Index: 20 - Muscle: Pronator Teres</summary>
		static const unsigned int PronatorTeres = 20;
		///<summary> Index: 21 - Muscle: Flexor Carpi Radialis</summary>
		static const unsigned int FlexCarpiRadial = 21;
		///<summary> Index: 22 - Muscle: Flexor Carpi Ulnaris</summary>
		static const unsigned int FlexCarpiUlnaris = 22;
		///<summary> Index: 23 - Muscle: Palmaris Longus</summary>
		static const unsigned int PalmarisLongus = 23;
		///<summary> Index: 24 - Muscle: Extensor Carpi Radialis</summary>
		static const unsigned int ExtCarpiRadialis = 24;
		///<summary> Index: 25 - Muscle: Extensor Carpi Ulnaris</summary>
		static const unsigned int ExtCarpiUlnaris = 25;
		///<summary> Index: 26 - Muscle: Extensor Digitorum Communis</summary>
		static const unsigned int ExtDigCommunis = 26;
		///<summary> Index: 27 - Muscle: Brachioradialis</summary>
		static const unsigned int Brachioradialis = 27;
		///<summary> Index: 28 - Muscle: Abductor Pollicis Brevis</summary>
		static const unsigned int AbductorPollicisBrev = 28;
		///<summary> Index: 29 - Muscle: Abductor Pollicis Longus</summary>
		static const unsigned int AbductorPollicisLong = 29;
		///<summary> Index: 30 - Muscle: Opponens Pollicis</summary>
		static const unsigned int OpponensPollicis = 30;
		///<summary> Index: 31 - Muscle: Adductor Pollicis</summary>
		static const unsigned int AdductorPollicis = 31;
		///<summary> Index: 32 - Muscle: Flexor Pollicis Brevis</summary>
		static const unsigned int FlexPollBrevis = 32;
		///<summary> Index: 33 - Muscle: Adbuctor Digiti Minimi</summary>
		static const unsigned int AbdDigitiMinimi = 33;
		///<summary> Index: 34 - Muscle: Flexor Digiti Minimi</summary>
		static const unsigned int FlexDigitiMinimi = 34;
		///<summary> Index: 35 - Muscle: Opponens Digiti Minimi</summary>
		static const unsigned int OppDigitiMinimi = 35;
		///<summary> Index: 36 - Muscle: Dorsal Interossei</summary>
		static const unsigned int DorsalInterossei = 36;
		///<summary> Index: 37 - Muscle: Palmar Interossei</summary>
		static const unsigned int PalmarInterossei = 37;
		///<summary> Index: 38 - Muscle: Lumbrical</summary>
		static const unsigned int Lumbrical = 38;
		///<summary> Index: 39 - Muscle: Rectus Abdominis</summary>
		static const unsigned int RectusAbdominis = 39;
		///<summary> Index: 40 - Muscle: Abdominal External Oblique</summary>
		static const unsigned int ExtAbdomObliq = 40;
		///<summary> Index: 41 - Muscle: Serratus Anterior</summary>
		static const unsigned int SerratusAnterior = 41;
		///<summary> Index: 42 - Muscle: Pectoralis Major</summary>
		static const unsigned int PectoralisMajor = 42;
		///<summary> Index: 43 - Muscle: Sternocostal Head</summary>
		static const unsigned int SternocSterHead = 43;
		///<summary> Index: 44 - Muscle: Sternoclavicular Head</summary>
		static const unsigned int SternocClavHead = 44;
		///<summary> Index: 45 - Muscle: Anterior Scalenus</summary>
		static const unsigned int AnteriorScalenus = 45;
		///<summary> Index: 46 - Muscle: Tensor Fascia Latae</summary>
		static const unsigned int TensorFasciaLatae = 46;
		///<summary> Index: 47 - Muscle: Gastrocnemio Lateralis</summary>
		static const unsigned int GastrocnLateralis = 47;
		///<summary> Index: 48 - Muscle: Gastrocnemio Medialis</summary>
		static const unsigned int GastrocnMedialis = 48;
		///<summary> Index: 49 - Muscle: Biceps Femoris</summary>
		static const unsigned int BicepsFemoris = 49;
		///<summary> Index: 50 - Muscle: Soleus</summary>
		static const unsigned int Soleus = 50;
		///<summary> Index: 51 - Muscle: Semitendinosus</summary>
		static const unsigned int Semitendinosus = 51;
		///<summary> Index: 52 - Muscle: Gluteus Maximus</summary>
		static const unsigned int GluteusMaximus = 52;
		///<summary> Index: 53 - Muscle: Gluteus Medius</summary>
		static const unsigned int GluteusMedius = 53;
		///<summary> Index: 54 - Muscle: Vastus Lateralis</summary>
		static const unsigned int VastusLateralis = 54;
		///<summary> Index: 55 - Muscle: Vastus Medialis</summary>
		static const unsigned int VastusMedialis = 55;
		///<summary> Index: 56 - Muscle: Rectus Femoris</summary>
		static const unsigned int RectusFemoris = 56;
		///<summary> Index: 57 - Muscle: Tibialis Anterior</summary>
		static const unsigned int TibialisAnterior = 57;
		///<summary> Index: 58 - Muscle: Peroneus Longus</summary>
		static const unsigned int PeroneusLongus = 58;
		///<summary> Index: 59 - Muscle: Semimembranosus</summary>
		static const unsigned int Semimembranosus = 59;
		///<summary> Index: 60 - Muscle: Gracilis</summary>
		static const unsigned int Gracilis = 60;
		///<summary> Index: 61 - Muscle: External Anal Sphincter</summary>
		static const unsigned int ExtAnalSphincter = 61;
		///<summary> Index: 62 - Muscle: Puborectalis</summary>
		static const unsigned int Puborectalis = 62;
		///<summary> Index: 63 - Muscle: Urethral Sphincter</summary>
		static const unsigned int UrethralSphincter = 63;
		///<summary> Index: 64 - Muscle: Not a Muscle</summary>
		static const unsigned int NotaMuscle = 64;
		///<summary>Muscle Description</summary>
		///<param name="ID">index of muscle to translate</param>
		///<returns>description of muscle</returns>
		static String^ Description(const unsigned int ID);
		///<summary>Muscle ID</summary>
		///<param name="description">description of muscle to translate</param>
		///<returns>index of muscle</returns>
		static unsigned int ID(String^ description);
	};
	///<summary>Muscle sides available</summary>
	public ref class MuscleSide
	{
	public:
		///<summary> Index: L - Side: Left</summary>
		static const char Left = 'L';
		///<summary> Index: - - Side: Undefined</summary>
		static const char Undefined = '-';
		///<summary> Index: R - Side: Right</summary>
		static const char Right = 'R';
		///<summary>Side Description</summary>
		///<param name="ID">char of muscle to translate</param>
		///<returns>side of muscle</returns>
		static String^ Description(const char ID);
		///<summary>Muscle ID</summary>
		///<param name="description">side of muscle to translate</param>
		///<returns>char of muscle</returns>
		static char ID(String^ description);
	};
}
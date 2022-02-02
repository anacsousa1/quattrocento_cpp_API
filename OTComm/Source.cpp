using namespace System;
namespace OTComm
{

	public ref class  Muscles
	{
		static const unsigned int NotDefined = 0;
		static const unsigned int TemporalisAnterior = 1;
		static const unsigned int SuperficialMasseter = 2;
		static const unsigned int SpleniusCapitis = 3;
		static const unsigned int UpperTrapezius = 4;
		static const unsigned int MiddleTrapezius = 5;
		static const unsigned int LowerTrapezius = 6;
		static const unsigned int RhomboideusMajor = 7;
		static const unsigned int RhomboideusMinor = 8;
		static const unsigned int AnteriorDeltoid = 9;
		static const unsigned int PosteriorDeltoid = 10;
		static const unsigned int LateralDeltoid = 11;
		static const unsigned int Infraspinatus = 12;
		static const unsigned int TeresMajor = 13;
		static const unsigned int ErectorSpinae = 14;
		static const unsigned int LatissimusDorsi = 15;
		static const unsigned int BicBrLongHead = 16;
		static const unsigned int BicBrShortHead = 17;
		static const unsigned int TricBrLatHead = 18;
		static const unsigned int TricBrMedHead = 19;
		static const unsigned int PronatorTeres = 20;
		static const unsigned int FlexCarpiRadial = 21;
		static const unsigned int FlexCarpiUlnaris = 22;
		static const unsigned int PalmarisLongus = 23;
		static const unsigned int ExtCarpiRadialis = 24;
		static const unsigned int ExtCarpiUlnaris = 25;
		static const unsigned int ExtDigCommunis = 26;
		static const unsigned int Brachioradialis = 27;
		static const unsigned int AbductorPollicisBrev = 28;
		static const unsigned int AbductorPollicisLong = 29;
		static const unsigned int OpponensPollicis = 30;
		static const unsigned int AdductorPollicis = 31;
		static const unsigned int FlexPollBrevis = 32;
		static const unsigned int AbdDigitiMinimi = 33;
		static const unsigned int FlexDigitiMinimi = 34;
		static const unsigned int OppDigitiMinimi = 35;
		static const unsigned int DorsalInterossei = 36;
		static const unsigned int PalmarInterossei = 37;
		static const unsigned int Lumbrical = 38;
		static const unsigned int RectusAbdominis = 39;
		static const unsigned int ExtAbdomObliq = 40;
		static const unsigned int SerratusAnterior = 41;
		static const unsigned int PectoralisMajor = 42;
		static const unsigned int SternocSterHead = 43;
		static const unsigned int SternocClavHead = 44;
		static const unsigned int AnteriorScalenus = 45;
		static const unsigned int TensorFasciaLatae = 46;
		static const unsigned int GastrocnLateralis = 47;
		static const unsigned int GastrocnMedialis = 48;
		static const unsigned int BicepsFemoris = 49;
		static const unsigned int Soleus = 50;
		static const unsigned int Semitendinosus = 51;
		static const unsigned int GluteusMaximus = 52;
		static const unsigned int GluteusMedius = 53;
		static const unsigned int VastusLateralis = 54;
		static const unsigned int VastusMedialis = 55;
		static const unsigned int RectusFemoris = 56;
		static const unsigned int TibialisAnterior = 57;
		static const unsigned int PeroneusLongus = 58;
		static const unsigned int Semimembranosus = 59;
		static const unsigned int Gracilis = 60;
		static const unsigned int ExtAnalSphincter = 61;
		static const unsigned int Puborectalis = 62;
		static const unsigned int UrethralSphincter = 63;
		static const unsigned int NotaMuscle = 64;

		static String^ Description(unsigned int ID)
		{
			switch (ID)
			{
			case 0:
			default:
				return "Not defined";
			case 1:
				return "Temporalis Anterior";
			}

		}
	};
}
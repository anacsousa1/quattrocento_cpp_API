#include "Muscles.h"


namespace OTComm
{
	String^ Muscles::Description(const unsigned int ID)
	{
		switch (ID)
		{
		case 0:				return "Not defined";
		case 1:				return "Temporalis Anterior";
		case 2:				return "Superficial Masseter";
		case 3:				return "Splenius Capitis";
		case 4:				return "Upper Trapezius";
		case 5:				return "Middle Trapezius";
		case 6:				return "Lower Trapezius";
		case 7:				return "Rhomboideus Major";
		case 8:				return "Rhomboideus Minor";
		case 9:				return "Anterior Deltoid";
		case 10:			return "Posterior Deltoid";
		case 11:			return "Lateral Deltoid";
		case 12:			return "Infraspinatus";
		case 13:			return "Teres Major";
		case 14:			return "Erector Spinae";
		case 15:			return "Latissimus Dorsi";
		case 16:			return "Biceps Brachii Long Head";
		case 17:			return "Biceps Brachii Short Head";
		case 18:			return "Triceps Brachii Lateral Head";
		case 19:			return "Triceps Brachii Medial Head";
		case 20:			return "Pronator Teres";
		case 21:			return "Flexor Carpi Radialis";
		case 22:			return "Flexor Carpi Ulnaris";
		case 23:			return "Palmaris Longus";
		case 24:			return "Extensor Carpi Radialis";
		case 25:			return "Extensor Carpi Ulnaris";
		case 26:			return "Extensor Digitorum Communis";
		case 27:			return "Brachioradialis";
		case 28:			return "Abductor Pollicis Brevis";
		case 29:			return "Abductor Pollicis Longus";
		case 30:			return "Opponens Pollicis";
		case 31:			return "Adductor Pollicis";
		case 32:			return "Flexor Pollicis Brevis";
		case 33:			return "Adbuctor Digiti Minimi";
		case 34:			return "Flexor Digiti Minimi";
		case 35:			return "Opponens Digiti Minimi";
		case 36:			return "Dorsal Interossei";
		case 37:			return "Palmar Interossei";
		case 38:			return "Lumbrical";
		case 39:			return "Rectus Abdominis";
		case 40:			return "Abdominal External Oblique";
		case 41:			return "Serratus Anterior";
		case 42:			return "Pectoralis Major";
		case 43:			return "Sternocostal Head";
		case 44:			return "Sternoclavicular Head";
		case 45:			return "Anterior Scalenus";
		case 46:			return "Tensor Fascia Latae";
		case 47:			return "Gastrocnemio Lateralis";
		case 48:			return "Gastrocnemio Medialis";
		case 49:			return "Biceps Femoris";
		case 50:			return "Soleus";
		case 51:			return "Semitendinosus";
		case 52:			return "Gluteus Maximus";
		case 53:			return "Gluteus Medius";
		case 54:			return "Vastus Lateralis";
		case 55:			return "Vastus Medialis";
		case 56:			return "Rectus Femoris";
		case 57:			return "Tibialis Anterior";
		case 58:			return "Peroneus Longus";
		case 59:			return "Semimembranosus";
		case 60:			return "Gracilis";
		case 61:			return "External Anal Sphincter";
		case 62:			return "Puborectalis";
		case 63:			return "Urethral Sphincter";
		case 64:			return "Abductor Hallucis";
		case 65:			return "Not a Muscle";
		default:			return "Not defined";
		}

	}

	String^ MuscleSide::Description(const char ID)
	{
		switch (ID)
		{
		case 'L':			return "Left";
		case'-':
		default:			return "Not Defined";
		case 'R':			return "Right";
		}
	}

	unsigned int Muscles::ID(String^ description)
	{
		if (description == "Not defined") return 0;
		if (description == "Temporalis Anterior") return 1;
		if (description == "Superficial Masseter") return 2;
		if (description == "Splenius Capitis") return 3;
		if (description == "Upper Trapezius") return 4;
		if (description == "Middle Trapezius") return 5;
		if (description == "Lower Trapezius") return 6;
		if (description == "Rhomboideus Major") return 7;
		if (description == "Rhomboideus Minor") return 8;
		if (description == "Anterior Deltoid") return 9;
		if (description == "Posterior Deltoid") return 10;
		if (description == "Lateral Deltoid") return 11;
		if (description == "Infraspinatus") return 12;
		if (description == "Teres Major") return 13;
		if (description == "Erector Spinae") return 14;
		if (description == "Latissimus Dorsi") return 15;
		if (description == "Biceps Brachii Long Head") return 16;
		if (description == "Biceps Brachii Short Head") return 17;
		if (description == "Triceps Brachii Lateral Head") return 18;
		if (description == "Triceps Brachii Medial Head") return 19;
		if (description == "Pronator Teres") return 20;
		if (description == "Flexor Carpi Radialis") return 21;
		if (description == "Flexor Carpi Ulnaris") return 22;
		if (description == "Palmaris Longus") return 23;
		if (description == "Extensor Carpi Radialis") return 24;
		if (description == "Extensor Carpi Ulnaris") return 25;
		if (description == "Extensor Digitorum Communis") return 26;
		if (description == "Brachioradialis") return 27;
		if (description == "Abductor Pollicis Brevis") return 28;
		if (description == "Abductor Pollicis Longus") return 29;
		if (description == "Opponens Pollicis") return 30;
		if (description == "Adductor Pollicis") return 31;
		if (description == "Flexor Pollicis Brevis") return 32;
		if (description == "Adbuctor Digiti Minimi") return 33;
		if (description == "Flexor Digiti Minimi") return 34;
		if (description == "Opponens Digiti Minimi") return 35;
		if (description == "Dorsal Interossei") return 36;
		if (description == "Palmar Interossei") return 37;
		if (description == "Lumbrical") return 38;
		if (description == "Rectus Abdominis") return 39;
		if (description == "Abdominal External Oblique") return 40;
		if (description == "Serratus Anterior") return 41;
		if (description == "Pectoralis Major") return 42;
		if (description == "Sternocostal Head") return 43;
		if (description == "Sternoclavicular Head") return 44;
		if (description == "Anterior Scalenus") return 45;
		if (description == "Tensor Fascia Latae") return 46;
		if (description == "Gastrocnemio Lateralis") return 47;
		if (description == "Gastrocnemio Medialis") return 48;
		if (description == "Biceps Femoris") return 49;
		if (description == "Soleus") return 50;
		if (description == "Semitendinosus") return 51;
		if (description == "Gluteus Maximus") return 52;
		if (description == "Gluteus Medius") return 53;
		if (description == "Vastus Lateralis") return 54;
		if (description == "Vastus Medialis") return 55;
		if (description == "Rectus Femoris") return 56;
		if (description == "Tibialis Anterior") return 57;
		if (description == "Peroneus Longus") return 58;
		if (description == "Semimembranosus") return 59;
		if (description == "Gracilis") return 60;
		if (description == "External Anal Sphincter") return 61;
		if (description == "Puborectalis") return 62;
		if (description == "Urethral Sphincter") return 63;
		if (description == "Abductor Hallucis") return 64;
		if (description == "Not a Muscle") return 65;
		return 0;
	}
	char MuscleSide::ID(String^ description)
	{
		if (description == "Left") return 'L';
		if (description == "Right") return 'R';
		return '-';
	}
}

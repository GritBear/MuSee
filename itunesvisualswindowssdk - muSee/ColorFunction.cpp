#include "Colors.h"
Color3 selectColor(blendColorMode theColor){
	switch(theColor){
	case NoColor:
		return noColor;
	case brightGreenBlue:
		return bewitchedTree;
	case darkGreenblue:
		return mysticalGreen;
	case lightGreenblue:
		return lightHeartBlue;
	case whiteBlue:
		return glassGall;
	case warmGreen:
		return sillyFizz;
	case whiteYellowBrown:
		return brainSand;
	case solidYellowbrown:
		return mustardAddicted;
	case lightBrown:
		return magicPowder;
	case lightPink:
		return trueBlush;
	case vibrateRed:
		return merryCranesbill;
	case BrightOringe:
		return brightOringe;
	}
}
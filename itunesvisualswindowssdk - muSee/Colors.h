#pragma once

struct Color3 {
	//all values are between 0 and 1
	float r;
	float g;
	float b;
};

enum blendColorMode {NoColor, brightGreenBlue, darkGreenblue, lightGreenblue,
						whiteBlue, warmGreen, whiteYellowBrown, solidYellowbrown,
						lightBrown, lightPink, vibrateRed,BrightOringe};

const Color3 noColor = {1.0,1.0,1.0}; //no blending color

//all the color will be + 10 offseted
const Color3 bewitchedTree = {45.0/255, 210.0/255, 190.0/255}; //bright green-blue
const Color3 mysticalGreen = {150.0/255, 205.0/255, 195.0/255}; //dark green-blue
const Color3 lightHeartBlue = {178.0/255, 249.0/255, 242.0/255}; //light green-blue
const Color3 glassGall = {202.0/255, 245.0/255, 250.0/255}; //white blue
const Color3 sillyFizz = {194.0/255, 246.0/255, 216.0/255}; //warm green
const Color3 brainSand = {225.0/255, 222.0/255, 190.0/255}; //white yellow-brown
const Color3 mustardAddicted = {221.0/255, 196.0/255, 133.0/255}; //solid yellow-brown
const Color3 magicPowder = {248.0/255, 217.0/255, 189.0/255}; //light brown
const Color3 trueBlush = {253.0/255, 185.0/255, 173.0/255}; //light pink
const Color3 merryCranesbill = {262.0/255, 111.0/255, 124.0/255}; //vibrateRed
const Color3 brightOringe = {263.0/255, 224.0/255, 93.0/255}; //brightOringe

Color3 selectColor(blendColorMode theColor);
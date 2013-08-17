#pragma once
//----------------------------------------------------------------------------
// Melody Tone Acceptance Scoring
//----------------------------------------------------------------------------
const float startingOffset = 20;
const float isHighestOffset = 20;
const float negativeHighestNotPickedScale = -8;
const float negativeClosenessScale = -2;
const int	 regressionOrder = 3;
const float negativeTrendScale = -4; //more important than closeness
const float reconsiderTopMelodyToneAssignmentThreshold = -40;

#define NotATone	-1000

//----------------------------------------------------------------------------
// Melody Bounds for Melody Touching Detection
//----------------------------------------------------------------------------
const int toneBound = 1; //1 tone
const int timeBound = 5; //5 MS

//----------------------------------------------------------------------------
// Effective Melody Analysis Constants
//----------------------------------------------------------------------------
const unsigned int minimumBuffer = 6;
const float upwardEffect = 2;
const float downwardEffect = 1.5;
const float scoreEffect = 1/startingOffset;
const float negativeScoreEffect = scoreEffect;

const unsigned int maxSmoothingOrder = minimumBuffer;
const unsigned int minSmoothingOrder = 2;
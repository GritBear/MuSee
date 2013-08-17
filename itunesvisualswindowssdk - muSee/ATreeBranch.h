#pragma once
#include "AnimationComponents.h"
#include "AInterface.h"
#include "ALineSeg.h"

const int numSegPerUnitLength = 50; //per 1 length
const float segLength = 1.0/numSegPerUnitLength;
const int maxFluctuation = 180;
//Responsible of:
// 1: spawn all segments with approperiate width
// 2: tracking root position and destruction condition
// 3: update motion and angle
// 4: trigger event and create children

//Constraint for non-straight branch:
// 1: the target vertex must be within the span of the branch when straight


class ATreeBranch : public AObject{
private:
	bool straightline;
	bool enableAngleAcc;
protected: 
	float effectTime;//vibration effect
	AState effectState; //vibration state

	float length;
	float segL;

	float angleAcc;
	float angleSpeed;
	int angleUpdateStage;

	float dampingFactor; //less/equal to 1, greater than 0
	float previousDiff;
	int compareRootY;
	int fluctuationCnt;

	AStruct lineCopy;
	deque<ALineSeg*> lineSegL;

	//list<ALineSeg*>::iterator lineIter;

	//computation for each segment: 
		//angle = rootAngle + deltaAngle * angleConst * ((i + indexOffset)/vertex)^(AngOrder)
		int AngOrder;
		float angleConst;
		float * rootAngle; //unchanging value
		float indexOffset;
		float deltaAngle;
		float angle; //temp angle

		//width = rootWidth + WidthC * (i/vertex)^(WOrder)
		int WOrder;
		//float rootWidth; //unchanging value (maximum) //defined in AObject
		float minwidth;
		float widthDiff;

		//float width;  //this is the temp store //defined in AObject
		
	//constraint values
	float maxAngleSpeed;

	//the width is determined at spawning time
	//make a copy for each
	Vector3 * branchRootStart;		//positionQ[0];

	Vector3 * treeSpeed;			//speedPosition[0];

	Vector3 * branchTop;		//positionQ[0];
	Vector3 rootDiff;

public:
	
	deque<ATreeBranch*> branchChild;

	//------------------------------------------
	//core functions
	//------------------------------------------
	ATreeBranch(){}; //default
	ATreeBranch(AStruct * aStruct);
	~ATreeBranch(){FreeAll(lineSegL); AObject::Destroy();}

	void Init(AStruct * aStruct);

	void Destroy(){AObject::Destroy();}
	void SpawnSegments();

	//update: angle speed updating
	void Update();
	void Render();// needs only to render child or effect

	void CreateBranch(AStruct * aStruct, float PercentBranch, CoorPosition theAnchored, float depthChange = 0.01);
	void CreateEndBranch(AStruct * aStruct, float PercentBranch, CoorPosition theAnchored, float depthChange = 0.01);
	void InsertAnimation(AObject *TheNextAni, float PercentBranch, CoorPosition theAnchored, float depthChange = 0.01);

	void InsertAnimationInChildBranch(AObject *TheNextAni, float PercentBranch, CoorPosition theAnchored, int level, float depthChange = 0.01);


	//update methods
	void UpdateAngleDrive(); //produce new angle speed and deltaAngle
	void UpdateAngleSpeed();
	void CheckAngleConstraints();
};
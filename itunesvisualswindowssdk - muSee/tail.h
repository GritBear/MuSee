#pragma once
#include "AnimationComponents.h"
//#include "ATreeBranch.h"
#include "ALineSeg.h"

//all it need is the pointer to a tail queue
//elimination constraints (e.g. time or length)

class ATail : public AObject{
private:
	bool lengthLimit;
protected:
	float length;

	//constraint parameter
	deque<Vector3> * tailQ;

	AStruct lineCopy;
	list<ALineSeg*> lineSegL;
	list<ALineSeg*>::iterator lineIter;

	//width = rootWidth + widthDiff * ((vertex - i)/vertex)^(WOrder)
	int WOrder;
	float rootWidth; //unchanging value (maximum)
	float minwidth;
	float widthDiff;

	float width;  //this is the temp store

	Vector3 * branchRootStart;		//positionQ[0];

public:
	int queueLengthN; //if using time storage



	ATail(AStruct * aStruct);
	~ATail(){FreeAll(lineSegL); AObject::Destroy();}

	void Init(AStruct * aStruct);

	void Destroy(){AObject::Destroy();}
	void SpawnSegments();

	//update: angle speed updating
	void Update();
	void Render();// needs only to render child or effect

	//update methods

};
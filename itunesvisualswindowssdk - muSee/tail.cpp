#include "tail.h"

//not finished yet

ATail::ATail(AStruct * aStruct){
	AObject::Init(aStruct);
	AObject::InitSubComponent( aStruct);
	ATail::Init(aStruct);
}

void ATail::Init(AStruct * aStruct){
	//copy basic element
	lineCopy = (*aStruct);
	lineSegL.clear();
	
	
	WOrder = aStruct->widthOrder;
	minwidth = aStruct->minWidth;

	//copy special values
	length = aStruct->length;

	rootWidth = aStruct->width;
	widthDiff = rootWidth - minwidth;

	tailQ = aStruct->tailQ;

	theChildren->SetCleanLim(-2.5, 4, 2.5, -2.5);
	theChildren->SetActivateMode(AllActive);

	branchRootStart = &(*tailQ)[0];

	SpawnSegments();
}

void ATail::SpawnSegments(){
	ALineSeg* previous = nullptr;

	//some piecewise processing
	lineCopy.vertex = 4;
//	lineCopy.length = segL;
	lineCopy.theShape = rect_ver;
	lineCopy.enterTime = lineCopy.enterTime/vertex;
	lineCopy.exitTime = lineCopy.exitTime/vertex;
	lineCopy.aPositionQ = deque<Vector3>(4); 
	lineCopy.aSpeedQ = deque<Vector3>(4); 

	//spawn all line segments
	for(int i = 0; i < vertex; i++){
		//compute the width
		lineCopy.width = minwidth + widthDiff * pow((float(vertex-i))/vertex,WOrder);

		//spawn line segments
		ALineSeg * thisSeg = new ALineSeg(&lineCopy, i, previous, branchRootStart);

		//push into list
		lineSegL.push_back(thisSeg);
		theChildren->Insert((AInterface*)thisSeg);
		previous = thisSeg;
	}
	//get the spawned top vertex
}

void ATail::Update(){
	//update the target
	float timeElapsed = theSpeedControl->GetTimeElapsed();

	AObject::Update();
}

void ATail::Render(){
	if(toRender){
		//theRender->Render(); save for later implementing effects
		theChildren->Render();
	}
}


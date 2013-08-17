// essential animation object components
// 0. AStruct:	Stores all essential shared variables (revision of animation struct)
// 1. AMotion:	AnimationMotion: (controls update)
// 2. AObject:	Animation Structure (controls children) the Main Recursive Body
// 3. ARender:	Animation entrance/exiting/dying effect (controls reader)
// 4. AEvent:	Defines different event and the control class

//further modularization is supported
#pragma once
#include "AniStructs.h"
#include "AInterface.h"
#include "AniDispatcher.h"
#include "SubComponents.h"

//the main body of animation object
//the base of other animations object
class AObject : public AInterface{
private:
	//if both false, then plot the entire queue
	bool timeLimit;
	float timeLim;
protected:

	bool tailEnabled;
	clock_t spawningT;

	bool toUpdate;
	bool toRender;
	unsigned int id;

	AMotion *theMotion;
	AEvent *theEvent;
	ARender *theRender;

	DataReader * theReader;
	CreationCore *theCreationCore;
	SpeedControl *theSpeedControl;
	control * theControl;
	
	//copied variable pointers and values
	int vertex;
	
	deque<Vector3> * position;
	deque<Vector3> * speed;

	//for anchor processing
	Vector3 * targetP;				//positionQ[1];
	Vector3 * targetspeed;			//speedPosition[1];
public:

	//Debug:
	//children management
	AniDispatcher * theChildren;
	//positional variables
	float angle;
	float width;
	float rootWidth;


	AObject();//default constructor

	AObject(AStruct * aStruct);
	~AObject(){Destroy();}

	void Init(AStruct * aStruct);
	void virtual InitSubComponent(AStruct * aStruct);

	virtual void Destroy();

	virtual void Update();
	void Event(EventType theType);
	virtual void Render();
	virtual void Clean();

	virtual void AddAnotherChild(AInterface * aChild){theChildren->Insert(aChild);}
	void AddAnchoredChild(AObject * aChild, CoorPosition CenterAnchor, float deltaDepth = 0.01);

	//2D check: true means at least one vertex is NOT OUTSIDE clean limit
	virtual bool checkInCleanLim(float left,float right,float top,float bottom);

	//Getter Setter
	virtual void SetAnchor(CoorPosition centerAnchor,Vector3 * parentAchorPosition,float * parentAnchorAngle);
	
	deque<Vector3> * GetPosition(){return position;}

	bool GetToUpdate(){return toUpdate;}
	void SetToUpdate(bool ToUpdate){toUpdate = ToUpdate;}

	bool GetToRender(){return toRender;}
	void SetToRender(bool ToRender){toRender = ToRender;}

	bool GetActive(){return toUpdate;}
	void SetActive(bool Active){toUpdate = toRender = Active;}

	float GetAvgX();
	float GetAvgY();
	virtual Vector3 GetFinalVertex(){return theEvent->GetFinalVertex();}

	void SetID(unsigned int ID){id = ID;}
	unsigned int GetID(){return id;}

	void SetDepth(float depth){theMotion->SetDepth(depth);}
	AState GetState(){return theEvent->GetState();}

	AMotion * GetMotion(){return theMotion;}
	AEvent *  GetEvent(){return theEvent;}
	ARender * GetRender(){return theRender;}
};


#pragma once
#include "AnimationComponents.h"
#include "tail.h"

//it takes control of the velocity
//enum FlyingMode {fTrace, fRandom, fLinear};
class AFlyMotion : public AMotion{
private:
	bool isAnchor; //if true, follow trace, if not, follow anchor
	bool stopped; //only for anchor
protected:
	AMotion * flyingMotion; //hijacked from the child
	deque<Vector3> coorFromCenter; //anchor with arbitrary angle 0
	float angleAbsSumFactor;
public:
	FlyingMode fMode;
	deque<Vector3> trace; //we only need to know the next value
	Vector3 backgroundVel;

	float speed;
	float distanceTillNext; //time till next point in the trace
	
	//the center coordinate and its speed
	Vector3 anchor;
	Vector3 anchorVel;

	//anchor variables (if it is a flying object instead of an anchor object)
	AFlyMotion * anchorMotion;

	//Vector3 *anchorPtr;  already defined in the AMotion
	//Vector3 *anchorVelPtr;

	//FlyingAnimationTexture Parameters---------------------------------------------
	float accFactor;
	float accPower;

	//Anchor Stopping/Slow down State---------------------------------------------
	//move along the trace
	float stopMaxSpeed; //in sumAbs
	float drag; // coefficient for 2 order acceleration

	//---------------------------------------------------------
	AFlyMotion(AStruct * aStruct, AMotion *FlyingMotion = nullptr);
	~AFlyMotion(){AMotion::Destroy();}

	void Init(AStruct * aStruct, AMotion *FlyingMotion);
	void InitCoorFromCenter(); //the initial position Q with an angle

	void Update();
	void AnchorBasedPositionUpdate();//generate new position queue

	//anchor processing
	void InitAnchor();
	void ComputeAnchorVel();
	void SetIsAnchor(bool IsAnchor, AFlyMotion * AnchorMotion = nullptr);

	//non anchor processing
	void AnchorPullDrive();
	void CheckLim();
	void Slowing();
};

//=========================================================

//=========================================================
class AFly : public AObject{
private:
	bool displayAnchor;
	bool enableAnchor;
protected:
	ATail * itsTail;
	AStruct tailCopy;

	//float angle; defined in AObject //controls the rotation

	AObject * animationToFly;
	AFly * anchorObj;
	AFlyMotion * aFlyMotion;
public:
	AFly(AStruct * aStruct, AObject * AnimationToFly = nullptr, AFly * AnchorObj = nullptr);
	~AFly(){AObject::Destroy();}

	void Init(AStruct * aStruct, AObject * AnimationToFly, AFly * AnchorObj);
	void InitSubComponent(AStruct * aStruct);

	//Getters Setters
	//display anchor for debug purpose
	void SetIsAnchor(bool anchor);
	void SetDisplayAnchor(bool displayAnc){displayAnchor = displayAnc; anchorObj->SetToRender(displayAnc);}
	void PushInTrace(Vector3 nextTarget){aFlyMotion->trace.push_back(nextTarget);}

	AFlyMotion * GetMotion(){return aFlyMotion;}
};
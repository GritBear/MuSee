#pragma once
#include "Global.h"


class AInterface{
protected:
public:
	//core functions
	virtual void Destroy(){};
	virtual void Update(){};
	virtual void Render(){};

	virtual bool checkInCleanLim(float left,float right,float top,float bottom){return false;}
	
	//Getter Setter
	virtual bool GetActive(){return false;}
	virtual void SetActive(bool Active){};
	virtual void SetID(unsigned int ID){};
	virtual unsigned int GetID(){return 0;}
	virtual void Clean(){};
	virtual void AddAnotherChild(AInterface * aChild){};

	float virtual GetAvgX(){return 0;}
	float virtual GetAvgY(){return 0;}
	AState virtual GetState(){return NoState;}

	//custormerizables: changable depending on the animation implementation
	Vector3 virtual GetFinalVertex(){Vector3 aVect = {0,0,0}; return aVect;}

	Vector3 virtual GetPointMethod(){Vector3 aVect = {0,0,0}; return aVect;}
	float virtual GetValueMethod(){return 0;}

	//some special methods
	void virtual SetAngle(float Value){};
	void virtual SetWidth(float Value){};
	void virtual SetDepth(float depth){};
};

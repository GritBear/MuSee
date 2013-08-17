#pragma once
#include "AniStructs.h"
#include "AInterface.h"

enum ActivateMode {Sequential, AllActive};

//This AniDispatcher controls the children animations
//template<class T>
class AniDispatcher{
private:

protected:
	ActivateMode actMode;

	control * theControl;
	list<AInterface *> aniL;
	list<AInterface *>::iterator aniObjListIter, aniObjListIterManual;
	list<AInterface *>::reverse_iterator RaniObjListIter, RactivateIter;

	//track the clean method
	deque<int> newlyRemovedPositionQ;

	//set clean conditions
	float topLim, bottomLim, leftLim, rightLim;

public:
	AniDispatcher();
	~AniDispatcher(){Destroy();}

	void Destroy();
	void Init();

	void Activate();
	void Update();
	void Render();

	
	void Insert(AInterface * newAni);//should be append
	void Append(AInterface * newAni){Insert(newAni);}

	//manual iterator methods (design for loop update)
	void InitIterator(){aniObjListIterManual = aniL.begin();}
	//true means successful, false means unsuccessful
	bool IteratorForward(){if(aniObjListIterManual!=aniL.end()){aniObjListIterManual++;return true; }else{return false;}}
	bool IteratorCheck(){return !(aniObjListIterManual!=aniL.end());} //true means not the last, false means the last one
	AInterface * GetIterator(){return (*aniObjListIterManual);}

	//RemoveTooOldEntries
	void Clean();
	deque<int> GetNewlyRemoved(){return newlyRemovedPositionQ;}

	//Getter Setters
	void SetCleanLim(float left, float right= dontCare, float top= dontCare, float bottom= dontCare){
		topLim = top;
		bottomLim = bottom;
		leftLim = left;
		rightLim = right;
	}

	//AInterface * AniFind(int positionInList);
	list<AInterface *> GetAniList(){return aniL;}
	void SetAniList(list<AInterface *> aList){aniL = aList;}
	int GetANum(){return aniL.size();}
	void SetActivateMode(ActivateMode theMode){actMode = theMode;}
	ActivateMode GetActivateMode(){return actMode;}
};


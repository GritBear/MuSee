#include "AniDispatcher.h"


AniDispatcher::AniDispatcher(){
	Init();
}


void AniDispatcher::Destroy(){
	if(aniL.size() > 0){
		for(aniObjListIter = aniL.begin(); aniObjListIter != aniL.end(); ++aniObjListIter){
			(*aniObjListIter)->Destroy();
			delete((*aniObjListIter));
		}
	}
	FreeAll(aniL);
}


void AniDispatcher::Init(){
	actMode = AllActive;

	topLim = dontCare;
	bottomLim = dontCare;
	leftLim = dontCare;
	rightLim = dontCare;

}


void AniDispatcher::Activate(){
	if(aniL.size()>0){
		aniL.front()->SetActive(true);

		switch(actMode){
		case Sequential:
			//move to the first active animation
			while(RactivateIter != aniL.rend() && !(*RactivateIter)->GetActive() /*&& ((*RactivateIter)->GetState() != Enter)*/){
				RactivateIter++;
			}
			//activate the next animation (if not the last one)
			if(RactivateIter != aniL.rbegin() && !(*(--RactivateIter))->GetActive()){
				(*RactivateIter)->SetActive(true);
			}
			break;
		}

	}
}

void AniDispatcher::Update(){
	if(actMode != AllActive)
		Activate();

	if(aniL.size() > 0){
		for(aniObjListIter = aniL.begin(); aniObjListIter != aniL.end(); ++aniObjListIter){
			(*aniObjListIter)->Update();
		}
	}
}

void AniDispatcher::Render(){
	if(aniL.size() > 0){
		for(aniObjListIter = aniL.begin(); aniObjListIter != aniL.end(); ++aniObjListIter){
			(*aniObjListIter)->Render();
		}
	}
}

void AniDispatcher::Insert(AInterface * newAni){
	if(actMode == AllActive)
		newAni->SetActive(true);
	
	aniL.push_back(newAni);
	RactivateIter = aniL.rend();
}

void AniDispatcher::Clean(){
	deque<int> removedPositionQ;
	int counter = 0;

	if(aniL.size() > 0){
		for(aniObjListIter = aniL.begin(); aniObjListIter != aniL.end(); ++aniObjListIter){
			(*aniObjListIter)->Clean(); //maintain each chilren


			if(!(*aniObjListIter)->checkInCleanLim(leftLim, rightLim, topLim, bottomLim)){
				delete(*aniObjListIter);
				aniObjListIter = aniL.erase(aniObjListIter);
				removedPositionQ.push_back(counter - removedPositionQ.size());
				RactivateIter = (aniL.size()>0) ? aniL.rbegin() : RactivateIter; // re-initialize the R-iterator
			}
			counter++;
		}
	}

	//pass the new deque pointer
	newlyRemovedPositionQ.swap(removedPositionQ);
	FreeAll(removedPositionQ);
}



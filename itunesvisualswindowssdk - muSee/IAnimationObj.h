#pragma once;
// This is an interface experiment

class IAnimationObj{
public:
	virtual ~IAnimationObj(){};
	virtual void  Update() = 0;
	virtual void  Render(double screen_w = 0, double screen_h = 0, bool updating = true) = 0;
	virtual void  Destroy() = 0;
	virtual bool  GetAlive() = 0;
	virtual bool  GetToRender() = 0;
};
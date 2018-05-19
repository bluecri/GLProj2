#pragma once

//class Canvas;
#include"Canvas.h"

class Scene
{
public:
	void render()
	{
		
	}
	void changeCanvas(Canvas* newTargetCanvas)
	{
		_targetCanvas->setBRender(false);
		newTargetCanvas->setBRender(true);
		_targetCanvas = newTargetCanvas;
	}


// pre elems
private:
	Canvas * _targetCanvas;

};
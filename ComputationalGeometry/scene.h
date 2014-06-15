#pragma once

#include "input.h"

class Renderer;

class Scene {
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void onDraw(Renderer* renderer) = 0;
	virtual void onDrawText(Renderer* renderer) = 0;
	virtual void onInput(Renderer * renderer, const MouseKeyboardInput& input) = 0;
};

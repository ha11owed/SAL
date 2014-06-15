#pragma once

#include "renderer.h"
#include <vector>

class DemoScene : public Scene {
public:
	DemoScene() : demoIdx(0) { lastMsg[0] = '\0'; }
	virtual ~DemoScene() {}

	void onDraw(Renderer * renderer);
	void onDrawText(Renderer * renderer);

	void onInput(Renderer * renderer, const MouseKeyboardInput& input);

	int demoIdx;
	char lastMsg[512];

	std::vector<Point3D> userClicks;
};

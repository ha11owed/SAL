#include "preheader.h"
#include "demos.h"

void demoDirection(DemoScene* scene, Renderer* renderer)
{
	Color c(0.5f, 0.2f, 0.7f);

	std::vector<Point3D>& userClicks = scene->userClicks;
	std::size_t n = userClicks.size();

	if (n > 0)
	{
		renderer->drawPoint(userClicks[0], c);
		for (size_t i = 0; i < n - 1; i++)
		{
			renderer->drawLine(userClicks[i], userClicks[i + 1], c);
		}
		renderer->drawPoint(userClicks[n-1], c);

		if (n == 3)
		{
			Vector3D v1(userClicks[1], userClicks[0]);
			Vector3D v2(userClicks[1], userClicks[2]);
			Vector3D cross(v1.cross(v2));

			if (cross.z() > 0)
				std::sprintf(scene->lastMsg, "Direction clockwise");
			else if (cross.z() < 0)
				std::sprintf(scene->lastMsg, "Direction counterclockwise");
			else
				std::sprintf(scene->lastMsg, "Direction none");
		}
		else
		{
			std::sprintf(scene->lastMsg, "Click 3 times");
		}

		if (n > 3) { userClicks.clear(); }
	}
}

struct Demo
{
	char *title;
	void(*onDraw)(DemoScene*, Renderer*);
};

Demo demos[] = {
	{ "Direction", demoDirection }
};

inline int demoCount() {
	// I hate this macro usage...
	return sizeof(demos) / sizeof(Demo);
}


void DemoScene::onDraw(Renderer * renderer)
{
	Demo& demo = demos[demoIdx];

	demo.onDraw(this, renderer);
}

void DemoScene::onDrawText(Renderer * renderer)
{
	int row = 1;
	Demo& demo = demos[demoIdx];
	renderer->shapesPrintf(row++, 1, "SAL Algorithms Demos");
	renderer->shapesPrintf(row++, 1, "Demo: %s (%d/%d)", demo.title, demoIdx + 1, demoCount());
	renderer->shapesPrintf(row++, 1, lastMsg);
}

void DemoScene::onInput(Renderer * renderer, const MouseKeyboardInput& input)
{
	double scale;
	Vector3D mousePos;
	Point3D p;
	switch (input.keyType)
	{
	case MouseKeyboardInput::ArrowUp:
		demoIdx++;
		userClicks.clear();
		break;
	case MouseKeyboardInput::ArrowDown:
		demoIdx--;
		userClicks.clear();
		break;
	case MouseKeyboardInput::MouseLeftReleased:
		mousePos = renderer->mouseToCoord(input.x, input.y);
		scale = std::abs(mousePos.z());
		p = mousePos.scale(1.0/scale).toPoint();
		userClicks.push_back(p);
		break;
	default:
		break;
	}

	if (demoIdx < 0) {
		demoIdx = 0;
	}
	else {
		int demoCnt = demoCount();
		if (demoIdx >= demoCnt) {
			demoIdx = demoCnt;
		}
	}
}

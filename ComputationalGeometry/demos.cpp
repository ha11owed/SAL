#include "preheader.h"
#include "demos.h"
#include "algorithms.h"

static Color c(0.5f, 0.2f, 0.7f);

static MouseKeyboardInput lastUserInput;

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
		renderer->drawPoint(userClicks[n - 1], c);
	}

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

void demoIntersect(DemoScene* scene, Renderer* renderer)
{
	std::vector<Point3D>& userClicks = scene->userClicks;
	std::size_t n = userClicks.size();

	if (n > 0)
	{
		for (size_t i = 0; i < n - 1; i++)
		{
			renderer->drawPoint(userClicks[i], c);
			if (i % 2 == 0)
				renderer->drawLine(userClicks[i], userClicks[i + 1], c);
		}
		renderer->drawPoint(userClicks[n - 1], c);
	}

	if (n == 4)
	{
		Point2D p1(userClicks[0].x(), userClicks[0].y());
		Point2D p2(userClicks[1].x(), userClicks[1].y());
		Point2D p3(userClicks[2].x(), userClicks[2].y());
		Point2D p4(userClicks[3].x(), userClicks[3].y());
		Segment2D s1(p1, p2);
		Segment2D s2(p3, p4);

		if (intersect(s1, s2))
			std::sprintf(scene->lastMsg, "Segments intersect");
		else
			std::sprintf(scene->lastMsg, "Segments don't intersect");
	}
	else
	{
		std::sprintf(scene->lastMsg, "Click 4 times");
	}

	if (n > 4) { userClicks.clear(); }
}

void demoAnyIntersect(DemoScene* scene, Renderer* renderer)
{
	static bool wasRun = false;
	std::vector<Point3D>& userClicks = scene->userClicks;
	std::size_t n = userClicks.size();

	std::vector<Segment2D> segments;
	segments.reserve(n / 2);

	if (n > 0)
	{
		for (size_t i = 0; i < n - 1; i += 2)
		{
			Point2D p1(userClicks[i].x(), userClicks[i].y());
			Point2D p2(userClicks[i + 1].x(), userClicks[i + 1].y());
			segments.push_back(Segment2D(p1, p2));

			renderer->drawPoint(userClicks[i], c);
			renderer->drawLine(userClicks[i], userClicks[i + 1], c);
			renderer->drawPoint(userClicks[i + 1], c);
		}
		renderer->drawPoint(userClicks[n - 1], c);
	}

	if (n >= 4 && n % 2 == 0 && lastUserInput.keyType == MouseKeyboardInput::MouseRightReleased)
	{
		if (anySegmentsIntersect(segments))
			std::sprintf(scene->lastMsg, "Segments intersect");
		else
			std::sprintf(scene->lastMsg, "Segments don't intersect");

		wasRun = true;
	}
	else
	{
		std::sprintf(scene->lastMsg, "Click 2n times. Right click to run algorithm.");
	}

	if (wasRun && lastUserInput.keyType != MouseKeyboardInput::MouseRightReleased) {
		wasRun = false;
		userClicks.clear();
	}
}

void demoConvexHull(DemoScene* scene, Renderer* renderer)
{
	static bool wasRun = false;
	std::vector<Point3D>& userClicks = scene->userClicks;
	std::size_t n = userClicks.size();

	for (size_t i = 0; i < n; i++)
	{
		renderer->drawPoint(userClicks[i], c);
	}
	if (n >= 3)
	{
		double z = userClicks[0].z();
		std::vector<Point2D> points;
		points.reserve(n);
		for (size_t i = 0; i < n; i++) {
			points.push_back(Point2D(userClicks[i].x(), userClicks[i].y()));
		}

		std::vector<Point2D> outConvexHull;
		convexHullGrahamScan(points, outConvexHull);

		for (size_t i = 0; i < outConvexHull.size() - 1; i++)
		{
			renderer->drawLine(Point3D(outConvexHull[i], z), Point3D(outConvexHull[i + 1], z), c);
		}
		renderer->drawLine(Point3D(outConvexHull[0], z), Point3D(outConvexHull[outConvexHull.size() - 1], z), c);
		wasRun = true;
	}

	std::sprintf(scene->lastMsg, "Right click to reset.");

	if (wasRun && lastUserInput.keyType == MouseKeyboardInput::MouseRightReleased) {
		wasRun = false;
		userClicks.clear();
	}
}

struct Demo
{
	char *title;
	void(*onDraw)(DemoScene*, Renderer*);
	void(*onInput)(Renderer *, const MouseKeyboardInput&);
};

Demo demos[] = {
	{ "Convex hull", demoConvexHull, nullptr },
	{ "Any intersection", demoAnyIntersect, nullptr },
	{ "Intersection", demoIntersect, nullptr },
	{ "Direction", demoDirection, nullptr }
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
	renderer->shapesPrintf(row++, 1, "SAL Algorithms Demos (up/down arrow to navigate)");
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
		demoIdx--;
		userClicks.clear();
		lastMsg[0] = '\0';
		break;
	case MouseKeyboardInput::ArrowDown:
		demoIdx++;
		userClicks.clear();
		lastMsg[0] = '\0';
		break;
	case MouseKeyboardInput::MouseLeftReleased:
		mousePos = renderer->mouseToCoord(input.x, input.y);
		scale = std::abs(mousePos.z());
		p = mousePos.scale(1.0 / scale).toPoint();
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

	lastUserInput = input;
}

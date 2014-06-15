#pragma once

#include "primitives.h"
#include "color.h"
#include "scene.h"

class Renderer {
public:
	virtual ~Renderer();

	static Renderer & instance();
	void mainLoop();

	Vector3D mouseToCoord(int x, int y);

	void shapesPrintf(int row, int col, const char *fmt, ...);

	void drawPoint(const Point3D& point, const Color& color);
	void drawLine(const Point3D& p1, const Point3D& p2, const Color& color);
	void drawVector(const Point3D& point, const Vector3D& vector, const Color& color);

	Scene* scene;

private:
	static Renderer g_instance;
	Renderer();
};

#pragma once

#include "primitives.h"

inline double direction(const Point2D& p1, const Point2D& p2, const Point2D& p3)
{
	return Vector2D(p3, p1).cross(Vector2D(p2, p1));
}

inline bool onSegment(const Point2D& start, const Point2D& end, const Point2D& p)
{
	return std::fmin(start.x(), end.x()) <= p.x() && p.x() <= std::fmax(start.x(), end.x())
		&& std::fmin(start.y(), end.y()) <= p.y() && p.y() <= std::fmax(start.y(), end.y());
}




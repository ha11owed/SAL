#pragma once

#include "primitives.h"
#include <vector>
#include <set>
#include <algorithm>
#include <galib/intrusive/list.h>

struct VoronoiEdge
{
	Point2D * from;
	Point2D * to;

	int surfaceLeft;
	int surfaceRight;

	VoronoiEdge * left;
	VoronoiEdge * right;


	galib::intrusive::Link<VoronoiEdge> m_link;
};

typedef galib::intrusive::List<VoronoiEdge, &VoronoiEdge::m_link> VoronoiEdgeList;

Segment2D voronoiBisector(const Point2D& p1, const Point2D& p2, const Rect2D& boundingRect)
{
	double dblMax = 1e+15;
	Vector2D mid((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2);

	Vector2D v(p1, p2);
	Vector2D normal(v.y(), -v.x());

	Vector2D v1 = normal.scale(dblMax).add(mid);
	Vector2D v2 = normal.scale(-dblMax).add(mid);
	Segment2D s(Point2D(v1.x(), v1.y()), Point2D(v2.x(), v2.y());
	boundingRect.inside(s, s);
	return s;
}

void voronoiMerge(const VoronoiEdgeList& listLeft, const VoronoiEdgeList& listRight, VoronoiEdgeList& edges)
{
	std::vector<Point2D> pts;
	for (VoronoiEdgeList::const_iterator it = listLeft.begin(); it != listLeft.end(); it++)
	{
		if (it->from != NULL)
		{
			pts.push_back(*(it->from));
			//it->
		}
	}


	assert(listLeft.isEmpty());
	assert(listRight.isEmpty());
}

void voronoiRec(size_t start, size_t n, const std::vector<Point2D>& points, const Rect2D& boundingRect, VoronoiEdgeList& edges)
{
	if (n == 1)
	{
		// nothing to do
	}
	else if (n == 2)
	{
		
	}
	else
	{
		size_t n2 = n / 2;
		// n2 must be at least 2
		VoronoiEdgeList listLeft;
		voronoiRec(start, n2, points, boundingRect, listLeft);
		VoronoiEdgeList listRight;
		voronoiRec(start + n2, n - n2, points, boundingRect, listRight);

		voronoiMerge(listLeft, listRight, edges);
	}
}

void voronoi(const std::vector<Point2D>& points, VoronoiEdgeList& edges)
{
	edges.deleteAll();
	std::vector<Point2D> sortedPoints(points);
	std::sort(sortedPoints.begin(), sortedPoints.end(), [](const Point2D& p1, const Point2D& p2) -> bool
	{
		return p1.x() < p1.x();
	});

	size_t n = points.size();
	if (n > 1)
	{

		double minX = points[0].x(), maxX = points[0].x();
		double minY = points[0].y(), maxY = points[0].y();
		for (size_t i = 1; i < n; i++)
		{
			minX = std::min(minX, points[i].x());
			maxX = std::max(maxX, points[i].x());
			minY = std::min(minY, points[i].y());
			maxY = std::max(maxY, points[i].y());
		}
		Rect2D boundingRect(Point2D(minX, minY), Point2D(maxX, maxY));
		voronoiRec(0, n, sortedPoints, boundingRect, edges);
	}
}


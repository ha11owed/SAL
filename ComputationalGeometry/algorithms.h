#pragma once

#include "primitives.h"
#include <vector>
#include <set>
#include <algorithm>

inline double direction(const Point2D& p1, const Point2D& p2, const Point2D& p3)
{
	return Vector2D(p3, p1).cross(Vector2D(p2, p1));
}

inline bool inRectangle(const Point2D& start, const Point2D& end, const Point2D& p)
{
	return std::fmin(start.x(), end.x()) <= p.x() && p.x() <= std::fmax(start.x(), end.x())
		&& std::fmin(start.y(), end.y()) <= p.y() && p.y() <= std::fmax(start.y(), end.y());
}

inline bool intersect(const Segment2D& segment1, const Segment2D& segment2)
{
	double d1a = direction(segment1.from(), segment1.to(), segment2.to());
	double d1b = direction(segment1.from(), segment1.to(), segment2.from());
	double d2a = direction(segment2.from(), segment2.to(), segment1.to());
	double d2b = direction(segment2.from(), segment2.to(), segment1.from());
	if (((d1a < 0 && d1b > 0) || (d1a > 0 && d1b < 0))
		&& ((d2a < 0 && d2b > 0) || (d2a > 0 && d2b < 0))) {
		return true;
	}

	if (d1a == 0.0 && inRectangle(segment1.from(), segment1.to(), segment2.to())) {
		return true;
	}
	if (d1b == 0.0 && inRectangle(segment1.from(), segment1.to(), segment2.from())) {
		return true;
	}
	if (d2a == 0.0 && inRectangle(segment2.from(), segment2.to(), segment1.to())) {
		return true;
	}
	if (d2b == 0.0 && inRectangle(segment2.from(), segment2.to(), segment1.from())) {
		return true;
	}

	return false;
}

inline bool anySegmentsIntersect(std::vector<Segment2D>& segments)
{
	struct PointPosition{
	public:
		Point2D point;
		Segment2D* originalSegment;
	};

	struct RbCompare
	{
		bool operator()(Segment2D* s1, Segment2D* s2) const {
			return s1->from().y() < s2->from().y();
		}
	};
	auto sortXCompare = [](const PointPosition& pp1, const PointPosition& pp2) -> bool {
		return pp1.point.x() < pp2.point.x();
	};


	int n = segments.size();
	std::vector<PointPosition> sortedPoints;
	sortedPoints.reserve(n * 2);
	for (int i = 0; i < n; i++) {
		if (segments[i].from().x() > segments[i].to().x()) {
			segments[i] = Segment2D(segments[i].to(), segments[i].from());
		}

		PointPosition pp;
		// don't mess with the segments vector :D
		pp.originalSegment = (Segment2D*)&(segments[i]);

		pp.point = segments[i].from();
		sortedPoints.push_back(pp);
		pp.point = segments[i].to();
		sortedPoints.push_back(pp);
	}

	// sort the segment according to the 'to' axis
	std::sort(sortedPoints.begin(), sortedPoints.end(), sortXCompare);

	// A C++ set is implemented as a Red-Black Tree
	std::set<Segment2D*, RbCompare> tree;
	for (size_t i = 0; i < sortedPoints.size(); i++) {
		PointPosition& pp = sortedPoints[i];
		bool isStart = pp.point == pp.originalSegment->from();

		std::set<Segment2D*, RbCompare>::iterator it, above, below;
		if (isStart) {
			it = tree.insert(pp.originalSegment).first;
			above = std::next(it);
			below = std::prev(it);
			if ((above != it && above != tree.end() && intersect((const Segment2D&)*above, (const Segment2D&)*pp.originalSegment))
				|| (below != it && below != tree.end() && intersect((const Segment2D&)*below, (const Segment2D&)*pp.originalSegment))) {
				return true;
			}
		}
		else {
			it = tree.find(pp.originalSegment);
			above = std::next(it);
			below = std::prev(it);
			if (above != it && below != it && above != tree.end() && below != tree.end() && intersect((const Segment2D&)*above, (const Segment2D&)*below)) {
				return true;
			}
			tree.erase(it);
		}
	}

	return false;
}

inline void convexHullGrahamScan(const std::vector<Point2D>& points, std::vector<Point2D>& outConvexHull)
{
	size_t idxMin = 0;
	size_t n = points.size();
	for (size_t i = 1; i < n; i++)
	{
		if (points[idxMin].y() > points[i].y())
		{
			idxMin = i;
		}
	}

	std::vector<Point2D> sortedPoints;
	sortedPoints.reserve(n - 1);
	for (size_t i = 0; i < n; i++)
	{
		if (i != idxMin)
		{
			sortedPoints.push_back(points[i]);
		}
	}

	Point2D p0 = points[idxMin];

	std::sort(sortedPoints.begin(), sortedPoints.end(), [p0](const Point2D& p1, const Point2D& p2) -> bool
	{
		Vector2D v1(p1, p0);
		Vector2D v2(p2, p0);
		double cc = v1.cross(v2);
		return cc < 0;
	});

	std::vector<Point2D> stack;
	stack.reserve(n);

	stack.push_back(p0);
	stack.push_back(sortedPoints[0]);
	stack.push_back(sortedPoints[1]);

	for (size_t i = 2; i < sortedPoints.size(); i++)
	{
		Point2D top = stack[stack.size() - 1];
		Point2D nextToTop = stack[stack.size() - 2];
		double cc = Vector2D(nextToTop, top).cross(Vector2D(top, sortedPoints[i]));
		while (cc > 0)
		{
			stack.pop_back();
			top = stack[stack.size() - 1];
			nextToTop = stack[stack.size() - 2];
			cc = Vector2D(nextToTop, top).cross(Vector2D(top, sortedPoints[i]));
		}
		
		stack.push_back(sortedPoints[i]);
	}
	outConvexHull = stack;
}

inline double minDistanceNaive(size_t start, size_t n, const std::vector<Point2D>& points, std::pair<Point2D, Point2D>& outMinDistance)
{
	double d = 1E+37;
	for (size_t i = start; i < n; i++)
	{
		for (size_t j = i + 1; j < n; j++)
		{
			double dij = points[i].dinstanceTo(points[j]);
			if (d > dij)
			{
				d = dij;
				outMinDistance = std::pair<Point2D, Point2D>(points[i], points[j]);
			}
		}
	}
	return d;
}

inline double minDistance(size_t start, size_t n, const std::vector<Point2D>& points, const std::vector<Point2D>& pointsY, std::pair<Point2D, Point2D>& outClosest)
{
	double dMin;
	if (n <= 3)
	{
		dMin = minDistanceNaive(start, n, points, outClosest);
	}
	else
	{
		size_t n2 = n / 2;
		// n2 must be at least 2

		std::pair<Point2D, Point2D> lMinPair;
		double lMin = minDistance(start, n2, points, pointsY, lMinPair);
		std::pair<Point2D, Point2D> rMinPair;
		double rMin = minDistance(start + n2, n - n2, points, pointsY, rMinPair);
		
		if (rMin < lMin)
		{
			outClosest = rMinPair;
			dMin = rMin;
		}
		else
		{
			outClosest = lMinPair;
			dMin = lMin;
		}

		double middleX = points[start + n2].x();
		std::vector<int> inRange;
		for (size_t i = 0; i < pointsY.size(); i++)
		{
			double diff = std::abs(pointsY[i].x() - middleX);
			if (diff < dMin)
			{
				inRange.push_back(i);
			}
		}

		// Don't use size_t because of the -8. it can overflow
		for (int i = 0; i < (int)inRange.size() - 1; i++)
		{
			for (int j = i + 1; j < i + 8 && j < (int)inRange.size(); j++)
			{
				double d = pointsY[inRange[i]].dinstanceTo(pointsY[inRange[j]]);
				if (d < dMin)
				{
					dMin = d;
					outClosest = std::pair<Point2D, Point2D>(pointsY[inRange[i]], pointsY[inRange[j]]);
				}
			}
		}
	}

	return dMin;
}

inline void minDistance(const std::vector<Point2D>& points, std::vector<Point2D>& outMinDistance)
{
	std::vector<Point2D> sortedPoints(points);
	std::sort(sortedPoints.begin(), sortedPoints.end(), [](const Point2D& p1, const Point2D& p2) -> bool
	{
		return p1.x() < p1.x();
	});

	std::vector<Point2D> sortedPointsY(points);
	std::sort(sortedPointsY.begin(), sortedPointsY.end(), [](const Point2D& p1, const Point2D& p2) -> bool
	{
		return p1.y() < p2.y();
	});
	
	std::pair<Point2D, Point2D> minPair;
	minDistance(0, sortedPoints.size(), sortedPoints, sortedPointsY, minPair);
	outMinDistance.push_back(minPair.first);
	outMinDistance.push_back(minPair.second);
}

#pragma once

#include <algorithm>
#include <cmath>

inline bool isCloseToZero(double v)
{
	return std::abs(v) < 1e-15;
}

// ---- 2D ----

class Point2D {
public:
	Point2D() : _x(0.0), _y(0.0) {}
	Point2D(double x, double y) : _x(x), _y(y) {}
	Point2D(const Point2D& other) : _x(other._x), _y(other._y) {}

	double x() const { return _x; }
	double y() const { return _y; }

	double dinstanceTo(const Point2D& other) const { return std::sqrt((_x - other._x)*(_x - other._x) + (_y - other._y)*(_y - other._y)); }

	bool equals(const Point2D& other) const { return _x == other._x && _y == other._y; }
	bool operator == (const Point2D& other) const { return equals(other); }
	bool operator != (const Point2D& other) const { return !equals(other); }

private:
	double _x;
	double _y;
};

class Segment2D {
public:
	Segment2D() {}
	Segment2D(const Point2D& from, const Point2D& to) : _from(from), _to(to) {}
	Segment2D(const Segment2D& other) : _from(other._from), _to(other._to) {}

	bool equals(const Segment2D& other) const { return _from == other._from && _to == other._to; }
	bool operator == (const Segment2D& other) const { return equals(other); }
	bool operator != (const Segment2D& other) const { return !equals(other); }

	const Point2D& from() const { return _from; }
	const Point2D& to() const { return _to; }
	bool lineIntersection(const Segment2D& s, Point2D& outIntersectPoint) const;
	bool intersection(const Segment2D& s, Point2D& outIntersectPoint) const;
	bool contains(const Point2D& p) const;
	bool isCollinear(const Point2D& p) const;

private:
	Point2D _from;
	Point2D _to;
};

inline bool Segment2D::isCollinear(const Point2D& p) const
{
	double crossProduct = (to().x() - from().x()) * (p.y() - from().y()) - (p.x() - from().x()) * (to().y() - from().y());
	return isCloseToZero(crossProduct);
}

inline bool Segment2D::contains(const Point2D& p) const
{
	if (isCollinear(p))
	{
		double mn = std::min(from().x(), to().x());
		double mx = std::max(from().x(), to().x());
		return mn <= p.x() && p.x() <= mx;
	}
	return false;
}

inline bool Segment2D::lineIntersection(const Segment2D& s, Point2D& outIntersectPoint) const
{
	// same as solving the equations for the segments
	// a1*x + b1*y = c1 and a2*x + b2*y = c2
	// a1 = to.y - from.y
	// b1 = from.x - to.x
	// c1 = a1*from.x + b1*from.y
	double a1 = to().y() - from().y();
	double b1 = from().x() - to().x();
	double c1 = a1 * from().x() + b1 * from().y();

	double a2 = s.to().y() - s.from().y();
	double b2 = s.from().x() - s.to().x();
	double c2 = a1 * s.from().x() + b1 * s.from().y();

	double det = a1*b2 - a2*b1;
	if (!isCloseToZero(det))
	{
		double x = (b2*c1 - b1*c2) / det;
		double y = (a1*c2 - a2*c1) / det;
		// check if the point is contained by the two segments
		outIntersectPoint = Point2D(x, y);
		return true;
	}
	return false;
}

inline bool Segment2D::intersection(const Segment2D& s, Point2D& outIntersectPoint) const
{
	Point2D pi;
	bool ok = lineIntersection(s, pi);
	if (ok && contains(pi))
	{
		outIntersectPoint = pi;
	}
	return ok;
}

class Vector2D {
public:
	Vector2D() : _x(0.0), _y(0.0) {}
	Vector2D(double x, double y) : _x(x), _y(y) {}
	Vector2D(const Vector2D& other) : _x(other._x), _y(other._y) {}
	Vector2D(const Point2D& to) : _x(to.x()), _y(to.y()) {}
	Vector2D(const Point2D& from, const Point2D& to) : _x(to.x() - from.x()), _y(to.y() - from.y()) {}

	double x() const { return _x; }
	double y() const { return _y; }

	double lengthSquared() const { return _x * _x + _y * _y; }
	double length() const { return std::sqrt(_x *_x + _y * _y); }

	Vector2D add(const Vector2D& other) const { return Vector2D(_x + other._x, _y + other._y); }
	Vector2D substract(const Vector2D& other) const { return Vector2D(_x - other._x, _y - other._y); }
	Vector2D scale(double ratio) const { return Vector2D(_x * ratio, _y * ratio); }
	Vector2D normalize(double ratio) const { double l = length(); return Vector2D(_x / l, _y / l); }

	Vector2D operator + (const Vector2D& other) const { return add(other); }
	Vector2D operator - (const Vector2D& other) const { return substract(other); }
	Vector2D operator * (double ratio) const { return scale(ratio); }
	Vector2D operator / (double ratio) const { return scale(1.0 / ratio); }

	double cross(const Vector2D& other) const { return _x * other._y - other._x * _y; }
	bool isCollinear(const Point2D& p) const { return isCloseToZero(cross(Vector2D(p))); }

	bool equals(const Vector2D& other) const { return _x == other._x && _y == other._y; }
	bool operator == (const Vector2D& other) const { return equals(other); }
	bool operator != (const Vector2D& other) const { return !equals(other); }

private:
	double _x;
	double _y;
};

class Rect2D {
public:
	Rect2D() {}
	Rect2D(const Point2D& p1, const Point2D& p2);
	Rect2D(const Rect2D& other) : _minXY(other._minXY), _maxXY(other._maxXY) {}

	bool equals(const Rect2D& other) const { return _minXY == other._minXY && _maxXY == other._maxXY; }
	bool operator == (const Rect2D& other) const { return equals(other); }
	bool operator != (const Rect2D& other) const { return !equals(other); }

	bool isInside(const Point2D& p) const;
	bool inside(const Segment2D& v, Segment2D& outInside) const;

	const Point2D& minXY() const { return _minXY; }
	const Point2D& maxXY() const { return _maxXY; }

private:
	Point2D _minXY;
	Point2D _maxXY;
};

inline Rect2D::Rect2D(const Point2D& p1, const Point2D& p2)
{
	_minXY = Point2D(std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()));
	_maxXY = Point2D(std::max(p1.x(), p2.x()), std::max(p1.y(), p2.y()));
}

inline bool Rect2D::isInside(const Point2D& p) const
{
	return std::fmin(_minXY.x(), _maxXY.x()) <= p.x() && p.x() <= std::fmax(_minXY.x(), _maxXY.x())
		&& std::fmin(_minXY.y(), _maxXY.y()) <= p.y() && p.y() <= std::fmax(_minXY.y(), _maxXY.y());
}

inline bool Rect2D::inside(const Segment2D& s, Segment2D& outInside) const
{
	int n = 0;
	Point2D p00(_maxXY.x(), _minXY.y());
	Point2D p11(_minXY.x(), _maxXY.y());
	Point2D iPts[2];
	Segment2D tmp(_minXY, p00);
	if (tmp.intersection(s, iPts[n]))
	{
		n++;
	}
	tmp = Segment2D(_minXY, p11);
	if (tmp.intersection(s, iPts[n]))
	{
		n++;
	}
	if (n < 2)
	{
		tmp = Segment2D(p00, _maxXY);
		if (tmp.intersection(s, iPts[n]))
		{
			n++;
		}

		if (n < 2)
		{
			tmp = Segment2D(p11, _maxXY);
			if (tmp.intersection(s, iPts[n]))
			{
				n++;
			}
		}
	}

	if (n == 2)
	{
		outInside = Segment2D(iPts[0], iPts[1]);
	}
	else if (n == 1)
	{
		if (isInside(s.from()))
			outInside = Segment2D(s.from(), iPts[0]);
		else
			outInside = Segment2D(iPts[0], s.to());
	}
	return n > 0;
}


// ---- 3D ----

class Point3D {
public:
	Point3D() : _x(0.0), _y(0.0) {}
	Point3D(double x, double y, double z) : _x(x), _y(y), _z(z) {}
	Point3D(const Point2D& other) : _x(other.x()), _y(other.y()), _z(0.0) {}
	Point3D(const Point2D& other, double z) : _x(other.x()), _y(other.y()), _z(z) {}
	Point3D(const Point3D& other) : _x(other._x), _y(other._y), _z(other._z) {}

	double x() const { return _x; }
	double y() const { return _y; }
	double z() const { return _z; }

	bool equals(const Point3D& other) const { return _x == other._x && _y == other._y && _z == other._z; }
	bool operator == (const Point3D& other) const { return equals(other); }
	bool operator != (const Point3D& other) const { return !equals(other); }

private:
	double _x;
	double _y;
	double _z;
};

class Vector3D {
public:
	Vector3D() : _x(0.0), _y(0.0), _z(0.0) {}
	Vector3D(double x, double y, double z) : _x(x), _y(y), _z(z) {}
	Vector3D(const Vector3D& other) : _x(other._x), _y(other._y), _z(other._z) {}
	Vector3D(const Point3D& to) : _x(to.x()), _y(to.y()), _z(to.z()) {}
	Vector3D(const Point3D& from, const Point3D& to) : _x(to.x() - from.x()), _y(to.y() - from.y()), _z(to.z() - from.z()) {}

	double x() const { return _x; }
	double y() const { return _y; }
	double z() const { return _z; }

	double lengthSquared() const { return _x * _x + _y * _y + _z * _z; }
	double length() const { return std::sqrt(_x *_x + _y * _y + _z * _z); }

	Vector3D add(const Vector3D& other) const { return Vector3D(_x + other._x, _y + other._y, _z + other._z); }
	Vector3D substract(const Vector3D& other) const { return Vector3D(_x - other._x, _y - other._y, _z - other._z); }
	Vector3D scale(double ratio) const { return Vector3D(_x * ratio, _y * ratio, _z * ratio); }
	Vector3D normalize(double ratio) const { double l = length(); return Vector3D(_x / l, _y / l, _z / l); }

	Vector3D operator + (const Vector3D& other) const { return add(other); }
	Vector3D operator - (const Vector3D& other) const { return substract(other); }
	Vector3D operator * (double ratio) const { return scale(ratio); }
	Vector3D operator / (double ratio) const { return scale(1.0 / ratio); }

	Vector3D cross(const Vector3D& other) const;
	Point3D toPoint() const { return Point3D(_x, _y, _z); }

	bool equals(const Vector3D& other) const { return _x == other._x && _y == other._y && _z == other._z; }
	bool operator == (const Vector3D& other) const { return equals(other); }
	bool operator != (const Vector3D& other) const { return !equals(other); }

private:
	double _x;
	double _y;
	double _z;
};

inline Vector3D Vector3D::cross(const Vector3D& other) const {
	return Vector3D(
		_y*other._z - _z*other._y,
		_z*other._x - _x*other._z,
		_x*other._y - _y*other._x);
}

class Segment3D {
public:
	Segment3D() {}
	Segment3D(const Point3D& from, const Point3D& to) : _from(from), _to(to) {}
	Segment3D(const Segment3D& other) : _from(other._from), _to(other._to) {}

	bool equals(const Segment3D& other) const { return _from == other._from && _to == other._to; }
	bool operator == (const Segment3D& other) const { return equals(other); }
	bool operator != (const Segment3D& other) const { return !equals(other); }

	const Point3D& from() const { return _from; }
	const Point3D& to() const { return _to; }

private:
	Point3D _from;
	Point3D _to;
};


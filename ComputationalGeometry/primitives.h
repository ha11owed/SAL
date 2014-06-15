#pragma once

#include <cmath>

class Point2D {
public:
	Point2D() : _x(0.0), _y(0.0) {}
	Point2D(double x, double y) : _x(x), _y(y) {}
	Point2D(const Point2D& other) : _x(other._x), _y(other._y) {}

	double x() const { return _x; }
	double y() const { return _y; }

	bool equals(const Point2D& other) const { return _x == other._x && _y == other._y; }
	bool operator == (const Point2D& other) const { return equals(other); }
	bool operator != (const Point2D& other) const { return !equals(other); }

private:
	double _x;
	double _y;
};

class Point3D {
public:
	Point3D() : _x(0.0), _y(0.0) {}
	Point3D(double x, double y, double z) : _x(x), _y(y), _z(z) {}
	Point3D(const Point2D& other) : _x(other.x()), _y(other.y()), _z(0.0) {}
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

	bool equals(const Vector2D& other) const { return _x == other._x && _y == other._y; }
	bool operator == (const Vector2D& other) const { return equals(other); }
	bool operator != (const Vector2D& other) const { return !equals(other); }

private:
	double _x;
	double _y;
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
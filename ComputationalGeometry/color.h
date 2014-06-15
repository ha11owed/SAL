#pragma once

struct Color {
	Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
	~Color() {}

	float r;
	float g;
	float b;
};

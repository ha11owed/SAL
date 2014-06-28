#pragma once

#include <vector>
#include <complex>
#include "polynomial.h"

class FFT
{
public:
	FFT() {};
	~FFT() {};


private:
	std::vector<std::pair<double, double>> pointValues;
};


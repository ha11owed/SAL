#include "polynomial.h"
#include "fft.h"
#include <cassert>
#include <ctime>
#include <iostream>

// This might be prblematic on linux because of precision, but I'm to lazy to try it out
static std::clock_t _startTime;

void startTimer(const char* msg)
{
	std::cout << "Starting Timer: " << msg << std::endl;
	_startTime = std::clock();
}

double endTimer(const char* msg)
{
	std::clock_t endTime = std::clock();
	double diff = (double)(endTime - _startTime) / CLOCKS_PER_SEC;
	std::cout << "Ending Timer:   " << msg << " in " << diff << " seconds" << std::endl;
	return diff;
}

void testPolynomialCalculate()
{
	double r;
	Polynomial p(2);
	p[1] = 7.5;
	p[0] = -2.5;

	r = p.calculate(0);
	assert(r == -2.5);
	r = p.calculate(1);
	assert(r == 5.0);
	r = p.calculate(0.33333333333333333333333333333);
	assert(r == 0.0);
}

void testMultiplication()
{
	const size_t NR_TESTS = 50;
	const size_t N = 10000;

	Polynomial p1Orig(N);
	Polynomial p2Orig(N);
	for (size_t i = 0; i < N; i++)
	{
		p1Orig[i] = 0.00001 * std::rand();
		p2Orig[i] = 0.00001 * std::rand();
	}


	startTimer("Naive polynomial multiplication");
	for (size_t j = 0; j < NR_TESTS; j++)
	{
		Polynomial p1(p1Orig);
		Polynomial p2(p2Orig);
		Polynomial r;
		p1.multiplyNaive(p2, r);
	}
	endTimer("Naive polynomial multiplication");

	startTimer("FFT polynomial multiplication");
	for (size_t j = 0; j < NR_TESTS; j++)
	{
		PolynomialComplex p1fft(p1Orig.size());
		for(size_t k = 0; k < p1Orig.size(); k++)
		{
			p1fft[k] = p1Orig[k];
		}
		PolynomialComplex p2fft(p2Orig.size());
		for(size_t k = 0; k < p2Orig.size(); k++)
		{
			p2fft[k] = p2Orig[k];
		}
		fft::transformInplace(p1fft, false);
		fft::transformInplace(p2fft, false);
		for(size_t k = 0; k < p1fft.size(); k++)
		{
			p1fft[k] = p1fft[k] * p2fft[k];
		}

		fft::transformInplace(p1fft, true);
	}
	endTimer("FFT polynomial multiplication");
}

int main(int argc, char** argv)
{
	testPolynomialCalculate();

	testMultiplication();

	char _c;
	std::cin >> _c;
	return 0;
}

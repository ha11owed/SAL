#pragma once

#include <vector>
#include <algorithm>

class Polynomial
{
public:
	Polynomial(size_t n) : coefficients(n, 0.0) {}
	Polynomial(const Polynomial& other) : coefficients(other.coefficients) {}
	Polynomial() {}
	~Polynomial() {}

	double& operator[](size_t index)                   { return coefficients[index]; }
	double operator[](size_t index) const              { return coefficients[index]; }
	size_t size() const                                { return coefficients.size(); }
	size_t resize(size_t newSize, double defaultValue) { coefficients.resize(newSize, defaultValue); }

	double calculate(double x) const;
	void add(const Polynomial& p, Polynomial& outResult) const;
	void multiplyNaive(const Polynomial& p, Polynomial& outResult) const;
	void multiplyKaratsuba(const Polynomial& p, Polynomial& outResult) const;

private:
	std::vector<double> coefficients;
};

inline double Polynomial::calculate(double x) const
{
	double px = 1.0, r = 0.0;

	size_t n = coefficients.size();
	for (size_t i = 0; i < n; i++)
	{
		r += px * coefficients[i];
		px *= x;
	}
	return r;
}

inline void Polynomial::add(const Polynomial& other, Polynomial& outResult) const
{
	size_t n = coefficients.size();
	size_t m = other.coefficients.size();
	size_t mn = std::min(m, n);
	size_t mx = std::max(m, n);

	outResult.coefficients.resize(mx, 0.0);
	for (size_t i = 0; i < mn; i++)
	{
		outResult.coefficients[i] = coefficients[i] + other.coefficients[i];
	}

	const std::vector<double>* v = &(other.coefficients);
	if (n > m)
	{
		v = &coefficients;
	}

	for (size_t i = mn; i < mx; i++)
	{
		outResult.coefficients[i] = v->at(mn + i);
	}
}

inline void Polynomial::multiplyNaive(const Polynomial& other, Polynomial& outResult) const
{
	const std::vector<double>& p1 = coefficients;
	const std::vector<double>& p2 = other.coefficients;
	std::vector<double>& res = outResult.coefficients;

	size_t n1 = p1.size();
	size_t n2 = p2.size();

	res.resize(n1 * n2);
	for (size_t i = 0; i < n1; i++)
	{
		for (size_t j = 0; j < n2; j++)
		{
			res[i + j] += (p1[i] * p2[j]);
		}
	}
}

inline void Polynomial::multiplyKaratsuba(const Polynomial& other, Polynomial& outResult) const
{
	const std::vector<double>& p1 = coefficients;
	const std::vector<double>& p2 = other.coefficients;
	std::vector<double>& res = outResult.coefficients;

	auto karatsuba = [](size_t n, const std::vector<double>& p1, const std::vector<double>& p2)
	{

	};

	size_t n1 = p1.size();
	size_t n2 = p2.size();

	res.resize(n1 * n2);
	for (size_t i = 0; i < n1; i++)
	{
		for (size_t j = 0; j < n2; j++)
		{
			res[i + j] += (p1[i] * p2[j]);
		}
	}
}


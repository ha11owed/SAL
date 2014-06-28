#pragma once

#include <vector>
#include <complex>
#include <algorithm>

template<typename T>
class TPolynomial
{
public:
	TPolynomial(size_t n) : coefficients(n) {}
	TPolynomial(const TPolynomial& other) : coefficients(other.coefficients) {}
	TPolynomial() {}
	~TPolynomial() {}

	      T&   operator[](size_t index)                       { return coefficients[index]; }
	const T&   operator[](size_t index) const                 { return coefficients[index]; }
	size_t     size() const                                   { return coefficients.size(); }
	void       resize(size_t newSize,  const T& defaultValue) { coefficients.resize(newSize, defaultValue); }
	void       resize(size_t newSize)                         { coefficients.resize(newSize); }

	T calculate(const T& x) const;
	void add(const TPolynomial& p, TPolynomial& outResult) const;
	void multiplyNaive(const TPolynomial& p, TPolynomial& outResult) const;
	void multiplyKaratsuba(const TPolynomial& p, TPolynomial& outResult) const;

private:
	std::vector<T> coefficients;
};

template<typename T>
inline T TPolynomial<T>::calculate(const T& x) const
{
	T px = 1.0, r = 0.0;

	size_t n = coefficients.size();
	for (size_t i = 0; i < n; i++)
	{
		r += px * coefficients[i];
		px *= x;
	}
	return r;
}

template<typename T>
inline void TPolynomial<T>::add(const TPolynomial& other, TPolynomial& outResult) const
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

	const std::vector<T>* v = &(other.coefficients);
	if (n > m)
	{
		v = &coefficients;
	}

	for (size_t i = mn; i < mx; i++)
	{
		outResult.coefficients[i] = v->at(mn + i);
	}
}

template<typename T>
inline void TPolynomial<T>::multiplyNaive(const TPolynomial& other, TPolynomial& outResult) const
{
	const std::vector<T>& p1 = coefficients;
	const std::vector<T>& p2 = other.coefficients;
	std::vector<T>& res = outResult.coefficients;

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

template<typename T>
inline void TPolynomial<T>::multiplyKaratsuba(const TPolynomial& other, TPolynomial& outResult) const
{
	const std::vector<T>& p1 = coefficients;
	const std::vector<T>& p2 = other.coefficients;
	std::vector<T>& res = outResult.coefficients;

	auto karatsuba = [](size_t n, const std::vector<T>& p1, const std::vector<T>& p2)
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

// we are not really going to work with so many types, just real and complex.
typedef TPolynomial<double>                    Polynomial;
typedef TPolynomial< std::complex<double> >    PolynomialComplex;



#pragma once

#include "polynomial.h"

namespace fft
{
	void transformInplace(PolynomialComplex& polynomial, bool inverse);

	void transformDirect(const Polynomial& polynomial, PolynomialComplex& outTransformed);

	void transformInverse(const PolynomialComplex& polynomial, Polynomial& outTransformed);
	
}

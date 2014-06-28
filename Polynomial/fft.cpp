#include "fft.h"
#include <cassert>

namespace fft
{
	inline void findPowOfToAndLog(int n, int& n2, int& log2n)
	{
		log2n = 0;
		n2 = 1;
		while(n2 < n)
		{
			n2 = n2 << 1;
			log2n++;
		}
	}

	void transformInplace(PolynomialComplex& polynomial, bool inverse)
	{
		double angle, wtmp, wpr, wpi, wr, wi;
		int n = 1, n2;
		double pi = 3.14159265358979323846;
		double pi2 = 3.14159265358979323846 * 2.0;

		int size = (int) polynomial.size();
		double scale = 1.0/size;
		std::complex<double> tc;

		int log2n = 0, size2 = 1;
		findPowOfToAndLog(size, size2, log2n);

		if(size != size2)
		{
			polynomial.resize(size2);
		}

		for (int k = 0; k < log2n; ++k)
		{
			n2 = n;
			n <<= 1;
			angle = (inverse)?pi2/n:-pi2/n;
			wtmp=sin(0.5*angle);
			wpr = -2.0*wtmp*wtmp;
			wpi = sin(angle);
			wr = 1.0;
			wi = 0.0;

			for (int m=0; m < n2; ++m)
			{
				for (int i=m; i < size; i+=n)
				{
					int j = i+n2;
					tc.real(wr * polynomial[j].real() - wi * polynomial[j].imag());
					tc.imag(wr * polynomial[j].imag() + wi * polynomial[j].real());
					polynomial[j] = polynomial[i] - tc;
					polynomial[i] += tc;
				}
				wr=(wtmp=wr)*wpr-wi*wpi+wr;
				wi=wi*wpr+wtmp*wpi+wi;
			}
		}
		if(inverse)
		{
			for(int i = 0;i < n;i++)
			{
				polynomial[i] *= scale;
			}
		}
	}

	void transformDirect(const Polynomial& polynomial, PolynomialComplex& outTransformed)
	{
		int n = polynomial.size();
		int log2n, n2;
		findPowOfToAndLog(n, n2, log2n);
		outTransformed.resize(n2);
		for(int i=0; i<n; i++)
		{
			outTransformed[i] = polynomial[i];
		}
		transformInplace(outTransformed, false);
	}

	void transformInverse(const PolynomialComplex& polynomial, Polynomial& outTransformed)
	{
		int n = polynomial.size();
		int log2n, n2;
		findPowOfToAndLog(n, n2, log2n);
		PolynomialComplex tmp(n2);
		for(int i =0; i<n; i++)
		{
			tmp[i] = polynomial[i];
		}
		transformInplace(tmp, true);
		outTransformed.resize(n2);
		for(int i=0; i<n; i++)
		{
			outTransformed[i] = tmp[i].real();
		}
	}

	void multiply(const Polynomial& p1, const Polynomial& p2, Polynomial& outResult)
	{
		PolynomialComplex p1fft(p1.size());
		for(size_t k = 0; k < p1.size(); k++)
		{
			p1fft[k] = p1[k];
		}
		PolynomialComplex p2fft(p2.size());
		for(size_t k = 0; k < p2.size(); k++)
		{
			p2fft[k] = p2[k];
		}
		fft::transformInplace(p1fft, false);
		fft::transformInplace(p2fft, false);
		outResult.resize(p1fft.size());
		for(size_t k = 0; k < p1fft.size(); k++)
		{
			p1fft[k] = p1fft[k] * p2fft[k];
		}

		fft::transformInplace(p1fft, true);

		std::complex<double> c;
		outResult.resize(p1fft.size());
		for(size_t k = 0; k < p1fft.size(); k++)
		{
			outResult[k] = p1fft[k].real();
		}
	}

}


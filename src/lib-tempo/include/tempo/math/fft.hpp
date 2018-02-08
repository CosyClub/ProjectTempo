#ifndef TEMPO_MATH_FFT_HPP
#define TEMPO_MATH_FFT_HPP

#include <stdint.h>
#include <iostream>
#include <valarray>
#include <complex>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

namespace tempo
{
	void fft(CArray& x);
	void ifft(CArray& x);
}
	
#endif

#pragma once

#include <fftw3.h>

void extractPhase(const fftw_complex* fft, fftw_complex* pfft, size_t size);

void normalizeComplexArray(fftw_complex* fft, size_t size);

//fft /= v
void divideComplexArray(fftw_complex* fft, double v, size_t size);

double getComplexArrayMaxAbs(fftw_complex* fft, size_t size);
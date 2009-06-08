#include "phase_saliency_detection_utils.h"

#include <cmath>

void extractPhase(const fftw_complex* fft, fftw_complex* pfft, size_t size) {
	double norm = 1.0;
	for (size_t i = 0; i < size; i++) {
		norm = sqrt(fft[i][0] * fft[i][0] + fft[i][1] * fft[i][1]);
		pfft[i][0] = fft[i][0] / norm;
		pfft[i][1] = fft[i][1] / norm;
	}
}

void normalizeComplexArray(fftw_complex* fft, size_t size) {
	double norm = 1.0;
	for (size_t i = 0; i < size; i++) {
		norm = sqrt(fft[i][0] * fft[i][0] + fft[i][1] * fft[i][1]);
		fft[i][0] /= norm;
		fft[i][1] /= norm;
	}
}

void divideComplexArray(fftw_complex* fft, double v, size_t size) {
	for (size_t i = 0; i < size; i++) {
		fft[i][0] /= v;
		fft[i][1] /= v;
	}
}

double getComplexArrayMaxAbs(fftw_complex* fft, size_t size) {
	double maxs = 0;
	for (size_t i = 0; i < size; i++) {
		double s = (fft[i][0] * fft[i][0] + fft[i][1] * fft[i][1]);
		if (maxs < s) maxs = s;
	}
	return maxs;
}
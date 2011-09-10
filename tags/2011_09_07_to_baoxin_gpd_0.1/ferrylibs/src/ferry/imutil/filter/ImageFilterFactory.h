#pragma once

#include <cassert>
#include <algorithm>

#include "ImageFilter.h"
#include "OpenCvSmoothFilter.h"
#include "SieveFilter.h"
#include "NearMinFilter.h"

using namespace std;

namespace ferry { namespace imutil { namespace filter {

/**
 * Configure image filter by parameters
 */
class ImageFilterFactory : public ImageFilter {

public:
	static enum FILTER_METHOD {
		OPENCV_BLUR_NO_SCALE, 
		OPENCV_BLUR, 
		OPENCV_GAUSSIAN, 
		OPENCV_MEDIAN, 
		OPENCV_BILATERAL, 
		SIEVE, 
		NEAR_MIN
	};

public:
	ImageFilterFactory(void) {
		filter = NULL;
	}

	~ImageFilterFactory(void) {
	}

public:
	//if param < 0, use default
	static ImageFilter* createImageFilter(FILTER_METHOD method, double param1 = -1, double param2 = -1, double param3 = -1, double param4 = -1) {
		int cvmethod = CV_BLUR_NO_SCALE;
		int cvparam1 = (param1 < 0) ? 3 : (int)param1;
		int cvparam2 = (param2 < 0) ? 0 : (int)param2;
		double cvparam3 = (param3 < 0) ? 0 : param3;
		switch(method) {
			case OPENCV_BLUR_NO_SCALE:
				return new OpenCvSmoothFilter(CV_BLUR_NO_SCALE, cvparam1, cvparam2, cvparam3);
			case OPENCV_BLUR:
				return new OpenCvSmoothFilter(CV_BLUR, cvparam1, cvparam2, cvparam3);
			case OPENCV_GAUSSIAN:
				return new OpenCvSmoothFilter(CV_GAUSSIAN, cvparam1, cvparam2, cvparam3);
			case OPENCV_MEDIAN:
				return new OpenCvSmoothFilter(CV_MEDIAN, cvparam1, cvparam2, cvparam3);
			case OPENCV_BILATERAL:
				return new OpenCvSmoothFilter(CV_BILATERAL, cvparam1, cvparam2, cvparam3);
			case SIEVE:
				if (param1 < 0) param1 = 2;
				if (param2 < 0) param2 = 2;
				if (param3 < 0) param3 = 0.2;
				if (param4 < 0) param4 = 1;
				return new SieveFilter((int)param1, (int)param2, param3, (int)param4);
			case NEAR_MIN:
				if (param1 < 0) param1 = 2;
				if (param2 < 0) param2 = 2;
				return new NearMinFilter((int)param1, (int)param2);
		}

		return NULL;
	}

public:
	void setParameters(FILTER_METHOD method, double param1 = -1, double param2 = -1, double param3 = -1, double param4 = -1) {
		if (filter != NULL) {
			delete filter;
		}
		filter = createImageFilter(method, param1, param2, param3, param4);
	}

	IplImage* process(IplImage* src, IplImage* dst) {
		return filter->process(src, dst);
	}

private:
	FILTER_METHOD method;
	double param1;
	double param2;
	double param3;
	double param4;

	ImageFilter* filter;
};

} } }

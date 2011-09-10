#pragma once

#include "ImageFilter.h"

namespace ferry { namespace imutil { namespace filter {

/**
 * @see cvSmooth
 */
class OpenCvSmoothFilter : public ImageFilter {

public:
	OpenCvSmoothFilter(int smoothtype = CV_GAUSSIAN, int param1 = 3, int param2 = 0, double param3 = 0) {
		this->smoothtype = smoothtype;
		this->param1 = param1;
		this->param2 = param2;
		this->param3 = param3;
	}

	~OpenCvSmoothFilter(void) {
	}

public:
	void setSmoothType(int type) {
		this->smoothtype = type;
	}

	void setParam1(int param1) {
		this->param1 = param1;
	}

	void setParam2(int param2) {
		this->param2 = param2;
	}

	void setParam3(double param3) {
		this->param3 = param3;
	}

public:
	IplImage* process(IplImage* src, IplImage* dst) {
		cvSmooth(src, dst, smoothtype, param1, param2, param3);
		return dst;
	}

private:
	int smoothtype;
	int param1;
	int param2;
	double param3;
};

} } }

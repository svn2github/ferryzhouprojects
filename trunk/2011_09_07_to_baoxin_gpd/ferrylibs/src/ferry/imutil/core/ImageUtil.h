#pragma once

#include "cv.h"

namespace ferry { namespace imutil { namespace core {


	IplImage* lut(IplImage* src, short* lutmap, IplImage* dst = NULL);

	bool isHeaderEqual(IplImage* im1, IplImage* im2);

	//string getImageInfo(IplImage* im);

	void synchronizeBuffer(IplImage* src, IplImage** buffer);

} } }
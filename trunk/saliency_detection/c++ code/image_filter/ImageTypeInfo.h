#pragma once

#include <cv.h>

namespace image_filter {

class ImageTypeInfo {
public:
	ImageTypeInfo();
	ImageTypeInfo(const CvSize& size, int depth, int channels);
	~ImageTypeInfo(void);

	///default copy constructors
	///default copy assignments
public:
	static ImageTypeInfo getTypeInfo(const IplImage* im);

	bool operator== (const ImageTypeInfo& rhs);

	static bool imageTypeEquals(const IplImage* im1, const IplImage* im2);

public:
	CvSize size;
	int depth;
	int channels;
};

}
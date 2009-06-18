#pragma once

#include <cv.h>
#include <string>

namespace fvision {

class CvMatFormatter {
public:
	CvMatFormatter(void);
	~CvMatFormatter(void);

public:
	std::string getString(const CvMat* mat) const ;
};

}
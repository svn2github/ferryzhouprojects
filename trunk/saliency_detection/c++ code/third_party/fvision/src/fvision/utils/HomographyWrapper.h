#pragma once

#include <cv.h>

namespace fvision {

/**
 * H = M * T * inv(M)
 */
class HomographyWrapper {
public:
	HomographyWrapper(const CvMat* M);
	~HomographyWrapper(void);

public:
	const CvMat* wrap(const CvMat* T);

private:
	CvMat* M;
	CvMat* invM;
	CvMat* H;
	CvMat* buf;
};

}
#ifndef FVISION_CVMATWRAPPER_H_
#define FVISION_CVMATWRAPPER_H_

#include <cv.h>

#include <iostream>

namespace fvision {

//for safe copy, assignment of CvMat*
class CvMatWrapper {
public:
	CvMatWrapper() {
		this->mat = NULL;
	}

	//clone mat
	CvMatWrapper(const CvMat* mat);

	CvMatWrapper(const CvMatWrapper& mw);

	~CvMatWrapper();

	const CvMatWrapper& operator=(const CvMatWrapper& rhs);

	const CvMatWrapper& operator=(const CvMat* rhs);

public:
	const CvMat* getMat() const { return mat; }

	CvMat* getMat() { return mat; }

private:
	CvMat* mat;
};

std::ostream& operator<< (std::ostream& os, const CvMatWrapper& mw);

}
#endif // FVISION_CVMATWRAPPER_H_

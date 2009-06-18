#include "CvMatFormatter.h"

#include <sstream>
#include <iomanip>

using namespace std;

namespace fvision {

CvMatFormatter::CvMatFormatter(void)
{
}

CvMatFormatter::~CvMatFormatter(void)
{
}

std::string CvMatFormatter::getString(const CvMat* mat) const {
	if (mat == NULL) {
		return "NULL";
	}
	ostringstream oss;
	oss << mat->rows << " " << mat->cols << " " << mat->type <<"\n";
	oss<<setprecision(6)<<fixed;
	for (int j = 0; j < mat->cols; j++) {
		oss<<setw(10)<<j<<" ";
	}
	oss<<"\n";
	for (int i = 0; i < mat->rows; i++) {
		for (int j = 0; j < mat->cols; j++) {
			oss<<setw(10) << cvmGet(mat, i, j) << " ";
		}
		oss << "\n";
	}
	return oss.str();
}
}
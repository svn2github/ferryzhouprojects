#pragma once

#include <vector>
#include <iostream>

#include "cv.h"
#include "CvMatUtil.h"
#include "PointCorrespondences.h"
#include "Camera.h"

using namespace std;
using namespace ferry::cv_mat;
using namespace ferry::cv_geometry;

namespace ferry {
	namespace cv_geometry {
		namespace io_util {

			ostream& operator<<(ostream& os, const vector<CvPoint2D32f>& pts);
			istream& operator>>(istream& is, vector<CvPoint2D32f>& pts);

			ostream& operator<<(ostream& os, const CvPoint2D32f& pt);
			istream& operator>>(istream& is, CvPoint2D32f& pt);

			ostream& operator<<(ostream& os, const CvPoint3D32f& pt);
			istream& operator>>(istream& is, CvPoint3D32f& pt);

			ostream& operator<<(ostream& os, const vector<CvPoint3D32f>& pts);
			istream& operator>>(istream& is, vector<CvPoint3D32f>& pts);

			ostream& operator<<(ostream& os, const vector<CvPoint>& pts);
			istream& operator>>(istream& is, vector<CvPoint>& pts);

			ostream& operator<<(ostream& os, const CvPoint& pt);
			istream& operator>>(istream& is, CvPoint& pt);

			ostream& operator<<(ostream& os, const CvMat* M);
			istream& operator>>(istream& is, CvMat** pM);
			
			ostream& operator<<(ostream& os, const PointCorrespondences& pc);
			istream& operator>>(istream& is, PointCorrespondences& pc);

			ostream& operator<<(ostream& os, Camera* pcam);
			istream& operator>>(istream& is, Camera** pcam);

			CvMat* loadMat(const char* filename);
			PointCorrespondences loadPointCorrespondences(const char* filename);
			Camera* loadCamera(const char* filename);

			void toMatlabFile(ostream& os, const CvMat* M, const char* varname);
		}
	}
}
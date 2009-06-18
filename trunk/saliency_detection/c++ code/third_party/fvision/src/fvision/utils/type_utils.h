#pragma once

#include <vector>

namespace fvision {

typedef std::vector<int>     v_int;
typedef std::vector<double>  v_double;
typedef std::vector<float>   v_float;

typedef std::vector<CvPoint2D32f>  v_CvPoint2D32f;
typedef std::vector<CvPoint>       v_CvPoint;
typedef std::vector<CvPoint3D32f>  v_CvPoint3D32f;
typedef std::vector<CvPoint3D64f>  v_CvPoint3D64f;

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)

}

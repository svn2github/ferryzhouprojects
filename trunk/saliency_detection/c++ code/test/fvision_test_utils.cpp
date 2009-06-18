#include "fvision_test_utils.h"

#include <cppunit/testassert.h>
#include <fvision/utils/cvmat_utils.h>

#include <cassert>

using namespace std;

namespace fvision {

void assertMatEquals(const CvMat* src, const CvMat* dst, double delta) {
	assertMatEqualsMessage("", src, dst, delta);
}

void assertMatEqualsMessage(const char* message, const CvMat* src, const CvMat* dst, double delta /*= FLT_EPSILON*/) {
	assert(src->rows == dst->rows 
		&& src->cols == dst->cols
		&& src->type == dst->type);

	CvMat* diff = cvCreateMat(src->rows, src->cols, src->type);
	cvAbsDiff(src, dst, diff);
	double maxv, minv;
	cvMinMaxLoc(diff, &minv, &maxv);

	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(message, 0.0, maxv, delta);

	cvReleaseMat(&diff);
}

void assertHomogeneousMatEqualsMessage(const char* message, const CvMat* src, const CvMat* dst, double delta) {
	CvMat* nSrc = getNormalizedMatrix2(src);
	CvMat* nDst = getNormalizedMatrix2(dst);

	assertMatEqualsMessage(message, nSrc, nDst, delta);
	
	cvReleaseMat(&nSrc);
	cvReleaseMat(&nDst);
}

void assertPointEquals(CvPoint3D32f p1, CvPoint3D32f p2, double value) {
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p2.x, value);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p2.y, value);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p2.z, value);
}

void assertPointsEqual(const vector<CvPoint3D32f>& p1s, const vector<CvPoint3D32f>& p2s, double value) {
	for (size_t i = 0; i < p1s.size(); i++) {
		assertPointEquals(p1s[i], p2s[i], value);
	}
}

void assertPointEquals(CvPoint3D64f p1, CvPoint3D64f p2, double value) {
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p2.x, value);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p2.y, value);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p1.z, p2.z, value);
}

void assertPointEquals(CvPoint2D32f p1, CvPoint2D32f p2, double value) {
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p1.x, p2.x, value);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(p1.y, p2.y, value);
}

void assertPointsEqual(const vector<CvPoint2D32f>& p1s, const vector<CvPoint2D32f>& p2s, double value) {
	for (size_t i = 0; i < p1s.size(); i++) {
		assertPointEquals(p1s[i], p2s[i], value);
	}
}

void assertPointEqualsMessage(const char* message, CvPoint3D64f p1, CvPoint3D64f p2, double value /*= FLT_EPSILON*/) {
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(message, p1.x, p2.x, value);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(message, p1.y, p2.y, value);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(message, p1.z, p2.z, value);
}
}
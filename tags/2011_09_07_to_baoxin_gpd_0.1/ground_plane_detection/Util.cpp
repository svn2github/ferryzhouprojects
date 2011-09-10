#include ".\util.h"

#include <ferry/cv_geometry/CvMatUtil.h>
#include <ferry/cv_geometry/BasicUtil.h>

using namespace ferry::cv_mat;
using namespace ferry::cv_geometry;

IplImage* compare(IplImage* im1, IplImage* im2, CvMat* H) {

	CvSize size = cvGetSize(im1);

	IplImage* im1h = cvCreateImage(size, 8, 3);
	cvWarpPerspective(im1, im1h, H);

	cvNamedWindow("im1 h", 1);
	cvShowImage("im1 h",im1h);
	
	IplImage* diff1 = cvCreateImage(size, 8, 3);
	IplImage* diff2 = cvCreateImage(size, 8, 3);
	IplImage* diff = cvCreateImage(size, 8, 3);

	cvSub(im2, im1h, diff1);
	cvSub(im1h, im2, diff2);
	cvAdd(diff1, diff2, diff);

	cvNamedWindow("diff", 1);
	cvShowImage("diff",diff);

	return diff;
}


void drawLines(IplImage* im, vector<RhoThetaLine>& vlines) {
	for (int i = 0; i < vlines.size(); i++) {
		float rho = vlines[i].rho;
		float theta = vlines[i].theta;

        CvPoint pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        cvLine( im, pt1, pt2, CV_RGB(0,255,0), 1, 8 );

	}
}

CvMat* getLine(CvPoint2D32f p1, CvPoint2D32f p2) {
	CvMat* M = cvCreateMat(3, 1, CV_64FC1);
	double mh[] = {p1.y - p2.y, p2.x - p1.x, p1.x * p2.y - p1.y * p2.x};
	cvSetData(M, mh, CV_AUTOSTEP);
	return cloneMat(M);
}

CvMat* getLine(CvPoint p1, CvPoint p2) {
	CvMat* M = cvCreateMat(3, 1, CV_64FC1);
	double mh[] = {p1.y - p2.y, p2.x - p1.x, p1.x * p2.y - p1.y * p2.x};
	cvSetData(M, mh, CV_AUTOSTEP);
	return cloneMat(M);
}

	vector<CvMat*> linesToMats(vector<RhoThetaLine>& vlines) {
		vector<CvMat*> lines;
		for (int i = 0; i < vlines.size(); i++) {
			CvMat* M = cvCreateMat(3, 1, CV_64FC1);
			double mh[] = {cos(vlines[i].theta), sin(vlines[i].theta), -vlines[i].rho};
			cvSetData(M, mh, CV_AUTOSTEP);
			lines.push_back(cloneMat(M));
		}
		return lines;
	}

CvPoint3D32f normalize(const CvPoint3D32f& p) {
	double r = 1 / sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	return scalePoint3D(p, r);
}

CvPoint3D32f addPoint3D(const CvPoint3D32f& p1, const CvPoint3D32f& p2) { 
	return cvPoint3D32f(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}


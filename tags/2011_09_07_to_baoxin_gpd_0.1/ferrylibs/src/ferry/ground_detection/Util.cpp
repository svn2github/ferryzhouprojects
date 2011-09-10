#include ".\util.h"

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
        cvLine( im, pt1, pt2, CV_RGB(0,255,0), 2, 8 );

	}
}

#include ".\drawingutil.h"
#include ".\basicutil.h"

namespace ferry {
	namespace cv_geometry {

	IplImage* create_correspondences_image(IplImage* image, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s) {
		IplImage* cross = cvCreateImage(cvGetSize(image), 8, image->nChannels);
		cvCopy(image, cross);
		for (int i = 0; i < (int)x1s.size(); i++) {
			drawCross(cross, x1s[i]);
			cvLine(cross, x1s[i], x2s[i], CV_RGB(255, 0, 0));
			//cvLine(cross, x1s[i], x2s[i], CV_RGB(0, 0, 0), 2);
		}
		return cross;
	}

	IplImage* create_numbers_correspondences_image(IplImage* image, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s, CvScalar color) {
		IplImage* cross = create_correspondences_image(image, x1s, x2s);
		drawNumbers(cross, x1s, color);
		return cross;
	}

	void drawNumbers(IplImage* image, const vector<CvPoint>& x1s, CvScalar color) {
		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4f, 0.4f);
		for (int i = 0; i < x1s.size(); i++) {
			char text[10];
			sprintf(text, "%d", i);
			cvPutText(image, text, x1s[i], &font, color);
		}
	}

	void drawLines(IplImage* image, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s, CvScalar color) {
		for (int i = 0; i < (int)x1s.size(); i++) {
			drawCross(image, x1s[i]);
			cvLine(image, x1s[i], x2s[i], color);
		}
	}

	void draw_numbers_correspondences_image(char* winname, IplImage* image, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s, CvScalar color) {
		IplImage* cross = create_numbers_correspondences_image(image, x1s, x2s, color);
		cvNamedWindow(winname, 1);
		cvShowImage(winname, cross);
		cvReleaseImage(&cross);
	}

	void draw_correspondences_image(char* winname, IplImage* image, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s) {
		IplImage* cross = create_correspondences_image(image, x1s, x2s);
		cvNamedWindow(winname, 1);
		cvShowImage(winname, cross);
		cvReleaseImage(&cross);
	}

	void draw_correspondences_image(char* winname, IplImage* image, const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s) {
		draw_correspondences_image(winname, image, cvPointsFrom32fs(x1s), cvPointsFrom32fs(x2s));
	}

	void save_correspondences(char* filename, IplImage* image, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s) {
		IplImage* cross = create_correspondences_image(image, x1s, x2s);
		cvSaveImage(filename, cross);
		cvReleaseImage(&cross);
	}

	IplImage* create_correspondences_image(IplImage* image1, IplImage* image2, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s) {
		CvSize size = cvGetSize(image1);
		CvSize double_size = cvSize((int)size.width * 1.5, size.height * 2);
		IplImage* cross = cvCreateImage(double_size, 8, image1->nChannels);
		
		cvSetImageROI(cross, cvRect(0, 0, size.width, size.height));
		cvCopy(image1, cross);
		drawCrosses(cross, x1s, CV_RGB(0, 255, 0));

		CvSize size2 = cvGetSize(image2);
		cvSetImageROI(cross, cvRect(size.width / 2, size.height, size2.width, size2.height));
		cvCopy(image2, cross);
		drawCrosses(cross, x2s, CV_RGB(0, 255, 0));
		
		cvSetImageROI(cross, cvRect(0, 0, double_size.width, double_size.height));
		for (int i = 0; i < (int)x1s.size(); i++) {
			cvLine(cross, x1s[i], cvPoint(x2s[i].x + size.width/2, x2s[i].y + size.height), CV_RGB(255, 0, 0));
		}
		return cross;
	}

	void draw_correspondences_image(char* winname, IplImage* image1, IplImage* image2, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s) {
		IplImage* cross = create_correspondences_image(image1, image2, x1s, x2s);
		cvNamedWindow(winname, 1);
		cvShowImage(winname, cross);
		cvReleaseImage(&cross);		
	}

	void save_correspondences(char* filename, IplImage* image1, IplImage* image2, const vector<CvPoint>& x1s, const vector<CvPoint>& x2s) {
		IplImage* cross = create_correspondences_image(image1, image2, x1s, x2s);
		cvSaveImage(filename, cross);
		cvReleaseImage(&cross);
	}

	void drawCornersImage(char* winname, IplImage* image, const vector<CvPoint>& corners) {
		IplImage* cross = cvCreateImage(cvGetSize(image), 8, image->nChannels);
		cvCopy(image, cross);

		drawCrosses(cross, corners, CV_RGB(0, 255, 0));

		cvNamedWindow(winname, 1);
		cvShowImage(winname, cross);
		cvReleaseImage(&cross);
	}

	IplImage* createGroundObstaclePointsImage(IplImage* image, const vector<CvPoint>& gxs, const vector<CvPoint>& oxs, CvScalar g_color, CvScalar o_color) {
		IplImage* cross = cvCreateImage(cvGetSize(image), 8, image->nChannels);
		cvCopy(image, cross);

		drawCrosses(cross, gxs, g_color);
		drawCrosses(cross, oxs, o_color);

		return cross;
	}

	void drawGroundObstaclePointsImage(char* winname, IplImage* image, const vector<CvPoint>& gxs, const vector<CvPoint>& oxs, CvScalar g_color, CvScalar o_color) {
		IplImage* cross = createGroundObstaclePointsImage(image, gxs, oxs, g_color, o_color);
		cvNamedWindow(winname, 1);
		cvShowImage(winname, cross);
		cvReleaseImage(&cross);
	}

	IplImage* createCornersImage(IplImage* image, const vector<CvPoint>& corners) {
		IplImage* cross = cvCreateImage(cvGetSize(image), 8, image->nChannels);
		cvCopy(image, cross);

		drawCrosses(cross, corners, CV_RGB(0, 255, 0));

		return cross;
	}

	void saveCorners(char* filename, IplImage* image, const vector<CvPoint>& corners, CvScalar color) {
		IplImage* cross = cvCreateImage(cvGetSize(image), 8, image->nChannels);
		cvCopy(image, cross);

		drawCrosses(cross, corners, color);
		cvSaveImage(filename, cross);		
		cvReleaseImage(&cross);
	}

	void drawCrosses(IplImage* cross, const vector<CvPoint>& pts, CvScalar color, int thickness) {
		for (int i = 0; i < (int)pts.size(); i++) {
			drawCross(cross, pts[i], color, thickness);
		}
	}

	void drawCross(char* winname, IplImage* image, CvPoint pt) {
		
		IplImage* cross = cvCreateImage(cvGetSize(image), 8, image->nChannels);
		cvCopy(image, cross);

		drawCross(cross, pt);

		cvNamedWindow(winname, 1);
		cvShowImage(winname, cross);
		cvReleaseImage(&cross);
	}

	void drawCross(IplImage* cross, CvPoint pt) {
		drawCross(cross, pt, CV_RGB(0, 255, 0));
		//drawCross(cross, pt, CV_RGB(255, 255, 255));
	}

	void drawCross(IplImage* cross, CvPoint pt, CvScalar color, int thickness) {
		CvPoint pt1, pt2, pt3, pt4;
		//cout<<pt.x<<", "<<pt.y<<endl;
		int len = 5;
		pt1.x = pt.x - len;
		pt1.y = pt.y;
		pt2.x = pt.x + len;
		pt2.y = pt.y;
		pt3.x = pt.x;
		pt3.y = pt.y - len;
		pt4.x = pt.x;
		pt4.y = pt.y + len;
		cvLine(cross, pt1, pt2, color, thickness);
		cvLine(cross, pt3, pt4, color, thickness);
	}

	void drawEpipoleLines(IplImage* im1, IplImage* im2, CvMat* F, CvScalar color) {
		CvMat* e1 = CvMatUtil::null(F);

		CvSize s = cvGetSize(im1);
		for (int i = 1; i < 5; i++) {
			drawEpipoleLine(im1, im2, F, e1, cvPoint2D32f(0, i * s.height / 5), color);
		}
	}

	void drawEpipoleLine(IplImage* im1, IplImage* im2, CvMat* F, CvMat* e1, CvPoint2D32f p1, CvScalar color) {
		CvMat* mx1 = CvMatUtil::hmatFromPoint2D(p1);
		CvMat* l1 = cvCreateMat(3, 1, CV_64FC1);
		cvCrossProduct(e1, mx1, l1);
		CvMat* l2 = CvMatUtil::matMul(F, mx1);

		drawLine(im1, l1, color);
		drawLine(im2, l2, color);
	}

	void drawLine(IplImage* im, CvMat* line, CvScalar color, int thickness) {
		double a = line->data.db[0];
		double b = line->data.db[1];
		double c = line->data.db[2];
		if (abs(a) <= abs(b)) {
			double x0 = 0;
			double x1 = im->width;
			double y0 = - (a * x0 + c) / b;
			double y1 = - (a * x1 + c) / b;
			cvLine(im, cvPoint((int)x0, (int)y0), cvPoint((int)x1, (int)y1), color, thickness);
		} else {
			double y0 = 0;
			double y1 = im->height;
			double x0 = - (b * y0 + c) / a;
			double x1 = - (b * y1 + c) / a;
			cvLine(im, cvPoint((int)x0, (int)y0), cvPoint((int)x1, (int)y1), color, thickness);
		}
	}

	void drawLines(IplImage* im, vector<CvMat*> lines, CvScalar color, int thickness) {
		for (int i = 0; i < lines.size(); i++) {
			drawLine(im, lines[i], color, thickness);
		}
	}

	}
}
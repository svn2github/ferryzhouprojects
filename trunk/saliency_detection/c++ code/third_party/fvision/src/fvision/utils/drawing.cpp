#include "drawing.h"

#include "cvmat_utils.h"
#include "geom.h"
#include "misc.h"

#include "drawing_PointDrawer.h"
#include "drawing_LineDrawer.h"
#include "drawing_TextDrawer.h"


namespace fvision {


IplImage* perspectiveWarp(const IplImage* src, const CvMat* H) {
	IplImage* dst = cvCloneImage(src);
	cvWarpPerspective(src, dst, H);
	return dst;
}

IplImage* createSideBySideImage(const IplImage* im_L, const IplImage* im_R) {
	IplImage* L_R = cvCreateImage(cvSize(im_L->width * 2, im_L->height), im_L->depth, im_L->nChannels);
	cvSetImageROI(L_R, cvRect(0, 0, im_L->width, im_L->height));
	cvCopy(im_L, L_R);
	cvSetImageROI(L_R, cvRect(im_L->width, 0, im_L->width, im_L->height));
	cvCopy(im_R, L_R);
	cvSetImageROI(L_R, cvRect(0, 0, im_L->width * 2, im_L->height));
	return L_R;
}

IplImage* createVerticalSideBySideImage(const IplImage* im_L, const IplImage* im_R) {
	IplImage* L_R = cvCreateImage(cvSize(im_L->width, im_L->height * 2), im_L->depth, im_L->nChannels);
	cvSetImageROI(L_R, cvRect(0, 0, im_L->width, im_L->height));
	cvCopy(im_L, L_R);
	cvSetImageROI(L_R, cvRect(0, im_L->height, im_L->width, im_L->height));
	cvCopy(im_R, L_R);
	cvSetImageROI(L_R, cvRect(0, 0, im_L->width * 2, im_L->height));
	return L_R;
}

IplImage* mix(const IplImage* im_L, const IplImage* im_R) {
	IplImage* mixim = cvCloneImage(im_L);
	cvAddWeighted(im_L, 0.5, im_R, 0.5, 0, mixim);
	return mixim;
}

void showImage(const char* winname, const IplImage* im) {
	cvNamedWindow(winname, 1);
	cvShowImage(winname, im);
}

void mixToRedCyan(const IplImage* im_L, const IplImage* im_R, IplImage* dstRedCyan) {
	CvSize size = cvGetSize(im_L);
	IplImage* L1 = cvCreateImage(size, im_L->depth, 1);
	IplImage* L2 = cvCreateImage(size, im_L->depth, 1);
	IplImage* L3 = cvCreateImage(size, im_L->depth, 1);
	//IplImage* L4 = cvCreateImage(size, im_L->depth, 1);
	IplImage* R1 = cvCreateImage(size, im_L->depth, 1);
	IplImage* R2 = cvCreateImage(size, im_L->depth, 1);
	IplImage* R3 = cvCreateImage(size, im_L->depth, 1);
	//IplImage* R4 = cvCreateImage(size, im_L->depth, 1);
	cvSplit(im_L, L1, L2, L3, NULL);
	cvSplit(im_R, R1, R2, R3, NULL);
	//cvMerge(L1, L2, R3, NULL, mixim);
	cvMerge(R1, R2, L3, NULL, dstRedCyan);
	cvReleaseImage(&L1);
	cvReleaseImage(&L2);
	cvReleaseImage(&L3);
	cvReleaseImage(&R1);
	cvReleaseImage(&R2);
	cvReleaseImage(&R3);
}

IplImage* mixToRedCyan(const IplImage* im_L, const IplImage* im_R) {
	IplImage* dstRedCyan = cvCloneImage(im_L);
	mixToRedCyan(im_L, im_R, dstRedCyan);
	return dstRedCyan;
}

void drawPoints(IplImage* src, const vector<CvPoint>& pts, const PointDrawer& pd) {
	for (int i = 0; i < (int)pts.size(); i++) {
		pd.draw(src, pts[i]);
	}
}

void drawPoints(IplImage* src, const vector<CvPoint2D32f>& pts, const PointDrawer& pd) {
	for (int i = 0; i < (int)pts.size(); i++) {
		pd.draw(src, pts[i]);
	}
}

void drawPolygon(IplImage* src, const vector<CvPoint2D32f>& pts, const LineDrawer& ld) {
	if (pts.size() == 0) return;
	for (size_t i = 0; i < pts.size() - 1; i++) {
		ld.draw(src, pts[i], pts[i + 1]);
	}
	ld.draw(src, pts[pts.size() - 1], pts[0]);
}

void drawLine(IplImage* im, CvMat* line, const LineDrawer& ld) {
	CvPoint2D32f p1, p2;
	computeLineInBoundary(line, cvGetSize(im), p1, p2);
	ld.draw(im, p1, p2);
}

void drawImagedXAxis(IplImage* im, const LineDrawer& ld) {
	ld.draw(im, cvPoint(0, im->height / 2), cvPoint(im->width, im->height / 2));
}

void drawImagedYAxis(IplImage* im, const LineDrawer& ld) {
	ld.draw(im, cvPoint(im->width / 2, 0), cvPoint(im->width / 2, im->height));
}

void drawLines(IplImage* im, const vector<CvMat*>& lines, const LineDrawer& ld) {
	for (size_t i = 0; i < lines.size(); i++) {
		drawLine(im, lines[i], ld);
	}
}

void drawPointsWithId(IplImage* src, 
					  const std::vector<CvPoint2D32f>& pts, 
					  const std::vector<int>& ids,
					  const PointDrawer& pd,
					  const TextDrawer& td) 
{
	drawPointsWithId(src, cvPointsFrom32fs(pts), ids, pd, td);
}

void drawPointsWithId(IplImage* src, 
					  const std::vector<CvPoint>& pts,
					  const std::vector<int>& ids, 
					  const PointDrawer& pd, 
					  const TextDrawer& td) 
{
	assert(pts.size() == ids.size());
	char numberText[20];
	CvPoint offset = cvPoint(5, 5);
	for (size_t i = 0; i < pts.size(); i++) {
		pd.draw(src, pts[i]);
		sprintf_s(numberText, 20, "%d", ids[i]);
		td.draw(src, pts[i] + offset, numberText);
	}
}

void drawPointWithId(IplImage* src, 
					 const CvPoint& pt, 
					 int id,
					 const PointDrawer& pd,
					 const TextDrawer& td) 
{
	char numberText[20];
	CvPoint offset = cvPoint(5, 5);
	pd.draw(src, pt);
	sprintf_s(numberText, 20, "%d", id);
	td.draw(src, pt + offset, numberText);
}


void drawPointWithId(IplImage* src, 
					 const CvPoint2D32f& pt, 
					 int id,
					 const PointDrawer& pd,
					 const TextDrawer& td) 
{
	drawPointWithId(src, cvPointFrom32f(pt), id, pd, td);
}

}
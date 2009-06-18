#pragma once

#include "PointTransformer.h"

namespace fvision {

class PointDrawer;
class LineDrawer;
class TextDrawer;

/**
 *	draw objects in normalized coordinates
 */
class DemoCanvas {
public:
	DemoCanvas(void);
	~DemoCanvas(void);

public:
	void setBackgroundColor(const CvScalar& color);

	void setUp(const CvSize& imSize, float scale);
	void clean();
	const IplImage* getImage() const { return im; }

public:
	void drawPoint(const CvPoint2D32f& p, const PointDrawer& pd);
	void drawLine(const CvPoint2D32f& p1, const CvPoint2D32f& p2, const LineDrawer& ld);
	void drawPose(const CvPoint2D32f& camPosition, double camAngle, const PointDrawer& pd, const LineDrawer& ld, float arrowLength = 0.1f);
	void drawLine(const CvMat* line, const LineDrawer& ld);
	void drawText(const CvPoint2D32f& position, const CvPoint2D32f& imOffset, const TextDrawer& td, const std::string& text);

public:
	void drawPoints(const std::vector<CvPoint2D32f>& points, const PointDrawer& pd);

private:
	CvPoint2D32f transformPoint(const CvPoint2D32f p);

private:
	CvSize imSize;
	CvPoint2D32f imCenter;
	float scale;
	CvMat* K;
	CvMat* lineHomography;
	CvMat* lineBuffer;
	PointTransformer* pointTransformer;


	IplImage* im;

	CvScalar bgColor;
};

}
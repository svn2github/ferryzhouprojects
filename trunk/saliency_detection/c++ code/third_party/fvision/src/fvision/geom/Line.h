#pragma once

#include <cv.h>
#include <iostream>

namespace fvision {

/** Line (Segment)
 * 
 * Two different internal presentations: <rho, theta> and <pt1, pt2>
 * 
 */
class Line {
public:
	/** Init with rho and theta, need to compute two end points intersect at the image boundary
	 *
	 */
	//Line(double rho, double theta, const CvSize& imSize);

	/** Init with two end points, need to compute rho and theta
	 *
	 */
	Line(const CvPoint2D32f& pt1, const CvPoint2D32f& pt2);

	Line(const CvPoint& pt1, const CvPoint& pt2);

	Line(float x1, float y1, float x2, float y2);

	Line();

	virtual ~Line(void);

	Line(const Line& rhs);
	Line& operator= (const Line& rhs);

public:
	//double getTheta() const { return theta; }

	CvPoint2D32f getPoint1() const { return p1; }
	CvPoint2D32f getPoint2() const { return p2; }

	const CvMat* getMat() const { return mat; }

	CvMat* getMat() { return mat;}

	void setPoints(const CvPoint2D32f& pt1, const CvPoint2D32f& pt2);

	// return the angle between y axis and the line
	// return a value between [-pi / 2, pi / 2]
	double getYAngle() const;

	double getLength() const;

private:
	void initWithPoints(const CvPoint2D32f& pt1, const CvPoint2D32f& pt2);
	void updateMat();

private:
	//double rho;
	//double theta;

	//end points
	CvPoint2D32f p1;
	CvPoint2D32f p2;

	CvMat* mat;
};

std::ostream& operator<< (std::ostream& os, const Line& line);
std::istream& operator>> (std::istream& is, Line& line);
}

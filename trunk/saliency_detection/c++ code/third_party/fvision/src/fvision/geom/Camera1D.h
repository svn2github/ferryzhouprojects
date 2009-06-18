#pragma once

#include <cv.h>
#include <iostream>
#include <vector>

namespace fvision {

/** Internal paramters of 1D camera
 * 
 * 1. focal length
 * 2. principal point; assume lies in the center of the image
 *
 * K = [f, px;
 *      0,  1]
 *
 * ix = x * f / y + px
 *
 */
class Camera1DInternal {

public:
	Camera1DInternal() {}

	Camera1DInternal(double focalLength, double imageWidth);

	//default destructor
	//default copy constructor and copy assignement

	static Camera1DInternal createWithFocalLengthAndImageWidth(double focalLength, double imageWidth);
	static Camera1DInternal createWithFocalLengthAndPrincipalPoint(double focalLength, double principalPoint);

public:
	double calculateImagePoint(const CvPoint2D32f& p) const {
		return p.x * focalLength / p.y + px;
	}

	double operator() (const CvPoint2D32f& p) const {
		return calculateImagePoint(p);
	}

	/**
	 * if the image point is outside the image boundary, return false
	 */
	bool isPoint1DInImage(double x) const {
		if (x >= 0 && x < imageWidth) return true;
		else return false;
	}

public: //GETTERS
	double getFov() const { return fov; }
	double getFocalLength() const { return focalLength; }
	double getImageWidth() const { return imageWidth; }
	double getPrincipalPoint() const { return px; }

private:
	double focalLength;
	double imageWidth;
	double px;         //principal point
	double fov;        //field of view
};

/** The pose of the 1D camera
 *  
 * 1. center
 * 2. orientation
 *
 * R = [cos(theta), -sin(theta);
 *      sin(theta,   cos(theta)]
 *
 * clockwise rotation of camera
 * counter clockwise rotation of points
 *
 * if theta = 0, the princpal axis is the y axis
 *
 * p' = R * (p - c)
 *
 */
class Camera1DExternal {
public:
	Camera1DExternal();

	Camera1DExternal(const CvPoint2D32f& c, double theta) {
		this->c     = c;
		this->theta = theta;
	}

public:
	CvPoint2D32f transformPoint(const CvPoint2D32f& p) const ;

	CvPoint2D32f operator() (const CvPoint2D32f& p) const {
		return transformPoint(p);
	}

	double project(const CvPoint2D32f& p) const {
		CvPoint2D32f tp = transformPoint(p);
		return tp.x / tp.y;
	}

	double getAngle() const { return theta; }

	CvPoint2D32f getPosition() const { return c; }

	void setAngle(double angle) { this->theta = angle; }

	void setPosition(const CvPoint2D32f& position) { this->c = position; }

private:
	CvPoint2D32f c;  //center
	double theta;    //orientation
};

/**
 * 1D Camera
 * 
 * center:       x, y
 * orientation:  theta
 *
 */
class Camera1D {
public:
	//default 
	//
	Camera1D();

public:
	Camera1D(double focalLength, double imageWidth, const CvPoint2D32f& c, double theta);
	Camera1D(const Camera1DInternal& camInternal, const Camera1DExternal& camExternal);

	static Camera1D create(double focalLength, double principalPoint, const CvPoint2D32f& camPosition, double camAngle);

public:
	~Camera1D(void);

	//default destructor
	//default copy constructor and copy assignement

public:

	double project(const CvPoint2D32f& p) const {
		return calculateImagePoint(p);
	}

	std::vector<double> project(const std::vector<CvPoint2D32f>& ps) const ;

	double calculateImagePoint(const CvPoint2D32f& p) const {
		return cameraInternal.calculateImagePoint(cameraExternal.transformPoint(p));
	}

	double operator() (const CvPoint2D32f& p) const {
		return calculateImagePoint(p);
	}

	bool isPoint1DInImage(double x) const {
		return cameraInternal.isPoint1DInImage(x);
	}

	std::vector<double> calculateImagePoints(const std::vector<CvPoint2D32f>& pts) const ;

	/**
	 * only return the image points that are inside the image, i.e., in [0, imageWidth)
	 */
	std::vector<double> calculateImagePointsWithinImage(const std::vector<CvPoint2D32f>& pts, std::vector<int>& is) const ;

public:
	Camera1DInternal getInternal() const { return cameraInternal; }
	Camera1DExternal getExternal() const { return cameraExternal; }

	double getFocalLength() const { return cameraInternal.getFocalLength(); }
	double getPrincipalPoint() const { return cameraInternal.getPrincipalPoint(); }

	friend std::ostream& operator<<(std::ostream& os, const Camera1D& camera);

private:
	Camera1DExternal cameraExternal;
	Camera1DInternal cameraInternal;
};

std::ostream& operator<<(std::ostream& os, const Camera1DInternal& cameraInternal);
std::ostream& operator<<(std::ostream& os, const Camera1DExternal& cameraExternal);
std::ostream& operator<<(std::ostream& os, const Camera1D& camera);

std::istream& operator>>(std::istream& is, Camera1DExternal& cameraExternal);

}
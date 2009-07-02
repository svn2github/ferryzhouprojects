#pragma once

#include <cv.h>

class OpticalFlowParticle {
public:
	OpticalFlowParticle(int frameId, const CvPoint2D32f& position, const CvPoint2D32f& velocity);

public:
	double getAngle() const { return 0; }
	double getSpeed() const { return 0; }

	CvPoint2D32f getPosition() const { return position; }

	double getVx() const { return velocity.x; }
	double getVy() const { return velocity.y; }

private:
	CvPoint2D32f position;
	CvPoint2D32f velocity; //in <x, y>
	int frameId;
};

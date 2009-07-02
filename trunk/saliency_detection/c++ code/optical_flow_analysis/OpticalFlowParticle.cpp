#include "OpticalFlowParticle.h"


OpticalFlowParticle::OpticalFlowParticle(int frameId, const CvPoint2D32f& position, const CvPoint2D32f& velocity) {
	this->frameId = frameId;
	this->position = position;
	this->velocity = velocity;
}
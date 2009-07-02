#include "OpticalFlowParticles.h"

OpticalFlowParticles::OpticalFlowParticles(void)
{
}

OpticalFlowParticles::~OpticalFlowParticles(void)
{
}

void OpticalFlowParticles::addParticle(const OpticalFlowParticle& particle) {
	particles.push_back(particle);
}
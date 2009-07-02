#pragma once

#include "OpticalFlowParticle.h"

#include <list>

class OpticalFlowParticles {
public:
	OpticalFlowParticles(void);
	~OpticalFlowParticles(void);

	typedef std::list<OpticalFlowParticle>::iterator ParticleIterator;
	typedef std::list<OpticalFlowParticle>::const_iterator ConstParticleIterator;

public:
	ConstParticleIterator begin() const { return particles.begin(); }
	ConstParticleIterator end() const { return particles.end(); }

	size_t size() const { return particles.size(); }

	void addParticle(const OpticalFlowParticle& particle);

private:
	std::list<OpticalFlowParticle> particles;
};

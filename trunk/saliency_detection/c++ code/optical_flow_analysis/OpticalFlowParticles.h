#pragma once

#include "OpticalFlowParticle"
#include <list>

class OpticalFlowParticles {
public:
	OpticalFlowParticles(void);
	~OpticalFlowParticles(void);

	typedef std::list<OpticalFlowParticle>::iterator ParticleIterator;
	typedef std::list<OpticalFlowParticle>::const_iterator ConstParticleIterator;

public:
	ConstParticleIterator begin() const { return particles.begin();}
	ConstParticleIterator end() const { return particles.end(); }

private:
	std::list<OpticalFlowParticle> particles;
};

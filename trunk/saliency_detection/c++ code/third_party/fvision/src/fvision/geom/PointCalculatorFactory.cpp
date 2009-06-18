#include "PointCalculatorFactory.h"

#include "PointCalculatorImpl.h"

namespace fvision {

PointCalculator* PointCalculatorFactory::create(const Configuration& configuration) {

	if (configuration.ransac) return RANSAC_PointCalculator::create(configuration.ransacErrorThreshold);
	else return new DltPointCalculator();
}

}
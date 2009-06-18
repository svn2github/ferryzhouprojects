#ifndef FVISION_POINT_CALCULATOR_FACTORY_H_
#define FVISION_POINT_CALCULATOR_FACTORY_H_

#include "PointCalculator.h"

namespace fvision {

class PointCalculatorFactory {
public:
	class Configuration {
	public:
		Configuration() {
			ransac = true;
			ransacErrorThreshold = 1.5; //pixels
		}

	public:
		bool ransac;
		double ransacErrorThreshold;
	};

public:
	PointCalculator* create(const Configuration& configuration);
};

}
#endif // FVISION_POINT_CALCULATOR_FACTORY_H_
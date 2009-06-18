#include "TwoViewPointTrackerFactory.h"

#include "impl/corner_detectors/OpenCVCornerDetector.h"
#include "impl/PyrLKTwoViewPointTracker.h"
#include "impl/SiftTwoViewPointTracker.h"
#include "impl/point_matchers/OpticalFlowPointMatcher.h"
#include "impl/corner_detectors/GridCornerDetector.h"
#include "impl/CornerPointMatchTwoViewPointTracker.h"

namespace fvision {

TwoViewPointTracker* TwoViewPointTrackerFactory::createPyrKLT(
	int     corners_count, 
	double  quality_level, 
	double  min_distance) {

	return new PyrLKTwoViewPointTracker(
		new OpenCVCornerDetector(corners_count, quality_level, min_distance));
}

TwoViewPointTracker* TwoViewPointTrackerFactory::createSIFT() {
	return new SiftTwoViewPointTracker();
}

TwoViewPointTracker* TwoViewPointTrackerFactory::createBlockTracker(int blockSize, 
																	int searchRange, 
																	int matchingWindowSize) 
{
	return new CornerPointMatchTwoViewPointTracker(
		new GridCornerDetector(blockSize), 
		new OpticalFlowPointMatcher(searchRange, matchingWindowSize / 2));
}

TwoViewPointTracker* TwoViewPointTrackerFactory::create(const Configuration& configuration) {
	switch (configuration.method) {
		case Configuration::PryKLT: return createPyrKLT((int)configuration.param1, configuration.param2, configuration.param3);
		case Configuration::SIFT:   return createSIFT();
		case Configuration::BLOCK:  return createBlockTracker((int)configuration.param1, (int)configuration.param2, (int)configuration.param3);
		default: return createSIFT();
	}
}
}
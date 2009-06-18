#include "PointTrackerFactory.h"

#include "impl/corner_detectors/OpenCVCornerDetector.h"
#include "impl/PyrLKPointTracker.h"
#include "impl/SiftPointTracker.h"

namespace fvision {

PointTrackerFactory::PointTrackerFactory(void)
{
}

PointTrackerFactory::~PointTrackerFactory(void)
{
}

PointTracker* PointTrackerFactory::create(const Configuration& configuration) {
	switch (configuration.method) {
		case Configuration::PryKLT: return createPyrKLT((int)configuration.param1, configuration.param2, configuration.param3, configuration.keyFrameInterval);
		case Configuration::SIFT: return createSift(configuration.param1, configuration.param2, configuration.keyFrameInterval);
		default: {
			return createPyrKLT(400, 0.01, 8, 3);
				 }
	}
}

PointTracker* PointTrackerFactory::createPyrKLT(int corners_count /*= 400*/, 
												double quality_level /*= 0.01*/, 
												double min_distance /*= 8*/,
												int     keyFrameInterval) 
{
	PyrLKPointTracker* tracker = new PyrLKPointTracker(new OpenCVCornerDetector(corners_count, quality_level, min_distance));
	tracker->setKeyFrameInterval(keyFrameInterval);
	tracker->setMinimalNeighborDistance(min_distance);
	return tracker;
}

PointTracker* PointTrackerFactory::createSift(double minimalRatioOfNeighborDistances, double keyFrameMinimalMatchRate, int keyFrameInterval) {
	SiftPointTracker* tracker = new SiftPointTracker();
	tracker->setMinimalRatioOfNeighborDistances(minimalRatioOfNeighborDistances);
	tracker->setKeyFrameMinimalMatchRate(keyFrameMinimalMatchRate);
	tracker->setKeyFrameInterval(keyFrameInterval);
	return tracker;
}
}
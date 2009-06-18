#ifndef FVISION_POINT_TRACKER_FACTORY_H_
#define FVISION_POINT_TRACKER_FACTORY_H_

#include "PointTracker.h"

namespace fvision {

class PointTrackerFactory {
public:
	PointTrackerFactory(void);
	~PointTrackerFactory(void);

	/**
	 *	for PryKLT method, param1 = cornersCount, params2 = confidence, params3 = minNeighborDistance
	 *  for SIFT method, param1 = minimalRatioOfNeighborDistances, param2 = keyFrameMinimalMatchRate
	 */
	class Configuration {
	public:
		Configuration() {
			setDefaultPryKLT();
		}

		void setDefaultPryKLT() {
			method = PryKLT;
			param1 = 400;
			param2 = 0.01;
			param3 = 8;
			keyFrameInterval = 3;
		}

		void setDefaultSift() {
			method = SIFT;
			param1 = 0.49;
			param2 = 0.4;
			keyFrameInterval = 3;
		}

	public:
		enum Method {
			PryKLT,
			SIFT
		};
	public:
		Method method;
		double param1;
		double param2;
		double param3;

		int keyFrameInterval;
	};

	PointTracker* create(const Configuration& configuration);

public:
	PointTracker* createPyrKLT(
		int     corners_count = 400, 
		double  quality_level = 0.01, 
		double  min_distance  = 8,
		int     keyFrameInterval = 3);

	PointTracker* createSift(
		double minimalRatioOfNeighborDistances,
		double keyFrameMinimalMatchRate,
		int    keyFrameInterval);
};

}
#endif // FVISION_POINT_TRACKER_FACTORY_H_
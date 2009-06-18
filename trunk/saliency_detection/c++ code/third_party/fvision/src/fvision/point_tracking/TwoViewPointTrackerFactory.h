#pragma once

#include "impl/PyrLKTwoViewPointTracker.h"
#include "impl/SiftTwoViewPointTracker.h"

#include "TwoViewPointTracker.h"

namespace fvision {

/** Factory for creating two-view-point-tracker such as SIFT and KLT
 *
 */
class TwoViewPointTrackerFactory {
public:
	class Configuration {
	public:
		Configuration() {
			setDefaultSIFT();
		}

		void setDefaultSIFT() {
			method = SIFT;
		}

		void setDefaultPyrKLT() {
			method = PryKLT;
			param1 = 400;
			param2 = 0.01;
			param3 = 8;
		}

	public:
		enum Method {
			PryKLT,
			SIFT,
			BLOCK
		};
	public:
		Method method;
		double param1;
		double param2;
		double param3;
	};

	TwoViewPointTracker* create(const Configuration& configuration);

public:
	/**
	 *
	 */
	TwoViewPointTracker* createPyrKLT(
		int     corners_count = 400, 
		double  quality_level = 0.01, 
		double  min_distance  = 8);

	/**
	 */
	TwoViewPointTracker* createSIFT();

	TwoViewPointTracker* createBlockTracker (
		int blockSize,
		int searchRange,
		int matchingWindowSize);

};

}
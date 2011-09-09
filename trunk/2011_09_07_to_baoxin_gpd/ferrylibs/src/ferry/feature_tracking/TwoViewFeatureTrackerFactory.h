#pragma once

#include <iostream>

#include "TwoViewFeatureTracker.h"

using namespace std;

namespace ferry {
	namespace feature_tracking {

class TrackerParam {
public:
	friend ostream& operator<<(ostream& os, const TrackerParam& tp) {
		os<<tp.corner_count<<" ";
		os<<tp.quality_level<<" ";
		os<<tp.points_min_distance<<" ";
		os<<tp.max_search_range<<" ";
		os<<tp.half_win_size<<" ";
		os<<tp.half_adjust_range<<" ";
		os<<tp.max_difference<<" ";
		return os;
	}

	friend istream& operator>>(istream& is, TrackerParam& tp) {
		is>>tp.corner_count;
		is>>tp.quality_level;
		is>>tp.points_min_distance;
		is>>tp.max_search_range;
		is>>tp.half_win_size;
		is>>tp.half_adjust_range;
		is>>tp.max_difference;
		return is;
	}

public:
	int corner_count;
	double quality_level;
	double points_min_distance;
	double max_search_range;
	int half_win_size;
	int half_adjust_range;
	double max_difference;
};

class TwoViewFeatureTrackerFactory
{
public:
	TwoViewFeatureTrackerFactory(void) {}
	~TwoViewFeatureTrackerFactory(void) {}

public:
	//default configuration
	//OpenCV, SimpleWindow
	static TwoViewFeatureTracker* create(TrackerParam tp) {
		CornerDetector* pcd = new OpenCVCornerDetector(tp.corner_count, tp.quality_level, tp.points_min_distance);
		PointsOutFeatureMatcher* ppofm = new SimpleWindowPOFM(new SimpleWindowSSDFDC(tp.half_win_size, tp.half_adjust_range, (int)tp.max_difference), tp.max_search_range);

		return new TwoViewFeatureTracker(pcd, ppofm);
	}
};
	}
}
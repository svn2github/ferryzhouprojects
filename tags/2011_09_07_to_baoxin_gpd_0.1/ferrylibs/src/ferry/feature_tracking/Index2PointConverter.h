#pragma once

namespace ferry {
	namespace feature_tracking {

class Index2PointConverter
{
public:
	static void convert(const vector<CvPoint>& corners1, const vector<CvPoint>& corners2, int* co1, int* co2,vector<CvPoint>& x1s, vector<CvPoint>& x2s) {
		for (int i = 0; i < (int)corners1.size(); i++) {
			if (co1[i] != -1) {
				x1s.push_back(corners1[i]);
				x2s.push_back(corners2[co1[i]]);
			}
		}
	}

};

	}
}
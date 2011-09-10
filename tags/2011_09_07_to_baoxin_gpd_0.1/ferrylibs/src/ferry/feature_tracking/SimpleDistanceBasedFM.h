#pragma once

#include <cmath>

#include "FeatureDistanceCalculator.h"
#include "FeatureMatcher.h"

namespace ferry {
	namespace feature_tracking {

class SimpleDistanceBasedFM : public FeatureMatcher
{
public:
	SimpleDistanceBasedFM(FeatureDistanceCalculator* pfdc, float maxDisparity = 50) {
		this->pfdc = pfdc;
		this->maxDisparity = maxDisparity;
	}

	SimpleDistanceBasedFM(float maxDisparity = 50) {
		this->maxDisparity = maxDisparity;
	}

	void setFeatureDistanceCalculator(FeatureDistanceCalculator* pfdc) {
		this->pfdc = pfdc;
	}

public:
	void compute(IplImage* im1, IplImage* im2, const vector<CvPoint>& corners1, const vector<CvPoint>& corners2, int* correspondences1, int* correspondences2) {
		computeDistanceMatrix(im1, im2, corners1, corners2);
		//IOUtil::savePoints("temp/corners1.txt", corners1);
		//IOUtil::savePoints("temp/corners2.txt", corners2);
		computeCorrespondences();
		getCorrespondences(correspondences1, correspondences2);
	}

	void computeDistanceMatrix(IplImage* im1, IplImage* im2, const vector<CvPoint>& corners1, const vector<CvPoint>& corners2) {
		count1 = (int)corners1.size();
		count2 = (int)corners2.size();
		for (int i = 0; i < count2; i++) {
			for (int j = 0; j < count1; j++) {
				if (calcDistance(corners1[j], corners2[i]) > maxDisparity) distances[i][j] = MAX_DISTANCE;
				else distances[i][j] = pfdc->compute(im1, im2, corners1[j], corners2[i]);
			}
		}
	}

	void getCorrespondences(int* correspondences1, int* correspondences2) {
		for (int i = 0; i < count1; i++) {
			correspondences1[i] = this->correspondences1[i];
		}
		for (int i = 0; i < count2; i++) {
			correspondences2[i] = this->correspondences2[i];
		}
	}

	void computeCorrespondences() {
		int co1[MAX_FEATURE_NUM];
		int co2[MAX_FEATURE_NUM];
		int i = 0;
		for (i = 0; i < count2; i++) {
			float mind = 100000;
			int index = 0;
			for (int j = 0; j < count1; j++) {
				if (mind > distances[i][j]) {
					mind = distances[i][j];
					index = j;
				}
			}
			co2[i] = index;
		}
		for (i = 0; i < count1; i++) {
			float mind = 100000;
			int index = 0;
			for (int j = 0; j < count2; j++) {
				if (mind > distances[j][i]) {
					mind = distances[j][i];
					index = j;
				}
			}
			co1[i] = index;
		}
		for (int i = 0; i < count1; i++) {
			correspondences1[i] = -1;
		}
		for (i = 0; i < count2; i++) {
			correspondences2[i] = -1;
		}
		for (i = 0; i < count1; i++) {
			if (co2[co1[i]] == i && distances[co1[i]][i] != MAX_DISTANCE) {
				//cout<<"i: "<<i<<" col[i]: "<<co1[i]<<" co2[co1[i]]: "<<co2[co1[i]]<<endl;
				correspondences1[i] = co1[i];
				correspondences2[co1[i]] = i;
				//cout<<"distances: "<<distances[co1[i]][i]<<endl;
			}
		}
	}

private:
	float calcDistance(const CvPoint& p1, const CvPoint& p2) {
		int dx = p1.x - p2.x;
		int dy = p1.y - p2.y;
		return sqrt((float)(dx * dx + dy * dy));
	}

private:
	FeatureDistanceCalculator* pfdc;
protected:
	float maxDisparity;

	float distances[MAX_FEATURE_NUM][MAX_FEATURE_NUM];
	int count1;
	int count2;
	int correspondences1[MAX_FEATURE_NUM];
	int correspondences2[MAX_FEATURE_NUM];

private:

};

	}
}
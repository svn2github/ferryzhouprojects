#pragma once

#include <cv.h>

#include <vector>
#include <set>
#include <map>
#include <algorithm>

/*!
misc functions

*/
namespace fvision {

	std::vector<CvPoint> cvPointsFrom32fs(const std::vector<CvPoint2D32f>& pts);
	std::vector<CvPoint2D32f> cvPointsTo32fs(const std::vector<CvPoint>& pts);

	CvPoint3D32f cvPoint3D32fFrom64f(const CvPoint3D64f& p);
	CvPoint3D64f cvPoint3D32fTo64f(const CvPoint3D32f& p);

	CvPoint2D32f cvPoint2D32fFrom64f(const CvPoint2D64f& p);

	std::vector<CvPoint3D32f> cvPoint3D32fsFrom64fs(const std::vector<CvPoint3D64f>& pts);
	std::vector<CvPoint3D64f> cvPoint3D32fsTo64fs(const std::vector<CvPoint3D32f>& pts);

	/** get disparities, x2s[i].x - x1s[i].x
	*/
	std::vector<double> getDisparities(const std::vector<CvPoint2D32f>& x1s, const std::vector<CvPoint2D32f>& x2s);

	//randomly sample N point pairs
	void randomlySampleNPointPairs(const std::vector<CvPoint2D32f>& x1s, 
		                           const std::vector<CvPoint2D32f>& x2s, 
								   std::vector<CvPoint2D32f>& sx1s, 
								   std::vector<CvPoint2D32f>& sx2s, 
								   int n);

	std::vector<CvPoint2D32f> addNoiseToPoints(float standardDeviation, std::vector<CvPoint2D32f>& pts);
	CvPoint2D32f addNoiseToPoint(float standardDeviation, const CvPoint2D32f& pt);

	std::vector<double> addNoiseToPoints(float standardDeviation, std::vector<double>& pts);

	class PointNoiser {
	public:
		PointNoiser(double standardDeviation) {
			this->stdDev = standardDeviation;
		}

		CvPoint2D32f operator() (const CvPoint2D32f& p) {
			return addNoiseToPoint((float)stdDev, p);
		}

	private:
		double stdDev;
	};

	std::vector<int> intersection(const std::vector<int>& v1, const std::vector<int>& v2);

	std::vector<int> intersection(const std::set<int>& v1, const std::set<int>& v2);

	/** v1 - v2; get elements in v1 but not in v2
	 */
	std::vector<int> subtraction(const std::vector<int>& v1, const std::vector<int>& v2);

	/** Given a set of error values and a threshold, filter inliers and outliers
	 */
	void getInlierOutlierIndices(const std::vector<double>& errors, double threshold, std::vector<int>& inlierIndices, std::vector<int>& outlierIndices);

	//[from, to)
	std::vector<int> createIntVector(int from, int to, int step = 1);
}

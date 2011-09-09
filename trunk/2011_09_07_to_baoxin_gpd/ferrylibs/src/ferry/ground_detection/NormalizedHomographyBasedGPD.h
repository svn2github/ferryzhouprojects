#pragma once

#include <algorithm>

#include "cv.h"

#include <ferry/cv_geometry/CvMatUtil.h>
#include <ferry/cv_geometry/TwoViewGeometryUtil.h>
#include <ferry/cv_geometry/RANSAC.h>
#include <ferry/cv_geometry/DLT_HC.h>

using namespace std;

using namespace ferry::cv_mat;
using namespace ferry::cv_geometry;
using namespace ferry::math;

class NormalizedHomographyBasedGPD
{
public:
	NormalizedHomographyBasedGPD(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, CvMat* M1, CvMat* M2, double threshold) {
		this->x1s = x1s;
		this->x2s = x2s;
		this->M1 = M1;
		this->M2 = M2;
		this->threshold = threshold;
		this->NH = NULL;
	}
	~NormalizedHomographyBasedGPD(void) {}

public:
	void compute() {
		normalizePoints();
		NH_RANSAC();
	}

	CvMat* getNH() {
		return NH;
	}

	vector<int> getis() {
		return is;
	}

	vector<int> getos() {
		return os;
	}

private:
	void normalizePoints() {
		CvMat* invM1 = inv(M1);
		CvMat* invM2 = inv(M2);
		nx1s = transformPoints(invM1, x1s);
		nx2s = transformPoints(invM2, x2s);

		cvReleaseMat(&invM1);
		cvReleaseMat(&invM2);
	}

	void NH_RANSAC() {
		int N = 1000;
		int sampleCount = 0;
		int pointsNumber = (int)nx1s.size();
		int modelSampleSize = 5;
		RANSAC ransac(modelSampleSize);
		
		CvMat* bestNH = NULL;
		int maxInliersNumber = 0;
		
		while ((sampleCount++) < N) {
			cout<<"iteration: "<<sampleCount<<endl;
			//select a random sample -- 8 point correspondences
			vector<CvPoint2D32f> sx1s, sx2s;
			//chooseSamples(sx1s, sx2s, modelSampleSize);
			randomlySampleNPointPairs(nx1s, nx2s, sx1s, sx2s, modelSampleSize);

			CvMat* gH = NULL;
			int inliersNumber = repeatedComputeH(sx1s, sx2s, &gH);
			if (inliersNumber == 0) continue;

			if (inliersNumber > maxInliersNumber) {
				bestNH = cloneMat(gH);
				maxInliersNumber = inliersNumber;
				N = ransac.calcN(inliersNumber, pointsNumber);
			}

			cout<<"N: "<<N<<endl;
		}

		cout<<"sampleCount: "<<sampleCount<<endl;

		if (bestNH == NULL) return;

		HomographyInlierOutlierModel iom(bestNH, threshold);
		iom.classifyToIndices(nx1s, nx2s, is, os);
		cout<<"inliers num: "<<(int)is.size()<<endl;
		NH = bestNH;

		//model -> primitive H
/*		vector<CvPoint2D32f> ix1s, ix2s, ox1s, ox2s;
		iom.classify(nx1s, nx2s, ix1s, ix2s, ox1s, ox2s);
		for (int i = 0; i < ix1s.size(); i++) {
			cout<<"y: "<<ix1s[i].y<<endl;
			if (ix1s[i].y < 0) {
				ix1s.erase(ix1s.begin() + i);
				ix2s.erase(ix2s.begin() + i);
				i--;
			}
		}
		Homography sH = hc.compute(ix1s, ix2s);
		HomographyInlierOutlierModel iom2(sH.getMat(), threshold/4 );
		is.clear();os.clear();
		iom2.classifyToIndices(nx1s, nx2s, is, os);
		cout<<"inliers num: "<<(int)is.size()<<endl;
		NH = cloneMat(sH.getMat());
*/
/*		CvMat* gH = fitModel(sH.getMat());
		HomographyInlierOutlierModel iom3(gH, threshold);
		is.clear();os.clear();
		iom3.classifyToIndices(nx1s, nx2s, is, os);
		cout<<"inliers num: "<<(int)is.size()<<endl;
		NH = gH;
*/	}

	void chooseSamples(vector<CvPoint2D32f>& sx1s, vector<CvPoint2D32f>& sx2s, int N) {
		int i0 = rani((int)nx1s.size());
		cout<<"i0: "<<i0<<endl;
		vector<int> ns = chooseClosestPoints(i0, N);
		for (int i = 0; i < ns.size(); i++) {
			cout<<ns[i]<<", ";
			sx1s.push_back(nx1s[ns[i]]);
			sx2s.push_back(nx2s[ns[i]]);
		}
		cout<<endl;
	}

	vector<int> chooseClosestPoints(int pi, int num) {
		vector<int> closestPoints;
		vector<double> distances;
		vector<int> distIndices;
		for (int i = 0; i < nx1s.size(); i++) {
			double dist = pointDistance(nx1s[pi], nx1s[i]);
			bool inserted = false;
			for (int j = 0; j < distances.size(); j++ ){
				if (dist < distances[j]) {
					distances.insert(distances.begin() + j, dist);
					distIndices.insert(distIndices.begin() + j, i);
					inserted = true;
					break;
				}
			}
			if ( !inserted ) {
				distances.push_back(dist);
				distIndices.push_back(i);
			}
		}
		//cout<<"dist Indices: "<<endl;
		//printIndices(distIndices);
		//sort(distances.begin(), distances.end());
		for (int i = 0; i < num; i++) {
			closestPoints.push_back(distIndices[i]);
		}
		return closestPoints;
	}

public:
	int repeatedComputeH(const vector<CvPoint2D32f>& sx1s, const vector<CvPoint2D32f>& sx2s, CvMat** pH) {
		CvMat* oldgH;
		int oldInliersNumber = 0;
		vector<CvPoint2D32f> tx1s = sx1s;
		vector<CvPoint2D32f> tx2s = sx2s;
		for (;;) {
			if (tx1s.size() < 5) return 0;
			Homography sH = hc.compute(tx1s, tx2s);
			//cout<<"sF: "<<sF<<endl;

			CvMat* gH = fitModel(sH.getMat());

			if (gH == NULL) return 0;

			HomographyInlierOutlierModel iom(gH, threshold);
			vector<CvPoint2D32f> ix1s, ix2s, ox1s, ox2s;
			iom.classify(nx1s, nx2s, ix1s, ix2s, ox1s, ox2s);
			int inliersNumber = ix1s.size();
			cout<<"inliers number: "<<inliersNumber<<endl;
			
			if (inliersNumber < 4) return 0; 
			vector<int> is, os;
			iom.classifyToIndices(nx1s, nx2s, is, os);
			for (int i = 0; i < is.size(); i++) {
				cout<<i<<", ";
			}
			cout<<endl;
			if (inliersNumber <= oldInliersNumber) {
				*pH = cloneMat(oldgH);
				return oldInliersNumber;
			} else {
				oldgH = gH;
				oldInliersNumber = inliersNumber;
				//append begin
				for (int i = 0; i < ix1s.size(); i++) {
					cout<<"y: "<<ix1s[i].y<<endl;
					if (ix1s[i].y < 0) {
						ix1s.erase(ix1s.begin() + i);
						ix2s.erase(ix2s.begin() + i);
						i--;
					}
				}
				//append end
				tx1s = ix1s;
				tx2s = ix2s;
			}
		}
	}

	CvMat* fitModel(CvMat* H) {
		CvMat* gH = cloneMat(H);
		cvScale(gH, gH, 1 / cvmGet(gH, 1, 1));
		cout<<"cand gH: "<<gH<<endl;
		const double MAX_ERROR = 0.1;
		double a10 = cvmGet(gH, 1, 0);
		double a12 = cvmGet(gH, 1, 2);
		if (abs(a10) > MAX_ERROR || abs(a12) > MAX_ERROR) return NULL;
		
		cvmSet(gH, 1, 0, 0);
		cvmSet(gH, 1, 2, 0);

		double a00 = cvmGet(gH, 0, 0);
		double a02 = cvmGet(gH, 0, 2);
		double a20 = cvmGet(gH, 2, 0);
		double a22 = cvmGet(gH, 2, 2);

		double theta = estimateTheta(a00, a02, a20, a22);
		double err = thetaError(theta, a00, a02, a20, a22);
		if (err > MAX_ERROR * MAX_ERROR) return NULL;

		cvmSet(gH, 0, 0, cos(theta));
		cvmSet(gH, 0, 2, sin(theta));
		cvmSet(gH, 2, 0, -sin(theta));
		cvmSet(gH, 2, 2, cos(theta));
		
		cout<<"theta: "<<theta<<endl;
		cout<<"cos(theta): "<<cos(theta)<<endl;
		cout<<"sin(theta): "<<sin(theta)<<endl;
		cout<<"error: "<<err<<endl;
		return gH;
	}

	double estimateTheta(double a00, double a02, double a20, double a22) {
		double cos1 = (a00 + a22) / 2;
		if (cos1 > 1) cos1 = 1;
		if (cos1 < -1) cos1 = -1;
		double theta1 = acos(cos1);
		double sin1 = (a02 - a20) / 2;
		if (sin1 > 1) sin1 = 1;
		if (sin1 < -1) sin1 = -1;
		double theta2 = asin(sin1);
/*		double theta;// = theta2;//(theta1 + theta2) / 2;
		if (theta1 * theta2 >= 0) theta = (theta1 + theta2) / 2;
		else theta = (theta2 - theta1) / 2;
*/
		//double theta = theta2;
		double minErrorTheta = theta2;
		double minError = thetaError(minErrorTheta, a00, a02, a20, a22);
		cout<<"initial error: "<<minError<<endl;
		cout<<"initial error theta: "<<minErrorTheta<<endl;
		for (int i = -10; i <= 10; i++) {
			double theta = minErrorTheta + i * 0.01;
			double err = thetaError(theta, a00, a02, a20, a22);
			if (err < minError) {
				minError = err;
				minErrorTheta = theta;
			}
		}
		cout<<"min error: "<<minError<<endl;
		cout<<"min error theta: "<<minErrorTheta<<endl;
		return minErrorTheta;
	}

	double thetaError(double theta, double a00, double a02, double a20, double a22) {
		return 2 + a00 * a00 + a02 * a02 + a20 * a20 + a22 * a22 - 2 * (a00 + a22) * cos(theta) - 2 * (a02 - a20) * sin(theta);
	}

private:
	//M1 = K1R1
	CvMat* M1;
	CvMat* M2;

	vector<CvPoint2D32f> x1s;
	vector<CvPoint2D32f> x2s;

	vector<CvPoint2D32f> nx1s;
	vector<CvPoint2D32f> nx2s;

	CvMat* NH;
	double threshold;
	vector<int> is;
	vector<int> os;

	DLT_HC hc;

};

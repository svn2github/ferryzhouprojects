#include "matching_utils.h"

using namespace std;

namespace fvision {

float windowMatch(const IplImage* im1, 
				  const IplImage* im2, 
				  const CvPoint& p1,
				  const CvPoint& p2, 
				  int halfWinSize, 
				  int type) 
{

	//if (type == 0) assert(im1->nChannels == 1);
	//else assert(im1->nChannels >= 3);

	uchar* data1 = (uchar*)im1->imageData;
	uchar* data2 = (uchar*)im2->imageData;
	CvSize size1 = cvGetSize(im1);
	CvSize size2 = cvGetSize(im2);
	float sum = 0.0;
	int num = 0;
	int nchannels = im1->nChannels;
	int nChannelUsed = 1;
	if (im1->nChannels != 1) { //not gray level
		if (type == 0) nChannelUsed = 1;
		else if (type == 1) nChannelUsed = 3;
		else nChannelUsed = 2;
	}
	for (int i = - halfWinSize; i <= halfWinSize; i++) {
		int x1 = p1.x + i;
		int x2 = p2.x + i;
		if (x1 < 0 || x1 >= size1.width) continue;
		if (x2 < 0 || x2 >= size2.width) continue;
		for (int j = - halfWinSize; j <= halfWinSize; j++) {
			int y1 = p1.y + j;
			int y2 = p2.y + j;
			if (y1 < 0 || y1 >= size1.height) continue;
			if (y2 < 0 || y2 >= size2.height) continue;
			for (int k = 0; k < nChannelUsed; k++) {
				float d = (float)(data1[y1 * im1->widthStep + x1 * nchannels + k] - data2[y2 * im2->widthStep + x2 * nchannels + k]);
				//sum += d * d;
				sum += abs(d);
				num++;
			}
		}
	}
	if (num != 0) return sum / num;
	else return 256; //MAX
}

float* windowNeighborMatch(const IplImage* im1, 
						   const IplImage* im2,
						   const CvPoint& p1, 
						   const CvPoint& p2, 
						   int halfWinSize, 
						   int halfNeighborWinSize, 
						   int type) 
{
	int length = 2 * halfNeighborWinSize + 1;
	float* dists = new float[length * length];
	for (int i = -halfNeighborWinSize; i <= halfNeighborWinSize; i++) {
		for (int j = -halfNeighborWinSize; j <= halfNeighborWinSize; j++) {
			dists[i * length + j] = windowMatch(im1, im2, p1, cvPoint(p2.x + j, p2.y + i), halfWinSize, type);
		}
	}
	return dists;
}

double pt_mu_pointDistance(const CvPoint& p1, const CvPoint& p2) {
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return sqrt((double)(dx * dx + dy * dy));			
}

vector<CvPoint> filterPointsByDistance(const CvPoint& p1, const std::vector<CvPoint>& ps, double maxDistance) {
	vector<CvPoint> ops;
	for (size_t i = 0; i < ps.size(); i++) {
		if (pt_mu_pointDistance(p1, ps[i]) <= maxDistance) ops.push_back(ps[i]);
	}
	return ops;
}

}
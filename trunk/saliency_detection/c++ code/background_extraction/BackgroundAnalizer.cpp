#include "BackgroundAnalizer.h"

#include "ImageBlockFeature.h"
#include "ImageBlockCluster.h"

#include <fvision/utils.h>

using namespace std;

class ImageBlockFeatureExtractorImpl : public ImageBlockFeatureExtractor {
public:
	ImageBlockFeatureExtractorImpl(size_t blockSize) {
		this->blockSize = blockSize;
		blockSquare = blockSize * blockSize;
		buffer = new ImageBlockFeature(values, 3, 0);
	}
	~ImageBlockFeatureExtractorImpl() {
		delete buffer;
	}
public:
	ImageBlockFeature* extract(const IplImage* frame, size_t xCoord, size_t yCoord, size_t id) {
		//calculate a mean r, g, b vector	
		uchar* data = (uchar*)frame->imageData;
		double sum[3] = {0, 0, 0};
		for (size_t i = 0; i < blockSize; i++) {
			size_t rowIndex = (yCoord + i) * frame->widthStep;
			for (size_t j = 0; j < blockSize; j++) {
				size_t index = rowIndex + (xCoord + j) * frame->nChannels;
				for (int k = 0; k < 3; k++) {
					sum[k] += data[index + k];
				}
			}
		}
		for (int k = 0; k < 3; k++) sum[k] /= blockSquare;
		buffer->setValues(sum);
		buffer->id = id;
		return buffer;
	}


private:
	double values[3];
	ImageBlockFeature* buffer;
	size_t blockSize;
	size_t blockSquare;
};

BackgroundAnalizer::BackgroundAnalizer(size_t blockSize) {
	this->blockSize = blockSize;
	featureExtractor = new ImageBlockFeatureExtractorImpl(blockSize);

	trainingStarted = false;
	testingStarted = false;

	distances = NULL;
	rawDistanceImage = NULL;
	foregroundBlockImage = NULL;
}

BackgroundAnalizer::~BackgroundAnalizer(void) {
	fvision::deepDelete(features);
	if (distances != NULL) {
		delete[] distances;
		cvReleaseImage(&rawDistanceImage);
		cvReleaseImage(&foregroundBlockImage);
	}
}

//
void BackgroundAnalizer::addFrame(const IplImage* frame) {
	assert(frame != NULL && !testingStarted);
	if (patchClusterManagers.size() == 0) {
		initializeWithImageSize(cvGetSize(frame));
		trainingStarted = true;
	}
	size_t gridIndex = 0;
	for (size_t i = 0; i < yCoords.size(); i++) {
		size_t offset = i * xCoords.size();
		for (size_t j = 0; j < xCoords.size(); j++) {
			gridIndex = offset + j;
			ImageBlockFeature* feature = featureExtractor->extract(frame, xCoords[j], yCoords[i], gridIndex);
			patchClusterManagers[gridIndex]->addFeature(feature);
		}
	}
}

void BackgroundAnalizer::initializeWithImageSize(const CvSize& imageSize) {
	this->imageSize = imageSize;
	xNum = (size_t)imageSize.width / blockSize;
	yNum = (size_t)imageSize.height / blockSize;
	totalNum = xNum * yNum;
	for (size_t i = 0; i < xNum; i++) xCoords.push_back(i * blockSize);
	for (size_t i = 0; i < yNum; i++) yCoords.push_back(i * blockSize);
	patchClusterManagers.resize(totalNum);
	for (size_t i = 0; i < totalNum; i++) {
		patchClusterManagers[i] = new ClusterManager(ImageBlockCluster::createInstance());
	}
}

std::vector<ImageBlockFeature*> BackgroundAnalizer::getBackgroundFeatures() {
	assert(trainingStarted);
	vector<ImageBlockFeature*> features(totalNum);
	for (size_t i = 0; i < totalNum; i++) {
		ImageBlockCluster* cluster = (ImageBlockCluster*)patchClusterManagers[i]->getMaxSizeCluster();
		features[i] = (ImageBlockFeature*)cluster->getMean()->clone();
	}
	testingStarted = true;
	return features;
}

void BackgroundAnalizer::renderImageWithBackgroundFeatures(IplImage* frame, const std::vector<ImageBlockFeature*>& features) const {
	//fill each block with the average value
	for (size_t i = 0; i < yNum; i++) {
		for (size_t j = 0; j < xNum; j++) {
			size_t index = i * xNum + j;
			ImageBlockFeature* feature = features[index];
			CvScalar color = cvScalar(feature->values[0], feature->values[1], feature->values[2]);
			fillBlock(frame, j, i, color);
		}
	}
}

void BackgroundAnalizer::getBackgroundImage(IplImage* frame) {
	if (features.size() == 0) features = getBackgroundFeatures();
	renderImageWithBackgroundFeatures(frame, features);
}

size_t BackgroundAnalizer::getGridSize() const {
	return totalNum;
}

void BackgroundAnalizer::testFrame(const IplImage* src) {
	assert (trainingStarted);
	if (features.size() == 0) {
		features = getBackgroundFeatures();
	}
	if (distances == NULL) {
		initTestFrameBuffers();
	}
	calculateBlockDistanceWithBackgroundFeatures(src, distances);
	compteRawDistanceImage(distances, rawDistanceImage);
	computeForegroundBlockImage(distances, foregroundBlockImage);
}

void BackgroundAnalizer::initTestFrameBuffers() {
	distances = new double[totalNum];
	rawDistanceImage = cvCreateImage(cvSize((int)xNum, (int)yNum), 8, 1);
	foregroundBlockImage = cvCreateImage(imageSize, 8, 3);
}

void BackgroundAnalizer::computeForegroundBlockImage(const double* distances, IplImage* foregroundImage) const {
	cvZero(foregroundImage);
	int index = 0;
	for (size_t i = 0; i < yNum; i++) {
		for (size_t j = 0; j < xNum; j++) {
			CvScalar color = CV_RGB(0, 0, 0);
			if (distances[index++] > ImageBlockCluster::threshold) {
				color = CV_RGB(255, 255, 255);
			}
			fillBlock(foregroundImage, j, i, color);
		}
	}
}

void BackgroundAnalizer::calculateBlockDistanceWithBackgroundFeatures(const IplImage* src, double* distances) {
	if (features.size() == 0) features = getBackgroundFeatures();
	//fill each block with the average value
	for (size_t i = 0; i < yNum; i++) {
		for (size_t j = 0; j < xNum; j++) {
			size_t index = i * xNum + j;
			ImageBlockFeature* feature = features[index];
			ImageBlockFeature* srcFeature = featureExtractor->extract(src, xCoords[j], yCoords[i], index);
			distances[index] = feature->computeDistance(srcFeature);
		}
	}
}

void BackgroundAnalizer::compteRawDistanceImage(const double* distances, IplImage* distanceImage) const {
	uchar* data = (uchar*)distanceImage->imageData;
	int index = 0;
	for (size_t i = 0; i < yNum; i++) {
		for (size_t j = 0; j < xNum; j++) {
			data[i * distanceImage->widthStep + j] = (uchar)distances[index++];
		}
	}
}


void BackgroundAnalizer::fillBlock(IplImage* frame, size_t xi, size_t yi, const CvScalar& color) const {
	cvRectangle(frame, cvPoint(xCoords[xi], yCoords[yi]), cvPoint(xCoords[xi] + blockSize - 1, yCoords[yi] + blockSize - 1), color, CV_FILLED);
}


#include "MultipleViewSimulation.h"

#include <fvision/geom/camera_utils.h>
#include <fvision/geom/transform_points_utils.h>
#include <fvision/utils/misc.h>
#include <fvision/utils/drawing_PointDrawer.h>
#include <fvision/utils/drawing.h>
#include <fvision/utils/mem_utils.h>
#include <fvision/utils/format_utils.h>

using namespace std;

namespace fvision {

void MultipleViewDataCreator::addLine(std::vector<std::pair<int, int>>& lines, int i, int j) {
	lines.push_back(std::pair<int, int>(i, j));
}

MultipleViewSimulation* MultipleViewSimulation::createWith(MultipleViewDataCreator* dataCreator, 
														   MultipleViewCameraCreator* cameraCreator, 
														   float noiseLevel /*= 1.0*/) 
{
	MultipleViewSimulation* sim = new MultipleViewSimulation();
	sim->noiseLevel = noiseLevel;
	dataCreator->setStructure(sim->Xs, sim->lines);
	cameraCreator->setCameras(sim->imSize, sim->cameras);
	sim->generateImagePoints();
	sim->generateImages();
	return sim;
}

void MultipleViewSimulation::generateImagePoints() {
	xss.resize(cameras.size());
	nxss.resize(cameras.size());
	xss_noisy.resize(cameras.size());
	nxss_noisy.resize(cameras.size());
	for (size_t i = 0; i < cameras.size(); i++) {
		xss[i] = cameras[i].project(Xs);
		xss_noisy[i] = addNoiseToPoints(noiseLevel, xss[i]);
		nxss[i] = normalizePointsWithCameraInternalMatrix(cameras[i].getK(), xss[i]);
		nxss_noisy[i] = normalizePointsWithCameraInternalMatrix(cameras[i].getK(), xss_noisy[i]);
	}
}

void MultipleViewSimulation::generateImages() {
	ims.resize(cameras.size());
	ims_noisy.resize(cameras.size());
	for (size_t i = 0; i < cameras.size(); i++) {
		ims[i] = cvCreateImage(imSize, 8, 3);
		drawImage(ims[i], xss[i]);
		ims_noisy[i] = cvCreateImage(imSize, 8, 3);
		drawImage(ims_noisy[i], xss_noisy[i]);
	}
}

void MultipleViewSimulation::drawImage(IplImage* im, const std::vector<CvPoint2D32f>& xs) {
	drawPoints(im, xs, pointDrawer);
	for (size_t i = 0; i < lines.size(); i++) {
		lineDrawer.draw(im, xs[lines[i].first], xs[lines[i].second]);
	}
}

MultipleViewSimulation::~MultipleViewSimulation(void) {
	releaseImageVector(ims);
	releaseImageVector(ims_noisy);
}

void MultipleViewSimulation::visualize(const ImageDemoHelper& demoHelper) {
	FormatHelper fh;
	ImageDemoHelper dh = demoHelper;
	for (size_t i = 0; i < ims.size(); i++) {
		dh.setPrefix(fh.getNumberString((int)i));
		dh.demo(ims_noisy[i], "im");
	}
}

DefaultMultipleViewCameraCreator::DefaultMultipleViewCameraCreator(const Params& params) {
	this->params = params;
}

void DefaultMultipleViewCameraCreator::setCameras(CvSize& imSize, std::vector<Camera>& cams) {

	imSize = cvSize(640, 480);

	cams.resize(params.cameraNum);
	double focalLength = 1000 * params.orthoScale;
	CvMat* K = fvision::getKFromFocalLengthAndImageSize(focalLength, imSize);
	double r = 5 * params.orthoScale;
	LookAtParams lookAtParams;
	lookAtParams.lookat = params.lookat;
	for (int i = 0; i < params.cameraNum; i++) {
		double theta = params.deltaTheta * i;
		lookAtParams.location = cvPoint3D64f(r * sin(theta), params.lookat.y, -r * cos(theta));
		Camera* cam = Camera::createWithLookAt(K, lookAtParams);
		cams[i] = *cam;
		delete cam;
	}
	cvReleaseMat(&K);
}



class HouseMultipleViewDataCreator : public MultipleViewDataCreator {
public:
	void setStructure(std::vector<CvPoint3D64f>& Xs, std::vector<std::pair<int, int>>& lines);
};

class GridCubeMultipleViewDataCreator : public MultipleViewDataCreator {
public:
	//[1, 100]
	GridCubeMultipleViewDataCreator(int gridNumEachEdge = 10);

public:
	void setStructure(std::vector<CvPoint3D64f>& Xs, std::vector<std::pair<int, int>>& lines);

private:
	int gridNumEachEdge;
};


MultipleViewSimulation* MultipleViewSimulationFactory::createHouse(const DefaultMultipleViewCameraCreator::Params& cameraParams, float noiseLevel) {

	MultipleViewDataCreator* dataCreator = new HouseMultipleViewDataCreator();
	MultipleViewCameraCreator* cameraCreator = new DefaultMultipleViewCameraCreator(cameraParams);
	MultipleViewSimulation* sim = MultipleViewSimulation::createWith(dataCreator, cameraCreator, noiseLevel);

	delete dataCreator;
	delete cameraCreator;

	return sim;
}

MultipleViewSimulation* MultipleViewSimulationFactory::createGridCube(const DefaultMultipleViewCameraCreator::Params& cameraParams, 
																	  int gridNumEachEdge, 
																	  float noiseLevel) 
{
	MultipleViewDataCreator* dataCreator = new GridCubeMultipleViewDataCreator(gridNumEachEdge);
	MultipleViewCameraCreator* cameraCreator = new DefaultMultipleViewCameraCreator(cameraParams);
	MultipleViewSimulation* sim = MultipleViewSimulation::createWith(dataCreator, cameraCreator, noiseLevel);

	delete dataCreator;
	delete cameraCreator;

	return sim;
}

void HouseMultipleViewDataCreator::setStructure(std::vector<CvPoint3D64f>& Xs, std::vector<std::pair<int, int>>& lines) {

	Xs.clear();
	lines.clear();

	double bottom = -0.5;
	double top = 0.5;
	double roof = 0.8;

	double bottomHalfLength = 0.5;
	double roofHalfLength = 0.3;

	//cubic bottom 
	Xs.push_back(cvPoint3D64f(-bottomHalfLength, bottom, -bottomHalfLength));
	Xs.push_back(cvPoint3D64f( bottomHalfLength, bottom, -bottomHalfLength));
	Xs.push_back(cvPoint3D64f( bottomHalfLength, bottom,  bottomHalfLength));
	Xs.push_back(cvPoint3D64f(-bottomHalfLength, bottom,  bottomHalfLength));

	//cubic top
	Xs.push_back(cvPoint3D64f(-bottomHalfLength, top, -bottomHalfLength));
	Xs.push_back(cvPoint3D64f( bottomHalfLength, top, -bottomHalfLength));
	Xs.push_back(cvPoint3D64f( bottomHalfLength, top,  bottomHalfLength));
	Xs.push_back(cvPoint3D64f(-bottomHalfLength, top,  bottomHalfLength));

	//roof
	Xs.push_back(cvPoint3D64f(-roofHalfLength, roof, -roofHalfLength));
	Xs.push_back(cvPoint3D64f( roofHalfLength, roof, -roofHalfLength));
	Xs.push_back(cvPoint3D64f( roofHalfLength, roof,  roofHalfLength));
	Xs.push_back(cvPoint3D64f(-roofHalfLength, roof,  roofHalfLength));

	addLine(lines, 0, 1);
	addLine(lines, 1, 2);
	addLine(lines, 2, 3);
	addLine(lines, 3, 0);
	addLine(lines, 4, 5);
	addLine(lines, 5, 6);
	addLine(lines, 6, 7);
	addLine(lines, 7, 4);
	addLine(lines, 0, 4);
	addLine(lines, 1, 5);
	addLine(lines, 2, 6);
	addLine(lines, 3, 7);
	addLine(lines, 8, 9);
	addLine(lines, 9, 10);
	addLine(lines, 10, 11);
	addLine(lines, 11, 8);
	addLine(lines, 4, 8);
	addLine(lines, 5, 9);
	addLine(lines, 6, 10);
	addLine(lines, 7, 11);

}

void GridCubeMultipleViewDataCreator::setStructure(std::vector<CvPoint3D64f>& Xs, std::vector<std::pair<int, int>>& lines) {
	double edgeHalfLength = 0.5;
	double delta = edgeHalfLength * 2 / gridNumEachEdge;
	double* coords = new double[gridNumEachEdge + 1];

	for (int i = 0; i <= gridNumEachEdge; i++) {
		coords[i] = -edgeHalfLength + delta * i;
	}

	for (int i = 0; i <= gridNumEachEdge; i++) {
		for (int j = 0; j <= gridNumEachEdge; j++) {
			Xs.push_back(cvPoint3D64f(coords[i], coords[j], coords[0]));
			Xs.push_back(cvPoint3D64f(coords[i], coords[j], coords[gridNumEachEdge]));
			Xs.push_back(cvPoint3D64f(coords[i], coords[0], coords[j]));
			Xs.push_back(cvPoint3D64f(coords[i], coords[gridNumEachEdge], coords[j]));
			Xs.push_back(cvPoint3D64f(coords[0], coords[j], coords[i]));
			Xs.push_back(cvPoint3D64f(coords[gridNumEachEdge], coords[j], coords[i]));
		}
	}

	delete[] coords;
}

GridCubeMultipleViewDataCreator::GridCubeMultipleViewDataCreator(int gridNumEachEdge /*= 10*/) {
	assert(gridNumEachEdge > 0 && gridNumEachEdge <= 100);
	this->gridNumEachEdge = gridNumEachEdge;
}

}
#include "MultipleView1DSimulation.h"

#include "camera1d_utils.h"
#include <fvision/utils/misc.h>
#include <fvision/utils/geom.h>
using namespace std;

namespace fvision {

void MultipleView1DDataCreator::addLine(std::vector<std::pair<int, int>>& lines, int i, int j) {
	lines.push_back(std::pair<int, int>(i, j));
}

MultipleView1DSimulation* MultipleView1DSimulation::createWith(
	MultipleView1DDataCreator* dataCreator, 
	MultipleView1DCameraCreator* cameraCreator, 
	float noiseLevel) 
{
	MultipleView1DSimulation* sim = new MultipleView1DSimulation();
	sim->noiseLevel = noiseLevel;
	dataCreator->setStructure(sim->Xs, sim->lines);
	cameraCreator->setCameras(sim->imSize, sim->cameras);
	sim->generateImagePoints();
	return sim;
}

void MultipleView1DSimulation::generateImagePoints() {
	xss.resize(cameras.size());
	nxss.resize(cameras.size());
	xss_noisy.resize(cameras.size());
	nxss_noisy.resize(cameras.size());
	for (size_t i = 0; i < cameras.size(); i++) {
		xss[i] = cameras[i].project(Xs);
		xss_noisy[i] = addNoiseToPoints(noiseLevel, xss[i]);
		nxss[i] = normalizePointsWithCamera1DInternalMatrix(cameras[i].getFocalLength(), cameras[i].getPrincipalPoint(), xss[i]);
		nxss_noisy[i] = normalizePointsWithCamera1DInternalMatrix(cameras[i].getFocalLength(), cameras[i].getPrincipalPoint(), xss_noisy[i]);
	}
}


MultipleView1DSimulation::~MultipleView1DSimulation(void) {
}


DefaultMultipleView1DCameraCreator::DefaultMultipleView1DCameraCreator(const Params& params) {
	this->params = params;
}

void DefaultMultipleView1DCameraCreator::setCameras(double& imSize, std::vector<Camera1D>& cams) {

	imSize = this->params.imSize;

	cams.resize(params.cameraNum);
	double r = params.rotationCenterY;
	for (int i = 0; i < params.cameraNum; i++) {
		double theta = i * params.deltaTheta;
		double angle = -theta;
		CvPoint2D32f position = cvPoint2D32f(r * sin(theta), r - r * cos(theta));
		cams[i] = Camera1D(params.focalLength, params.imSize, position, angle);
	}
}


class HouseMultipleView1DDataCreator : public MultipleView1DDataCreator {
public:
	void setStructure(std::vector<CvPoint2D32f>& Xs, std::vector<std::pair<int, int>>& lines);
};

class GridCubeMultipleView1DDataCreator : public MultipleView1DDataCreator {
public:
	//[1, 100]
	GridCubeMultipleView1DDataCreator(const CvPoint2D32f& center, int gridNumEachEdge = 10);

public:
	void setStructure(std::vector<CvPoint2D32f>& Xs, std::vector<std::pair<int, int>>& lines);

private:
	int gridNumEachEdge;
	CvPoint2D32f center;
};


MultipleView1DSimulation* MultipleView1DSimulationFactory::createHouse(const DefaultMultipleView1DCameraCreator::Params& cameraParams, float noiseLevel) {

	MultipleView1DDataCreator* dataCreator = new HouseMultipleView1DDataCreator();
	MultipleView1DCameraCreator* cameraCreator = new DefaultMultipleView1DCameraCreator(cameraParams);
	MultipleView1DSimulation* sim = MultipleView1DSimulation::createWith(dataCreator, cameraCreator, noiseLevel);

	delete dataCreator;
	delete cameraCreator;

	return sim;
}

MultipleView1DSimulation* MultipleView1DSimulationFactory::createGridCube(
	const DefaultMultipleView1DCameraCreator::Params& cameraParams, 
	int gridNumEachEdge, 
	float noiseLevel) 
{
	MultipleView1DDataCreator* dataCreator = new GridCubeMultipleView1DDataCreator(cvPoint2D32f(0, cameraParams.rotationCenterY), gridNumEachEdge);
	MultipleView1DCameraCreator* cameraCreator = new DefaultMultipleView1DCameraCreator(cameraParams);
	MultipleView1DSimulation* sim = MultipleView1DSimulation::createWith(dataCreator, cameraCreator, noiseLevel);

	delete dataCreator;
	delete cameraCreator;

	return sim;
}

void HouseMultipleView1DDataCreator::setStructure(std::vector<CvPoint2D32f>& Xs, std::vector<std::pair<int, int>>& lines) {

	Xs.clear();
	lines.clear();

	double bottom = -0.5;
	double top = 0.5;
	double roof = 0.8;

	double bottomHalfLength = 0.5;
	double roofHalfLength = 0.3;

	//cubic bottom 
	Xs.push_back(cvPoint2D32f(-bottomHalfLength, bottom));
	Xs.push_back(cvPoint2D32f( bottomHalfLength, bottom));

	//cubic top
	Xs.push_back(cvPoint2D32f(-bottomHalfLength, top));
	Xs.push_back(cvPoint2D32f( bottomHalfLength, top));

	//roof
	Xs.push_back(cvPoint2D32f(-roofHalfLength, roof));
	Xs.push_back(cvPoint2D32f( roofHalfLength, roof));

}

void GridCubeMultipleView1DDataCreator::setStructure(std::vector<CvPoint2D32f>& Xs, std::vector<std::pair<int, int>>& lines) {
	double edgeHalfLength = 3.0;
	double delta = edgeHalfLength * 2 / gridNumEachEdge;
	double* coords = new double[gridNumEachEdge + 1];

	for (int i = 0; i <= gridNumEachEdge; i++) {
		coords[i] = -edgeHalfLength + delta * i;
	}

	for (int i = 0; i <= gridNumEachEdge; i++) {
		for (int j = 0; j <= gridNumEachEdge; j++) {
			Xs.push_back(cvPoint2D32f(coords[i], coords[j]) + center);
		}
	}

	delete[] coords;
}

GridCubeMultipleView1DDataCreator::GridCubeMultipleView1DDataCreator(const CvPoint2D32f& center, int gridNumEachEdge) {
	assert(gridNumEachEdge > 0 && gridNumEachEdge <= 100);
	this->gridNumEachEdge = gridNumEachEdge;
	this->center = center;
}

}
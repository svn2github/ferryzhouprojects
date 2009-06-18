#pragma once

#include "Camera1D.h"

#include <cv.h>
#include <vector>

namespace fvision {

class MultipleView1DDataCreator {
public:
	virtual ~MultipleView1DDataCreator() {}
	virtual void setStructure(std::vector<CvPoint2D32f>& Xs, std::vector<std::pair<int, int>>& lines) = 0;
	void addLine(std::vector<std::pair<int, int>>& lines, int i, int j);
};

class MultipleView1DCameraCreator {
public:
	virtual ~MultipleView1DCameraCreator() {}
	virtual void setCameras(double& imSize, std::vector<Camera1D>& cams) = 0;
};

class MultipleView1DSimulation {
private:
	MultipleView1DSimulation(void) {}

public:
	~MultipleView1DSimulation(void);

public:
	static MultipleView1DSimulation* createWith(
		MultipleView1DDataCreator* dataCreator, 
		MultipleView1DCameraCreator* cameraCreator, 
		float noiseLevel = 1.0f);

	//void visualize(const ImageDemoHelper& demoHelper);

	//make the first camera's orientation to be I, and position to be zero
	//P' = P * H; X' = inv(H) * X
	//P'[0] = [I|0]
	//Ps, NPs, Xs change
	//void normalize();

private:
	void generateImagePoints();
	//void generateImages();

	//void drawImage(IplImage* im, const std::vector<CvPoint2D32f>& xs);

public:

	//========= Simulation Parameters ==============>
	float noiseLevel;

	//========= Ground Truth ==============>
	//n points
	std::vector<CvPoint2D32f> Xs;
	std::vector<std::pair<int, int>> lines;
	//m cameras
	double imSize;
	std::vector<Camera1D> cameras;


	//m images, n points each images
	std::vector< std::vector<double> > xss;
	//normalized. invK * xss
	std::vector< std::vector<double> > nxss;

	//======== algorithm input =========>
	//noisy points
	//m images, n points each images
	std::vector< std::vector<double> > xss_noisy;
	//normalized invK * xss_noisy
	std::vector< std::vector<double> > nxss_noisy;


	//======== buffered image ==========>
	//PointDrawer pointDrawer;
	//LineDrawer lineDrawer;

	//std::vector<IplImage*> ims;
	//std::vector<IplImage*> ims_noisy;
};

class DefaultMultipleView1DCameraCreator : public MultipleView1DCameraCreator {
public:
	class Params {
	public:
		Params() {
			focalLength = 800;
			imSize = 640;
			rotationCenterY = 10;
			deltaTheta = CV_PI / 15;
			cameraNum = 5;
		}
	public:
		double focalLength;
		double imSize;
		double rotationCenterY;
		double deltaTheta; //rotation angle
		int cameraNum;
	};

public:
	DefaultMultipleView1DCameraCreator(const Params& params);

public:
	void setCameras(double& imSize, std::vector<Camera1D>& cams);

private:
	Params params;
};



class MultipleView1DSimulationFactory {
public:
	MultipleView1DSimulation* createHouse(const DefaultMultipleView1DCameraCreator::Params& cameraParams, float noiseLevel);
	MultipleView1DSimulation* createGridCube(const DefaultMultipleView1DCameraCreator::Params& cameraParams, int gridNumEachEdge, float noiseLevel);

};

}
#pragma once

#include <fvision/geom/Camera.h>
#include <fvision/utils/drawing_PointDrawer.h>
#include <fvision/utils/drawing_LineDrawer.h>
#include <fvision/utils/drawing_ImageDemoHelper.h>

#include <vector>

#include <cv.h>

namespace fvision {

class MultipleViewDataCreator {
public:
	virtual ~MultipleViewDataCreator() {}
	virtual void setStructure(std::vector<CvPoint3D64f>& Xs, std::vector<std::pair<int, int>>& lines) = 0;
	void addLine(std::vector<std::pair<int, int>>& lines, int i, int j);
};

class MultipleViewCameraCreator {
public:
	virtual ~MultipleViewCameraCreator() {}
	virtual void setCameras(CvSize& imSize, std::vector<Camera>& cams) = 0;
};

class MultipleViewSimulation {
private:
	MultipleViewSimulation(void) {}

public:
	~MultipleViewSimulation(void);

public:
	static MultipleViewSimulation* createWith(
		MultipleViewDataCreator* dataCreator, 
		MultipleViewCameraCreator* cameraCreator, 
		float noiseLevel = 1.0f);

	void visualize(const ImageDemoHelper& demoHelper);

	//make the first camera's orientation to be I, and position to be zero
	//P' = P * H; X' = inv(H) * X
	//P'[0] = [I|0]
	//Ps, NPs, Xs change
	void normalize();

private:
	void generateImagePoints();
	void generateImages();

	void drawImage(IplImage* im, const std::vector<CvPoint2D32f>& xs);

public:

	//========= Simulation Parameters ==============>
	float noiseLevel;

	//========= Ground Truth ==============>
	//n points
	std::vector<CvPoint3D64f> Xs;
	std::vector<std::pair<int, int>> lines;
	//m cameras
	CvSize imSize;
	std::vector<Camera> cameras;


	//m images, n points each images
	std::vector< std::vector<CvPoint2D32f> > xss;
	//normalized. invK * xss
	std::vector< std::vector<CvPoint2D32f> > nxss;

	//======== algorithm input =========>
	//noisy points
	//m images, n points each images
	std::vector< std::vector<CvPoint2D32f> > xss_noisy;
	//normalized invK * xss_noisy
	std::vector< std::vector<CvPoint2D32f> > nxss_noisy;


	//======== buffered image ==========>
	PointDrawer pointDrawer;
	LineDrawer lineDrawer;

	std::vector<IplImage*> ims;
	std::vector<IplImage*> ims_noisy;
};

class DefaultMultipleViewCameraCreator : public MultipleViewCameraCreator {
public:
	class Params {
	public:
		Params() {
			orthoScale = 1.0;
			imSize = cvSize(640, 480);
			lookat = cvPoint3D64f(0, 0, 0);
			deltaTheta = CV_PI / 15;
			cameraNum = 5;
		}
	public:
		double orthoScale;
		//double focalLength;
		CvSize imSize;
		CvPoint3D64f lookat;
		double deltaTheta; //rotation angle
		int cameraNum;
	};

public:
	DefaultMultipleViewCameraCreator(const Params& params);

public:
	void setCameras(CvSize& imSize, std::vector<Camera>& cams);

private:
	Params params;
};



class MultipleViewSimulationFactory {
public:
	MultipleViewSimulation* createHouse(const DefaultMultipleViewCameraCreator::Params& cameraParams, float noiseLevel);
	MultipleViewSimulation* createGridCube(const DefaultMultipleViewCameraCreator::Params& cameraParams, int gridNumEachEdge, float noiseLevel);

};

}
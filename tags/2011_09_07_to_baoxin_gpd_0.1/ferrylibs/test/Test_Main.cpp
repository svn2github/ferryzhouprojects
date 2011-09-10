#include <iostream>

#include "Test_DLT_HC.h"
#include "Test_Camera.h"
#include "Test_MSVideo.h"
#include "Test_CvMatUtil.h"
#include "Test_TwoViewSimulation.h"
#include "Test_ImageRectifier.h"
#include "Test_ImageInterpolator.h"
#include "Test_TwoViewGeometryUtil.h"
#include "EpipolarLinesDrawer.h"
#include "Test_IOUtil.h"
#include "Test_FMC.h"
#include "Test_TwoViewFeatureTracker.h"
#include "Test_StandAloneImageRectifier.h"
#include "Test_3DReconstruction.h"
#include "Test_GeneratePatch.h"
#include "Test_ImageRotation.h"
#include "Test_Stitching.h"
#include "Test_RectificationGroupPoints.h"
#include "Test_CameraPositionEstimator.h"
#include "Test_SingleViewReconstruction.h"
#include "Test_RectificationFromSingleVideo.h"

using namespace std;
using namespace ferry::cv_geometry;
using namespace ferry::cv_geometry::test;
using namespace ferry::cv_mat::test;
using namespace ferry::feature_tracking::test;

void main() {
	//Test_DLT_HC tdlthc;
	//tdlthc.test();

	//Test_Camera tc;
	//tc.test();

	//Test_MSVideo tmv;
	//tmv.test();

	//Test_CvMatUtil tcmu;
	//tcmu.test();

	//Test_TwoViewSimulation ttvs;
	//ttvs.test();

	//Test_TwoViewGeometryUtil ttvgu;
	//ttvgu.test();

	//Test_TwoViewFeatureTracker ttvft;
	//ttvft.test();

	//Test_FMC t;
	//t.test();

	//Test_ImageRectifier tir;
	//tir.setRectificationType(Test_ImageRectifier::HARTLEY);
	//tir.test();
	//tir.continueRectify();

	//Test_ImageInterpolator tii;
	//tii.test();

	//EpipolarLinesDrawer t;
	//t.test();

	//Test_IOUtil t;
	//t.test();

	//Test_SimpleStereo tss;
	//tss.test();

	//Test_GroundPlaneDetection tgpd;
	//tgpd.test();
	//tgpd.testReal();

	//Test_StandAloneImageRectifier tsir;
	//tsir.test("temp/rectify_config.txt");

	//Test_3DReconstruction t3r;
	//t3r.test();

	//Test_GeneratePatch tgp;
	//tgp.test();

	//Test_ImageRotation tir;
	//tir.test();

	//Test_Stitching ts;
	//ts.test();

	//Test_RectificationGroupPoints trgp;
	//trgp.test();

	//Test_CameraPositionEstimator t;
	//t.test();
	//t.test2();
	//t.testCalibration();

	//Test_SingleViewReconstruction t;
	//t.test();

	Test_RectificationFromSingleVideo t;
	t.test();

	cvWaitKey(0);

	cout<<"finished................"<<endl;
	char c;
	cin>>c;
}
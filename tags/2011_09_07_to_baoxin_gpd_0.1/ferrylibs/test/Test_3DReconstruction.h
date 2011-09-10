#pragma once

#include <iostream>
#include <fstream>

#include <ferry/cv_geometry/TwoView3DReconstruction.h>
#include <ferry/cv_geometry/Camera.h>
#include <ferry/cv_geometry/IOUtil.h>

using namespace std;
using namespace ferry::cv_geometry;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_3DReconstruction
{
public:
	void test() {
		//testCalibration();
		testReconstruction();
	}

	void testReconstruction() {
		//char* Ffile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/3d_rec/kitchen/F.txt";
		//char* Kfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/3d_rec/kitchen/K.txt";
		//char* pcfile = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/3d_rec/kitchen/ixs.txt";

		char* Ffile = "data/CindyData/cam1_cam4_F.txt";
		char* pcfile = "data/CindyData/cam1_cam4_pc.txt";
		int imageWidth = 320;
		int imageHeight = 240;
		float alpha = 1.6;

		char* out_cam1_file = "data/CindyData/cam1.txt";
		char* out_cam2_file = "data/CindyData/cam4.txt";
		char* out_Xs_file = "data/CindyData/cam1_cam4_Xs.txt";

		CvMat* F = loadMat(Ffile);
		CvMat* K = getK(imageWidth, imageHeight, alpha);
		PointCorrespondences pc = loadPointCorrespondences(pcfile);

		ECalibrator ec(F, K, K, pc.x1s[1], pc.x2s[1]);
		cout<<"xs: "<<pc.x1s[0]<<", "<<pc.x2s[0]<<endl;
		ec.compute();
		CvMat* R = ec.getR();
		CvMat* t = ec.gett();

		cout<<"R: "<<R<<endl;
		cout<<"t: "<<t<<endl;

		Camera cam1(K, getIdentity(), getZero(3, 1));
		Camera cam2(K, R, t);

		vector<CvPoint3D32f> xs = triangulateToPoint3Ds(cam1.getP(), cam2.getP(), pc.x1s, pc.x2s);
		cout<<"xs: "<<xs<<endl;
		
		ofstream ofs(out_cam1_file);
		ofs<<&cam1<<endl;
		ofs.close();

		ofstream ofs1(out_cam1_file);
		ofs1<<&cam2<<endl;
		ofs1.close();

		ofstream ofs2(out_Xs_file);
		ofs2<<xs<<endl;
		ofs2.close();

		//ofstream ofs3("pc.txt");
		//ofs3<<pc<<endl;
		//ofs3.close();

		Camera* pcam1 = loadCamera(out_cam1_file);
		cout<<pcam1->getK()<<endl;
		cout<<pcam1->getR()<<endl;
		cout<<pcam1->gett()<<endl;
	}

	//for 3d tour
	void testCalibration() {
		CvMat* F;
		CvMat* K;
		PointCorrespondences pc;
		
		ifstream Fifs("data/urban_map/scene_1/small/im_db/F_t0_t1_k0_s0.txt");
		Fifs>>&F;
		Fifs.close();

		ifstream Kifs("data/urban_map/scene_1/small/im_db/K.txt");
		Kifs>>&K;
		Kifs.close();

		ifstream pcifs("data/urban_map/scene_1/small/im_db/ixs_t0_t1_k0_s0.txt");
		pcifs>>pc;
		pcifs.close();

		ECalibrator ec(F, K, K, pc.x1s[0], pc.x2s[0]);
		ec.compute();
		CvMat* R = ec.getR();
		CvMat* t = ec.gett();

		cout<<"R: "<<R<<endl;
		cout<<"t: "<<t<<endl;

		ofstream Rofs("data/urban_map/scene_1/small/im_db/R_t0_t1_k0_s0.txt");
		Rofs<<R<<endl;
		Rofs.close();

		ofstream tofs("data/urban_map/scene_1/small/im_db/t_t0_t1_k0_s0.txt");
		tofs<<t<<endl;
		tofs.close();

		CvMat* H;
		ifstream Hifs("data/urban_map/scene_1/small/im_db/H_t1_t0_k0_s0.txt");
		Hifs>>&H;
		Hifs.close();

		CvMat* Ntd = estimateNtd(K, K, R, t, H);
		cout<<"Ntd: "<<Ntd<<endl;

		ofstream Ntdofs("data/urban_map/scene_1/small/im_db/Ntd_t0_t1_k0_s0.txt");
		Ntdofs<<Ntd<<endl;
		Ntdofs.close();
	}

	void testTriangulation() {
		Camera cam1(3, cvPoint3D32f(-5, -5, 0), cvPoint3D32f(1, 1, 0), cvPoint3D32f(0, 0, 1));
		Camera cam2(2, cvPoint3D32f(5, -5, 0), cvPoint3D32f(-1, 1, 0), cvPoint3D32f(0, 0, 1));
		CvPoint3D32f X = cvPoint3D32f(1, -1, 2);
		CvPoint2D32f x1 = cam1.calcImagePoint(X);
		CvPoint2D32f x2 = cam2.calcImagePoint(X);

		CvPoint3D32f rX = triangulateToPoint3D(cam1.getP(), cam2.getP(), x1, x2);
		
		cout<<"X: "<<X<<endl;
		cout<<"rX: "<<rX<<endl;
	}

};

		}
	}
}
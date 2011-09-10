#pragma once

#include <iostream>
#include <fstream>
#include <ferry/cv_geometry/DLT_FMC.h>
#include <ferry/cv_geometry/RANSAC_FMC.h>
#include <ferry/cv_geometry/Normalized_FMC.h>
#include <ferry/cv_geometry/OpenCV_FMC.h>
#include <ferry/cv_geometry/IOUtil.h>
#include <ferry/cv_geometry/DrawingUtil.h>
#include <ferry/cv_geometry/PointCorrespondences.h>

using namespace std;
using namespace ferry::cv_geometry;
using namespace ferry::cv_geometry::io_util;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_FMC
{
public:
	void test() {
		test_DLT_FMC();
	}

	void test_DLT_FMC() {
		char* file1 = "data/CindyData/cam1_p.txt";
		char* file2 = "data/CindyData/cam4_p.txt";
		char* im1_file = "data/CindyData/cam1_image0050.png";
		char* im2_file = "data/CindyData/cam4_image0050.png";
		char* F_file = "data/CindyData/cam1_cam4_F.txt";
		char* pc_file = "data/CindyData/cam1_cam4_pc.txt";


		vector<CvPoint2D32f> x1s;
		vector<CvPoint2D32f> x2s;

		ifstream ifs1(file1);
		ifs1>>x1s;
		ifs1.close();

		ifstream ifs2(file2);
		ifs2>>x2s;
		ifs2.close();

		PointCorrespondences pc(x1s, x2s);
		ofstream pcofs(pc_file);
		pcofs<<pc;
		pcofs.close();
		
		DLT_FMC dltfmc;
		//Normalized_FMC nfmc(&dltfmc);

		RANSAC_FMC ransac_fmc(0.05);
		Normalized_FMC nfmc(&ransac_fmc);

		//OpenCV_FMC nfmc(CV_FM_RANSAC, 1, 0.99);
		//OpenCV_FMC nfmc(CV_FM_LMEDS, 1, 0.99);

		//FundamentalMatrix fm = dltfmc.compute(x1s, x2s);
		FundamentalMatrix fm = nfmc.compute(x1s, x2s);

		cout<<fm.toString()<<endl;

		ofstream ffs(F_file);
		ffs<<fm.getF();
		ffs.close();

		FundamentalMatrixErrorCalculator* pfmec = new PointLine_FMEC();
		//FundamentalMatrixErrorCalculator* pfmec = new SimpleMultiply_FMEC();
		FundamentalMatrixBatchErrorCalculator fmbec(fm.getF(), pfmec);
		double error = fmbec.compute(x1s, x2s);
		cout<<"error: "<<error<<endl;

		FundamentalMatrixInlierOutlierModel fmiom(fm.getF(), pfmec, 1.0);
		vector<CvPoint2D32f> ix1s, ix2s, ox1s, ox2s;
		fmiom.classify(x1s, x2s, ix1s, ix2s, ox1s, ox2s);

		ofstream x1fs("temp/ix1s.txt");
		x1fs<<ix1s;
		x1fs.close();

		ofstream x2fs("temp/ix2s.txt");
		x2fs<<ix2s;
		x2fs.close();

		//char* im1_file = "data/MS_Video/dancing/3_jpg_small.jpg";
		//char* im2_file = "data/MS_Video/dancing/4_jpg_small.jpg";

		//char* im1_file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/snoopy3/0808.jpg";
		//char* im2_file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/snoopy3/0809.jpg";

		//char* im1_file = "data/olympus/1_small.jpg";
		//char* im2_file = "data/olympus/3_small.jpg";

		//char* im1_file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/simulation/view_3.bmp";
		//char* im2_file = "C:/zj/vcprojects/cv_3d_reconstruction/test_cv_geometry/data/simulation/view_1.bmp";

		IplImage* im1 = cvLoadImage(im1_file, 1);
		IplImage* im2 = cvLoadImage(im2_file, 1);

		::draw_correspondences_image("cos1", im1, cvPointsFrom32fs(x1s), cvPointsFrom32fs(x2s));
		::draw_correspondences_image("cos2", im1, im2, cvPointsFrom32fs(x1s), cvPointsFrom32fs(x2s));

		::draw_correspondences_image("inliers cos1", im1, cvPointsFrom32fs(ix1s), cvPointsFrom32fs(ix2s));

		CvMat* F = fm.getF();
		drawEpipoleLines(im1, im2, F);

		int i = 1;
		drawEpipolarLine(im1, im2, F, cvPoint2D32f(261.0, 408.0), cvPoint2D32f(149.0, 412.0));
		drawEpipolarLine(im1, im2, F, cvPoint2D32f(185.0, 316.0), cvPoint2D32f(96.0, 322.0));
		drawEpipolarLine(im1, im2, F, cvPoint2D32f(229.0, 76.0), cvPoint2D32f(165.0, 82.0));

		drawEpipolarLine(im1, im2, F, x1s[0], x2s[0]);
		drawEpipolarLine(im1, im2, F, x1s[2], x2s[2]);
		drawEpipolarLine(im1, im2, F, x1s[1], x2s[1]);

		cvNamedWindow("im1", 1);
		cvNamedWindow("im2", 1);
		cvShowImage("im1", im1);
		cvShowImage("im2", im2);

	}

	void drawEpipolarLine(IplImage* im1, IplImage* im2, CvMat* F, const CvPoint2D32f& p1, const CvPoint2D32f& p2) {
		drawCross(im1, cvPointFrom32f(p1));
		drawCross(im2, cvPointFrom32f(p2));
		CvMat* mx1 = hmatFromPoint2D(p1);
		CvMat* line2 = matMul(F, mx1);
		drawLine(im2, line2, CV_RGB(255, 0, 0));
		CvMat* mx2 = hmatFromPoint2D(p2);
		CvMat* line1 = matMul(transpose(F), mx2);
		drawLine(im1, line1, CV_RGB(255, 0, 0));

		cout<<"point2: "<<mx2<<endl;
		cout<<"line2: "<<line2<<endl;
		cout<<"point line distance: "<<point2LineDistance(mx2, line2)<<endl;
		cout<<"point line distance: "<<point2LineDistance(mx1, line1)<<endl;
		cout<<"point1: "<<mx1<<endl;
		cout<<"line1: "<<line1<<endl;
		cout<<"point line distance: "<<point2LineDistance(mx1, line1)<<endl;
		//cout<<"Pfmec: "<<pfmec->compute(F, p1, p2)<<endl;

	}
};

		}
	}
}
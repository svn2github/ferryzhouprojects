#pragma once

#include <iostream>
#include <stdio.h>

#include <ferry/cv_geometry/Camera.h>

using namespace std;
using namespace ferry::cv_geometry;

namespace ferry {
	namespace cv_geometry {
		namespace test {

class Test_MSVideo
{
public:
	void test() {
		char* calibfile = "calibParams-breakdancers.txt";
		readCalibrationFile(calibfile);
		/*for (int i = 0; i < 8; i++) {
			cout<<"K: "<<CvMatUtil::matString(cams[i]->getK())<<endl;
			cout<<"NP: "<<CvMatUtil::matString(cams[i]->getNP())<<endl;
			cout<<CvMatUtil::matString(cams[i]->getP())<<endl;
		}*/
		int u = 300;
		int v = 250;
		unsigned char d = 107;
		double z = depthLevelToZ(d);
		int height = 768;

		CvPoint2D32f p = cvPoint2D32f(300, height - 250 - 1);
		CvPoint3D32f p3d = cams[0]->calcPoint3DWithDepth(p, z);
		cout<<"3d point: "<<p3d.x<<", "<<p3d.y<<", "<<p3d.z<<endl;
		for (int i = 0; i < 8; i++) {
			CvPoint2D32f pi = cams[i]->calcImagePoint(p3d);
			cout<<pi.x<<", "<<height - pi.y - 1<<endl;
		}
	}

	// d is pixel value in the depth images. 
	// d represents depth, but is NOT the actual depth value.
	// This function returns actual z (depth)
	double depthLevelToZ( unsigned char d )
	{
		double z;
		double MinZ = 44.0, MaxZ = 120.0;

		z = 1.0/((d/255.0)*(1.0/MinZ - 1.0/MaxZ) + 1.0/MaxZ);
		return z;
	}

	void readCalibrationFile(char *fileName)
	{
		int i, j, k;
		FILE *pIn;
		double dIn; // dummy variable
		int camIdx;

		if(pIn = fopen(fileName, "r"))
		{
			for(int t=0; t<8; t++)
			{
				// camera index
				fscanf(pIn, "%d", &camIdx);

				CvMat* K = cvCreateMat(3, 3, CV_64FC1);
				// camera intrinsics
				for (i=0; i<3; i++)
					fscanf(pIn, "%lf\t%lf\t%lf", &(K->data.db[i*3+0]),
					&(K->data.db[i*3+1]), &(K->data.db[i*3+2]));

				// read barrel distortion params (assume 0)
				fscanf(pIn, "%lf", &dIn);
				fscanf(pIn, "%lf", &dIn);

				CvMat* NP = cvCreateMat(3, 4, CV_64FC1);
				// read extrinsics
				for(i=0;i<3;i++)
				{
					for(j=0;j<4;j++)
					{
						fscanf(pIn, "%lf", &dIn);
						NP->data.db[i * 4 + j] = dIn;
					}
				}

				cams[t] = new Camera(K, NP);
			}


			fclose(pIn);
		}
	} // readCalibrationFile

public:
	Camera* cams[8];
};
		}
	}
}
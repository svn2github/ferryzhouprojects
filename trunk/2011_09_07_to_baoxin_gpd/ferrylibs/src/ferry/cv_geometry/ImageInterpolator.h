#pragma once


#include "highgui.h"
#include "cv.h"

#include "CvMatUtil.h"
#include "Camera.h"

using namespace std;
using namespace ferry::cv_mat;

namespace ferry {
	namespace cv_geometry {

class ImageInterpolator
{
public:
	ImageInterpolator(IplImage* color_im1, IplImage* color_im2, 
		IplImage* depth_im1, IplImage* depth_im2, Camera* pcam1, Camera* pcam2) {
		this->color_im1 = color_im1;
		this->color_im2 = color_im2;
		this->depth_im1 = depth_im1;
		this->depth_im2 = depth_im2;
		this->pcam1 = pcam1;
		this->pcam2 = pcam2;

		getProjMatrix(pcam1->getP(), projMatrix1);
		getProjMatrix(pcam2->getP(), projMatrix2);

		this->width = color_im1->width;
		this->height = color_im1->height;
	}

public:
	IplImage* interpolate(Camera* pcam) {
		IplImage* dst_im = cvCreateImage(cvSize(width, height), 8, 3);
		IplImage* occupiedImage = cvCreateImage(cvSize(width, height), 8, 1);

		unsigned char* color_left_data = (unsigned char*)color_im1->imageData;
		unsigned char* color_right_data = (unsigned char*)color_im2->imageData;
		unsigned char* depth_left_data = (unsigned char*)depth_im1->imageData;
		unsigned char* depth_right_data = (unsigned char*)depth_im2->imageData;
		unsigned char* dst_data = (unsigned char*)dst_im->imageData;

		unsigned char* occupied_data = (unsigned char*)occupiedImage->imageData;

		unsigned char d;
		double projMatrix[4][4];
		getProjMatrix(pcam->getP(), projMatrix);

		//==DEBUG
		cout<<"projMatrix1: "<<endl;
		printProjMatrix(projMatrix1);
		cout<<"projMatrix2: "<<endl;
		printProjMatrix(projMatrix2);
		cout<<"projMatrix: "<<endl;
		printProjMatrix(projMatrix);
		//==

		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				int index = j * width + i;
				int d = depth_left_data[index];
				double x, y, z = DepthLevelToZ(d);
				projUVZtoXY(projMatrix1, (double)i, (double)j, z, &x, &y);
				double ndx, ndy;
				projXYZtoUV(projMatrix, x, y, z, &ndx, &ndy);
				int nx = (int)ndx;
				int ny = (int)ndy;
				if (nx < 0 || nx > width || ny < 0 || ny > height) continue;
				int nIndex = (int)ny * width + (int)nx;
				occupied_data[nIndex] = 255;
				for (int k = 0; k < 3; k++) {
					dst_data[nIndex * 3 + k] = color_left_data[index * 3 + k];
				}
			}
			cout<<j<<" ................"<<endl;
		}

		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				int index = j * width + i;
				int d = depth_right_data[index];
				double x, y, z = DepthLevelToZ(d);
				projUVZtoXY(projMatrix2, (double)i, (double)j, z, &x, &y);
				double ndx, ndy;
				projXYZtoUV(projMatrix, x, y, z, &ndx, &ndy);
				int nx = (int)ndx;
				int ny = (int)ndy;
				if (nx < 0 || nx > width || ny < 0 || ny > height) continue;
				int nIndex = (int)ny * width + (int)nx;
				
				if (occupied_data[nIndex] == 255) continue;
				occupied_data[nIndex] = 255;
				for (int k = 0; k < 3; k++) {
					dst_data[nIndex * 3 + k] = color_right_data[index * 3 + k];
				}
			}
			cout<<j<<" ................"<<endl;
		}
		
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				int index = j * width + i;
				if (occupied_data[index] == 255) continue;
				for (int k = 0; k < 3; k++) {
					dst_data[index * 3 + k] = dst_data[(index - 1) * 3 + k];
				}
			}
		}

		//cvReleaseImage(&occupiedImage);

		return dst_im;
	}

	void getProjMatrix(CvMat* P, double projMatrix[4][4]) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				projMatrix[i][j] = cvmGet(P, i, j);
			}
		}
		projMatrix[3][0] = 0.0;
		projMatrix[3][1] = 0.0;
		projMatrix[3][2] = 0.0;
		projMatrix[3][3] = 1.0;
	}

	// d is pixel value in the depth images. 
	// d represents depth, but is NOT the actual depth value.
	// This function returns actual z (depth)
	double DepthLevelToZ( unsigned char d )
	{
		double z;
		double MinZ = 44.0, MaxZ = 120.0;

		z = 1.0/((d/255.0)*(1.0/MinZ - 1.0/MaxZ) + 1.0/MaxZ);
		return z;
	}

	void projUVZtoXY(double projMatrix[4][4], double u, double v, double z, double *x, double *y)
	{
		double c0, c1, c2;

		// image (0,0) is bottom lefthand corner
		v = (double) height - v - 1.0;

		c0 = z*projMatrix[0][2] + projMatrix[0][3];
		c1 = z*projMatrix[1][2] + projMatrix[1][3];
		c2 = z*projMatrix[2][2] + projMatrix[2][3];

		*y = u*(c1*projMatrix[2][0] - projMatrix[1][0]*c2) +
			v*(c2*projMatrix[0][0] - projMatrix[2][0]*c0) +
			projMatrix[1][0]*c0 - c1*projMatrix[0][0];

		*y /= v*(projMatrix[2][0]*projMatrix[0][1] - projMatrix[2][1]*projMatrix[0][0]) +
			u*(projMatrix[1][0]*projMatrix[2][1] - projMatrix[1][1]*projMatrix[2][0]) +
			projMatrix[0][0]*projMatrix[1][1] - projMatrix[1][0]*projMatrix[0][1];
			
		*x = (*y)*(projMatrix[0][1] - projMatrix[2][1]*u) + c0 - c2*u;
		*x /= projMatrix[2][0]*u - projMatrix[0][0];
	} // projUVZtoXY


	double projXYZtoUV(double projMatrix[4][4], double x, double y, double z, double *u, double *v)
	{
		double w;

		*u = projMatrix[0][0]*x + 
			projMatrix[0][1]*y + 
			projMatrix[0][2]*z + 
			projMatrix[0][3];

		*v = projMatrix[1][0]*x + 
			projMatrix[1][1]*y + 
			projMatrix[1][2]*z + 
			projMatrix[1][3];

		w = projMatrix[2][0]*x + 
			projMatrix[2][1]*y + 
			projMatrix[2][2]*z + 
			projMatrix[2][3];

		*u /= w;
		*v /= w;

		// image (0,0) is bottom lefthand corner
		*v = (double) height - *v - 1.0;

		return w;

	} // projXYZtoUV

private:
	void printProjMatrix(double projMatrix[4][4]) {
		cout<<endl;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				cout<<projMatrix[i][j]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	}

private:
	IplImage* color_im1;
	IplImage* color_im2;
    IplImage* depth_im1;
	IplImage* depth_im2;
	Camera* pcam1;
	Camera* pcam2;

	double projMatrix1[4][4];
	double projMatrix2[4][4];

	int width;
	int height;
};

	}
}
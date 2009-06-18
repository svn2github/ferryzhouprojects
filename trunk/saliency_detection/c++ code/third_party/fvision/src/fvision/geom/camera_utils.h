#pragma once

#include <cv.h>

namespace fvision {

CvPoint2D32f getPrincipalPointFromImageSize(const CvSize& imSize);

double getFocalLengthFromVerticalFovAndImageHeight(double vFov, double height);

double getFocalLengthFromHorizontalFovAndImageWidth(double hFov, double width);

//height/2 / focallength = tan(fov/2)
//fov = atan(height, 2 * focalLength) * 2
double getVerticalFovFromFocalLengthAndImageHeight(double focalLength, double height);

//=================== get K ================================================================>
/**
* K = [f,  0, pp.x;
*      0,  -f, pp.y;
*      0,  0,    1]
*   = [1, 0,     0        [f,  0,  pp.x
*      0, -1, 2*py.   *    0,  f,  pp.y
*      0, 0,    1]         0,  0,    1]
*
*
* This conforms the normal image coordinate system, where y axis goes from up to down
* i.e. the up left coordinates are (0, 0)
*
* In this coordinate system, the z axis is point forward from camera center to image plane
* thus the depth of points should be positive if it is in front of the camera.
*
* In the original camera coordinate system, the y axis points up
*
* see http://code.google.com/p/fvision2008/wiki/CameraSystem
*
* A simple case
* x = KX
* if X = [1, 1, 1]
* then x = [f + pp.x, pp.y - f, 1], it a point in up right of the image
*/
CvMat* getKFromFocalLengthAndPrinciplePoint(double focalLength, const CvPoint2D32f& pp);

CvMat* getKFromFocalLengthAndImageSize(double focalLength, const CvSize& imSize);

//focalLength = py / tan(vFov / 2)
CvMat* getKFromVerticalFovAndImageSize(double vFov, const CvSize& imSize);

//focalLength = px / tan(hFov / 2)
CvMat* getKFromHorizontalFovAndImageSize(double hFov, const CvSize& imSize);

//[f, 0, width/2; 0, -f, height/2; 0, 0, 1] f = alpha * width
CvMat* getKFromImageWidthHeightAndWidthAlpha(int width, int height, double alpha);




//new focal length = scale * old focal length
CvMat* getScaledK(CvMat* K, double scale);



CvMat* getRInLeftHandSystem(const CvPoint3D64f& location, const CvPoint3D64f& lookat, const CvPoint3D64f& up = cvPoint3D64f(0, 1, 0));

//[I|0]
CvMat* getStandardNP1();

//[R|t]
CvMat* getNP(const CvMat* R, const CvMat* t);

CvMat* getP(const CvMat* K, const CvMat* R, const CvMat* t);
//t = -RC
CvMat* getP(const CvMat* K, const CvMat* R, const CvPoint3D64f& C);

}
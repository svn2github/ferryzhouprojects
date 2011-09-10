#pragma once

#include <vector>

#include <highgui.h>
#include <cv.h>

using namespace std;

#include "RhoThetaLine.h"
#include "Util.h"

/**
 * rho, theta => x = x0 - bt, y = y0 + at
 * a = cos(theta), b = sin(theta)
 * x0 = a * rho, y0 = b * rho
 */
class ParamLine {

public:
	ParamLine(const RhoThetaLine& rtLine, int imWidth, int imHeight) {
		float rho = rtLine.rho;
		float theta = rtLine.theta;

		a = cos(theta);
		b = sin(theta);

		x0 = a * rho;
		y0 = b * rho;

		mint = -100000;
		maxt = 100000;
		double t[4] = {0, 0, 0, 0};
		if (abs(b) > 0.00001) {
			t[0] = x0 / b;
			t[1] = (x0 - imWidth) / b;
		}
		if (abs(a) > 0.00001) {
			t[2] = -y0 / a;
			t[3] = (imHeight - y0) / a;
		}

		for (int i = 0; i < 4; i++) {
			if (t[i] < 0 && t[i] > mint) mint = t[i];
			if (t[i] > 0 && t[i] < maxt) maxt = t[i];
		}
	}

	//if p[i] = 1, add 1; if p[i] +- 1 = 1, add 0.5;
	//if a <= b, y +- 1; else x +- 1
	double getSupport(IplImage* edgeIm) {

		double support = 0;

		int x = 0;
		int y = 0;

		int prevx = -1;
		int prevy = -1;

		const double deltaT = 0.5;
		for (double t = mint; t < maxt; t += deltaT) {
			x = cvRound(x0 - b * t);
			y = cvRound(y0 + a * t);

			if (x == prevx && y == prevy) continue;

			if (x < 0 || x >= edgeIm->width || y < 0 || y >= edgeIm->height) continue;

			prevx = x; prevy = y;

			int pixel = (int)cvGetReal2D(edgeIm, y, x);
			if (pixel == 255) support++;

			if (a <= b) {
				if ((y - 1) >= 0 && (int)cvGetReal2D(edgeIm, y-1, x) == 255) support += 0.5;
				if ((y + 1) < edgeIm->height && (int)cvGetReal2D(edgeIm, y+1, x) == 255) support += 0.5;
			} else {
				if ((x - 1) >= 0 && (int)cvGetReal2D(edgeIm, y, x - 1) == 255) support += 0.5;
				if ((x + 1) <= edgeIm->width && (int)cvGetReal2D(edgeIm, y, x + 1) == 255) support += 0.5;
			}
		}

		return support;
	}

public:
	double x0;
	double y0;

	double a;
	double b;

	double maxt;
	double mint;
};

class VerticalLineDetector
{
public:
	VerticalLineDetector(int method, double rho_precision, double theta_precision, int threshold, double param1, double param2) {
		this->method = method;
		this->rho_precision = rho_precision;
		this->theta_precision = theta_precision;
		this->threshold = threshold;
		this->param1 = param1;
		this->param2 = param2;
	}

	~VerticalLineDetector(void) {
	}

public:
	vector<RhoThetaLine> process(IplImage* im) {
		IplImage* dst = cvCreateImage( cvGetSize(im), 8, 1 );
		CvMemStorage* storage = cvCreateMemStorage(0);
		CvSeq* lines = 0;
		cvCanny( im, dst, 50, 200, 3 );

		cvNamedWindow("canny", 1);
		cvShowImage("canny", dst);
		//cvSaveImage("data/robot/canny_30_100.jpg", dst);

        lines = cvHoughLines2( dst, storage, method, rho_precision, theta_precision, threshold, param1, param2);
        for(int i = 0; i < lines->total; i++ )
        {
            float* line = (float*)cvGetSeqElem(lines,i);
            float rho = line[0];
            float theta = line[1];

			if (abs(theta) < CV_PI/30 * 2) {
				vlines.push_back(RhoThetaLine(rho, theta));
			}
		}

		cvNamedWindow("canny", 1);
		cvShowImage("canny", dst);

		IplImage* im1 = cvCloneImage(im);
		drawLines(im1, vlines);
		cvNamedWindow("before", 1);
		cvShowImage("before", im1);

		vlines = filterLines(vlines, dst);
	
		return vlines;
	}

public:
	//filter out repetitive lines, pick out best one
	vector<RhoThetaLine> filterLines(vector<RhoThetaLine>& vlines, IplImage* edge) {

		vector<RhoThetaLine> out;

		vector<int> label(vlines.size(), -1); // if label  = 1, its the best one among clusters
		for (int i = 0; i < (int)vlines.size(); i++) {
			if (label[i] != -1) continue;
			RhoThetaLine best = vlines[i];
			int bestIndex = i;
			label[bestIndex] = 1;
			ParamLine pl(best, edge->width, edge->height);
			double bsupport = pl.getSupport(edge);
			for (int j = i + 1; j < (int)vlines.size(); j++) {
				RhoThetaLine rtl = vlines[j];
				if (abs(rtl.rho - best.rho) < rho_precision * 5 && abs(rtl.theta - best.theta) < theta_precision * 5) {
					ParamLine pl(rtl, edge->width, edge->height);
					double s = pl.getSupport(edge);
					if (s > bsupport) {
						label[bestIndex] = 0;
						bestIndex = j;
						label[j] = 1;
						bsupport = s;
					} else label[j] = 0;
				}
			}

			out.push_back(vlines[bestIndex]);
		}
		return out;
	}

private:
	vector<RhoThetaLine> vlines;

	int method;
	double rho_precision;
	double theta_precision;
	int threshold;
	double param1;
	double param2;
};

#pragma once

#include <vector>
#include <stdio.h>

#include "cv.h"

#define MAXP 5000
#define MAXT 5000

using namespace std;

namespace ferry {
	namespace cv_geometry {

class TriangleMesh {
public:
	TriangleMesh() {
		nump = 0;
		numt = 0;
	}

public:
	float  pnt[MAXP][3];   //points
	int    nump;
	int    tri[MAXT][3];   //triangles
	int    numt;

public:
	void readOffFile(char* file) {

		int dum;
		int i, j;
		char str[100];

		FILE* infile=fopen(file,"r");

		fscanf(infile,"%s",str);
		fscanf(infile,"%d %d %d", &nump, &numt, &dum);

		for(i=0; i< nump; i++)
			fscanf(infile,"%f %f %f",&pnt[i][0],&pnt[i][1], &pnt[i][2]);

		
		for(i=0; i< numt; i++)
			fscanf(infile,"%d %d %d %d", &j, &tri[i][0], &tri[i][1], &tri[i][2]);

		fclose(infile);
	}

	void readTriangles(char* filename) {
		FILE* infile = fopen(filename, "r");

		fscanf(infile,"%d", &numt);
		
		for(int i=0; i< numt; i++)
			fscanf(infile,"%d %d %d", &tri[i][0], &tri[i][1], &tri[i][2]);

		fclose(infile);
	}

	void setPoints(const vector<CvPoint3D32f>& Xs) {
		nump = (int)Xs.size();
		for (int i = 0; i < nump; i++) {
			pnt[i][0] = (Xs[i]).x;
			pnt[i][1] = (Xs[i]).y;
			pnt[i][2] = (Xs[i]).z;
		}
	}

	void save(char* file) {
		int dum = 0;
		int i, j;
		char str[100];

		FILE* outfile=fopen(file,"w");

		fprintf(outfile,"%s\r\n","OFF");
		fprintf(outfile,"%d %d %d\r\n", nump, numt, dum);

		for(i=0; i< nump; i++)
			fprintf(outfile,"%f %f %f\r\n",pnt[i][0],pnt[i][1], pnt[i][2]);

		
		for(i=0; i< numt; i++)
			fprintf(outfile,"%d %d %d %d\r\n", 3, tri[i][0], tri[i][1], tri[i][2]);

		fclose(outfile);
	}
};

	}
}
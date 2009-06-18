#include "io.h"

#include <fstream>
#include <iomanip>

using namespace std;

namespace fvision {

ostream& operator<<(ostream& os, const vector<CvPoint2D32f>& pts) {
	os<<pts.size()<<"\n";
	for (int i = 0; i < (int)pts.size(); i++) {
		os<<pts[i].x<<" "<<pts[i].y<<"\n";
	}
	return os;
}

istream& operator>>(istream& is, vector<CvPoint2D32f>& pts) {
	int n;
	is>>n;
	for (int i = 0; i < n; i++) {
		CvPoint2D32f p;
		is>>p.x;
		is>>p.y;
		pts.push_back(p);
	}
	return is;
}

ostream& operator<<(ostream& os, const CvPoint2D32f& pt) {
	os<<pt.x<<" "<<pt.y;
	return os;
}

istream& operator>>(istream& is, CvPoint2D32f& pt) {
	is >> pt.x >> pt.y;
	return is;
}

ostream& operator<<(ostream& os, const CvPoint2D64f& pt) {
	os<<pt.x<<" "<<pt.y;
	return os;
}

istream& operator>>(istream& is, CvPoint2D64f& pt) {
	is >> pt.x >> pt.y;
	return is;
}

ostream& operator<<(ostream& os, const vector<CvPoint>& pts) {
	os<<pts.size()<<"\n";
	for (int i = 0; i < (int)pts.size(); i++) {
		os<<pts[i].x<<" "<<pts[i].y<<"\n";
	}
	return os;
}

istream& operator>>(istream& is, vector<CvPoint>& pts) {
	int n;
	is>>n;
	for (int i = 0; i < n; i++) {
		CvPoint p;
		is>>p.x;
		is>>p.y;
		pts.push_back(p);
	}
	return is;
}

ostream& operator<<(ostream& os, const CvPoint& pt) {
	os<<pt.x<<" "<<pt.y;
	return os;
}

istream& operator>>(istream& is, CvPoint& pt) {
	is >> pt.x >> pt.y;
	return is;
}

ostream& operator<<(ostream& os, const CvPoint3D32f& pt) {
	os<<pt.x<<" "<<pt.y<<" "<<pt.z;
	return os;
}

istream& operator>>(istream& is, CvPoint3D32f& pt) {
	is >> pt.x >> pt.y >> pt.z;
	return is;
}

ostream& operator<<(ostream& os, const CvPoint3D64f& pt) {
	os<<pt.x<<" "<<pt.y<<" "<<pt.z;
	return os;
}

istream& operator>>(istream& is, CvPoint3D64f& pt) {
	is >> pt.x >> pt.y >> pt.z;
	return is;
}

ostream& operator<<(ostream& os, const vector<CvPoint3D32f>& pts) {
	os<<pts.size()<<"\n";
	for (int i = 0; i < (int)pts.size(); i++) {
		os<<pts[i].x<<" "<<pts[i].y<<" "<<pts[i].z<<"\n";
	}
	return os;
}

istream& operator>>(istream& is, vector<CvPoint3D32f>& pts) {
	int n;
	is>>n;
	for (int i = 0; i < n; i++) {
		CvPoint3D32f p;
		is>>p.x;
		is>>p.y;
		is>>p.z;
		pts.push_back(p);
	}
	return is;
}

ostream& operator<<(ostream& os, const CvMat* M) {
	if (M == NULL) {
		os << 0 << " " << 0 << " " << 0 <<endl;
		return os;
	}
	os << M->rows << " " << M->cols << " " << M->type << "\n";
	for (int i = 0; i < M->rows; i++) {
		for (int j = 0; j < M->cols; j++) {
			os << cvmGet(M, i, j) << " ";
		}
		os << "\n";
	}
	return os;
}

istream& operator>>(istream& is, CvMat** pM) {
	int rows;
	int cols;
	int type;
	is >> rows >> cols >> type;
	if (rows == 0) {
		*pM = NULL;
		return is;
	}
	*pM = cvCreateMat(rows, cols, type);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			double v;
			is >> v;
			cvmSet(*pM, i, j, v);
		}
	}
	return is;
}


CvMat* loadMat(const char* filename) {
	ifstream ifs(filename);
	CvMat* M;
	ifs>>&M;
	ifs.close();
	return M;
}

void toMatlabFile(ostream& os, const CvMat* M, const char* varname) {
	os<<varname<<" = [";
	for (int i = 0; i < M->rows; i++) {
		for (int j = 0; j < M->cols; j++) {
			os << cvmGet(M, i, j) << " ";
		}
		os << ";\n";
	}
	os<<"]"<<endl;
}

void load(const char* filename, vector<CvMat*>& ms) {
	ifstream ifs(filename);
	int n;
	ifs>>n;
	for (int i = 0; i < n; i++) {
		CvMat* m;
		ifs>>&m;
		ms.push_back(m);
	}
	ifs.close();
}

void save(const CvMat* M, const char* filename) {
	ofstream ofs(filename);
	ofs<<M;
	ofs.close();
}

void save(const vector<CvMat*>& ms, const char* filename) {
	ofstream ofs(filename);
	ofs<<ms.size()<<endl;
	for (int i = 0; i < (int)ms.size(); i++) {
		ofs<<ms[i]<<endl;
	}
	ofs.close();
}

void save(const vector<double>& xs, const char* filename) {
	ofstream ofs(filename);
	for (int i = 0; i < (int)xs.size(); i++) {
		ofs<<xs[i]<<endl;
	}
	ofs.close();		
}

void save(const vector<float>& xs, const char* filename) {
	ofstream ofs(filename);
	for (int i = 0; i < (int)xs.size(); i++) {
		ofs<<xs[i]<<endl;
	}
	ofs.close();		
}

void save(const vector<CvPoint3D32f>& pts, const char* filename) {
	ofstream ofs(filename);
	ofs<<pts;
	ofs.close();
}

void saveMatWithNoHeader(const char* filename, const CvMat* M) {
	ofstream ofs(filename);
	for (int i = 0; i < M->rows; i++) {
		for (int j = 0; j < M->cols; j++) {
			ofs<<cvmGet(M, i, j)<<" ";
		}
		ofs<<endl;
	}
	ofs.close();
}

ostream& operator<<(ostream& os, const vector<double>& vs) {
	for (size_t i = 0; i < vs.size(); i++) {
		os<<setw(15)<<setprecision(4)<<vs[i];
	}
	return os;
}


}

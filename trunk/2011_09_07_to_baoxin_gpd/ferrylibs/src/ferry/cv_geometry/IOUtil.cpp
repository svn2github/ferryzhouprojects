#include ".\ioutil.h"

#include <fstream>

namespace ferry {
	namespace cv_geometry { 
		namespace io_util {
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

			ostream& operator<<(ostream& os, const PointCorrespondences& pc) {
				os<<pc.x1s<<pc.x2s;
				return os;
			}

			istream& operator>>(istream& is, PointCorrespondences& pc) {
				pc.clear();
				is>>pc.x1s>>pc.x2s;
				return is;
			}

			ostream& operator<<(ostream& os, Camera* pcam) {
				CvMat* K = pcam->getK();
				os<<K<<endl;
				os<<pcam->getR()<<endl;
				os<<pcam->gett()<<endl;
				return os;
			}

			istream& operator>>(istream& is, Camera** pcam) {
				CvMat* K;
				CvMat* R;
				CvMat* t;
				is>>&K>>&R>>&t;
				*pcam = new Camera(K, R, t);
				return is;
			}

			CvMat* loadMat(const char* filename) {
				ifstream ifs(filename);
				CvMat* M;
				ifs>>&M;
				ifs.close();
				return M;
			}

			PointCorrespondences loadPointCorrespondences(const char* filename) {
				ifstream ifs(filename);
				PointCorrespondences pc;
				ifs>>pc;
				ifs.close();
				return pc;
			}

			Camera* loadCamera(const char* filename) {
				ifstream ifs(filename);
				Camera* pcam;
				ifs>>&pcam;
				ifs.close();
				return pcam;
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

		}
	}
}

#include ".\basicutil.h"

namespace ferry {
	namespace cv_geometry {

		//pt: 3 x 1
		//line: 3 x 1
		double point2LineDistance(const CvMat* pt, const CvMat* line) {
			assertH3(pt);
			assertH3(line);

			double x0 = cvmGet(pt, 0, 0) / cvmGet(pt, 2, 0);
			double y0 = cvmGet(pt, 1, 0) / cvmGet(pt, 2, 0);
			double a = cvmGet(line, 0, 0);
			double b = cvmGet(line, 1, 0);
			double c = cvmGet(line, 2, 0);

			return abs(a * x0 + b * y0 + c) / sqrt(a * a + b * b);
		}

		vector<CvPoint> cvPointsFrom32fs(const vector<CvPoint2D32f>& pts) {
			vector<CvPoint> ps;
			for (int i = 0; i < (int)pts.size(); i++) {
				ps.push_back(cvPointFrom32f(pts[i]));
			}
			return ps;
		}

		vector<CvPoint2D32f> cvPointsTo32fs(const vector<CvPoint>& pts) {
			vector<CvPoint2D32f> ps;
			for (int i = 0; i < (int)pts.size(); i++) {
				ps.push_back(cvPointTo32f(pts[i]));
			}
			return ps;
		}

		double pointDistance(const CvPoint2D32f& p1, const CvPoint2D32f& p2) {
			double dx = p1.x - p2.x;
			double dy = p1.y - p2.y;
			return sqrt(dx * dx + dy * dy);
		}

		double pointDistance(const CvPoint& p1, const CvPoint& p2) {
			int dx = p1.x - p2.x;
			int dy = p1.y - p2.y;
			return sqrt((double)(dx * dx + dy * dy));			
		}

		CvPoint3D32f scalePoint3D(CvPoint3D32f p3d, double v) {
			return cvPoint3D32f(p3d.x * v, p3d.y * v, p3d.z * v);
		}

	}
}
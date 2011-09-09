#include ".\cvmatutil.h"

namespace ferry {
	namespace cv_mat {

	void svd(CvMat* A, CvMat** W, CvMat** U, CvMat** V) {
		*U = cvCreateMat(A->rows, A->rows, A->type);
		*V = cvCreateMat(A->cols, A->cols, A->type);
		*W = cvCreateMat(A->rows, A->cols, A->type);
		
		cvSVD(A, *W, *U, *V);
	}

	CvMat* null(CvMat* A) {
		CvMat* X = cvCreateMat(A->cols, 1, A->type);
		
		int n = MIN(A->rows, A->cols);
		CvMat* u = cvCreateMat(A->rows, A->rows, A->type);
		CvMat* v = cvCreateMat(A->cols, A->cols, A->type);
		CvMat* w = cvCreateMat(A->rows, A->cols, A->type);
		
		cvSVD(A, w, u, v);

		for (int i = 0; i < v->rows; i++) X->data.db[i] = v->data.db[(i + 1)*v->cols - 1];

		cvReleaseMat(&w);
		cvReleaseMat(&u);
		cvReleaseMat(&v);

		return X;
	}

	CvMat* leftNull(CvMat* A) {
		CvMat* At = transpose(A);
		CvMat* n = null(At);
		cvReleaseMat(&At);
		return n;
	}

	CvMat* transpose(const CvMat* A) {
		CvMat* At = cvCreateMat(A->cols, A->rows, A->type);
		cvT(A, At);
		return At;
	}

	CvMat* inv(const CvMat* A) {
		CvMat* inv_A = cvCreateMat(A->cols, A->rows, A->type);
		cvInvert(A, inv_A);
		return inv_A;
	}

	CvMat* matMul(const CvMat* A, const CvMat* B) {
		assert(A->cols == B->rows);

		CvMat* M = cvCreateMat(A->rows, B->cols, A->type);
		cvMatMul(A, B, M);
		return M;
	}

	CvMat* matMul(const CvMat* A, const CvMat* B, const CvMat* C) {
		assert(A->cols == B->rows && B->cols == C->rows);

		CvMat* AB = matMul(A, B);
		CvMat* ABC = matMul(AB, C);

		cvReleaseMat(&AB);

		return ABC;
	}

	//diagonal
	CvMat* diagpseudoinverse(const CvMat* D) {
		//assert(D->rows == D->cols);

		CvMat* DI = cvCreateMat(D->cols, D->rows, D->type);
		cvZero(DI);
		int n = MIN(D->rows, D->cols);
		for (int i = 0; i < n; i++) {
			double d = cvmGet(D, i, i);
			if (abs(d) > 0.000001) cvmSet(DI, i, i, 1 /d );
		}

		return DI;
	}

	//return A+ such that AA+ = I
	//suppose A = UWV', then A+ = VW+U'
	CvMat* pseudoinverse(CvMat* A) {
		int n = MIN(A->rows, A->cols);
		CvMat* u = cvCreateMat(A->rows, n, A->type);
		CvMat* v = cvCreateMat(A->cols, A->cols, A->type);
		CvMat* w = cvCreateMat(n, A->cols, A->type);
		
		cvSVD(A, w, u, v);

		CvMat* AP = cvCreateMat(A->cols, A->rows, A->type);
		
		CvMat* wp = diagpseudoinverse(w);

		CvMat* vwp = cvCreateMat(v->rows, wp->cols, v->type);
		CvMat* ut = cvCreateMat(u->cols, u->rows, u->type);
		
		//==DEBUG
/*		cout<<"w: "<<matString(w)<<endl;
		cout<<"wp: "<<matString(wp)<<endl;
		CvMat* wwp = matMul(w, wp);
		cout<<"w * wp: "<<matString(wwp)<<endl;
		CvMat* vt = cvCreateMat(v->cols, v->rows, v->type);
		CvMat* vtv = cvCreateMat(v->cols, v->cols, v->type);
		cvT(v, vt);
		cvMatMul(vt, v, vtv);
		cout<<"vtv: "<<matString(vtv)<<endl;
		CvMat* ut2 = cvCreateMat(u->cols, u->rows, u->type);
		CvMat* uut = cvCreateMat(u->rows, u->rows, u->type);
		cvT(u, ut2);
		cvMatMul(u, ut2, uut);
		cout<<"uut: "<<matString(uut)<<endl;
		CvMat* A1 = matMul(u, w, vt);
		cout<<"A1: "<<matString(A1)<<endl;
		CvMat* AP1 = matMul(v, wp, ut2);
		cout<<"AP1: "<<matString(AP1)<<endl;
		CvMat* A1AP1 = matMul(A1, AP1);
		cout<<"A1 * AP1: "<<matString(A1AP1)<<endl;
*/
		//==

		cvMatMul(v, wp, vwp);
		cvTranspose(u, ut);
		cvMatMul(vwp, ut, AP);

		cvReleaseMat(&u);
		cvReleaseMat(&v);
		cvReleaseMat(&w);
		cvReleaseMat(&wp);
		cvReleaseMat(&vwp);
		cvReleaseMat(&ut);

		return AP;
	}

	// v: n x 1 matrix
	// v = v / ||v||
	void normalizeVector(CvMat* v) {
		double d = cvNorm(v);
		for (int i = 0; i < v->rows; i++) {
			v->data.db[i] /= d;
		}
	}

	// v: 3 x 1 matrix
	CvMat* getCrossMatrix(CvMat* v) {
		assert(v->rows == 3 && v->cols == 1);

		CvMat* ex = cvCreateMat(3, 3, v->type);
		cvZero(ex);
		cvmSet(ex, 0, 1, -cvmGet(v, 2, 0));
		cvmSet(ex, 0, 2,  cvmGet(v, 1, 0));
		cvmSet(ex, 1, 0,  cvmGet(v, 2, 0));
		cvmSet(ex, 1, 2, -cvmGet(v, 0, 0));
		cvmSet(ex, 2, 0, -cvmGet(v, 1, 0));
		cvmSet(ex, 2, 1, cvmGet(v, 0, 0));

		return ex;
	}

	void setSubRect(CvMat* dst, const CvMat* submat, const CvRect& rect) {
		assert(submat->width == rect.width && submat->height == rect.height);

		CvMat stub;
		cvGetSubRect(dst, &stub, rect);
		cvCopy(submat, &stub);
	}

	void copySubRect(const CvMat* src, CvMat* submat, const CvRect& rect) {
		assert(submat->width == rect.width && submat->height == rect.height);

		for (int i = 0; i < rect.height; i++) {
			for (int j = 0; j < rect.width; j++) {
				cvmSet(submat, i, j, cvmGet(src, rect.y + i, rect.x + j));
			}
		}
	}

	void copyCols(CvMat* dst, CvMat* submat, int start, int end) {
		copySubRect(dst, submat, cvRect(start, 0, end - start, dst->height));
	}

	void copyCol(CvMat* dst, CvMat* submat, int index) {
		copyCols(dst, submat, index, index + 1);
	}

	void setCols(CvMat* dst, const CvMat* submat, int start, int end) {
		setSubRect(dst, submat, cvRect(start, 0, end - start, dst->height));
	}

	void setCol(CvMat* dst, const CvMat* submat, int col) {
		setSubRect(dst, submat, cvRect(col, 0, 1, dst->height));
	}

	CvMat* getRows(const CvMat* src, int start, int end) {
		CvMat* submat = cvCreateMat(end - start, src->cols, src->type);
		copySubRect(src, submat, cvRect(0, start, src->cols, end - start));
		return submat;
	}

	CvMat* getRow(const CvMat* src, int index) {
		return getRows(src, index, index + 1);
	}

	void setRow(CvMat* dst, const CvMat* submat, int row) {
		setSubRect(dst, submat, cvRect(0, row, dst->width, 1));
	}

	CvMat* getCols(CvMat* src, int start, int end) {
		CvMat* submat = cvCreateMat(src->rows, end - start, src->type);
		copySubRect(src, submat, cvRect(start, 0, end - start, src->rows));
		return submat;
	}

	CvMat* getCol(CvMat* src, int index) {
		return getCols(src, index, index + 1);
	}

	//===== BEGIN ==== Transformation between Point and Matrix ==========================/

	//homogeneous vector <- point2d
	CvMat* hmatFromPoint2D(const CvPoint2D32f& p) {
		CvMat* m = cvCreateMat(3, 1, CV_64FC1);
		m->data.db[0] = p.x;
		m->data.db[1] = p.y;
		m->data.db[2] = 1.0;
		return m;
	}

	//homogeneous vector -> point2d
	CvPoint2D32f hmatToPoint2D(CvMat* m) {
		CvPoint2D32f p;
		p.x = (float)(m->data.db[0] / m->data.db[2]);
		p.y = (float)(m->data.db[1] / m->data.db[2]);
		return p;
	}

	//homogeneous vector <- point3d
	CvMat* hmatFromPoint3D(const CvPoint3D32f& p) {
		CvMat* m = cvCreateMat(4, 1, CV_64FC1);
		m->data.db[0] = p.x;
		m->data.db[1] = p.y;
		m->data.db[2] = p.z;
		m->data.db[3] = 1.0;
		return m;
	}

	//homogeneous vector -> point3d
	CvPoint3D32f hmatToPoint3D(CvMat* m) {
		CvPoint3D32f p;
		p.x = m->data.db[0] / m->data.db[3];
		p.y = m->data.db[1] / m->data.db[3];
		p.z = m->data.db[2] / m->data.db[3];
		return p;
	}

	//unhomogeneous vector <- point3d
	CvMat* matFromPoint3D(const CvPoint3D32f& p) {
		CvMat* m = cvCreateMat(3, 1, CV_64FC1);
		m->data.db[0] = p.x;
		m->data.db[1] = p.y;
		m->data.db[2] = p.z;
		return m;
	}

	//unhomogeneous vector -> point3d
	CvPoint3D32f matToPoint3D(CvMat* m) {
		CvPoint3D32f p;
		p.x = m->data.db[0];
		p.y = m->data.db[1];
		p.z = m->data.db[2];
		return p;
	}

	CvMat* hmatFromMat(const CvMat* m) {
		assert(m->cols == 1);

		CvMat* hm = cvCreateMat(m->rows + 1, m->cols, m->type);
		setSubRect(hm, m, cvRect(0, 0, m->width, m->height));
		cvmSet(hm, m->rows, 0, 1.0);
		return hm;
	}

	CvMat* hmatToMat(const CvMat* hm) {
		assert(hm->cols == 1);

		CvMat* m = cvCreateMat(hm->rows - 1, hm->cols, hm->type);
		double d = cvmGet(hm, hm->rows - 1, 0);
		for (int i = 0; i < m->rows; i++) {
			cvmSet(m, i, 0, cvmGet(hm, i, 0) / d);
		}
		return m;
	}


	//homogeneous vector <- point2d
	vector<CvMat*> vectorHmatFromPoint2D(const vector<CvPoint2D32f>& ps) {
		vector<CvMat*> mps;
		for (int i = 0; i < (int)ps.size(); i++) {
			mps.push_back(hmatFromPoint2D(ps[i]));
		}
		return mps;
	}

	//homogeneous vector -> point2d
	vector<CvPoint2D32f> vectorHmatToPoint2D(const vector<CvMat*>& mps) {
		vector<CvPoint2D32f> ps;
		for (int i = 0; i < (int)mps.size(); i++) {
			ps.push_back(hmatToPoint2D(mps[i]));
		}
		return ps;
	}

	//homogeneous vector <- point3d
	vector<CvMat*> vectorHmatFromPoint3D(const vector<CvPoint3D32f>& ps) {
		vector<CvMat*> mps;
		for (int i = 0; i < (int)ps.size(); i++) {
			mps.push_back(hmatFromPoint3D(ps[i]));
		}
		return mps;
	}

	//homogeneous vector -> point3d
	vector<CvPoint3D32f> vectorHmatToPoint3D(const vector<CvMat*>& mps) {
		vector<CvPoint3D32f> ps;
		for (int i = 0; i < (int)mps.size(); i++) {
			ps.push_back(hmatToPoint3D(mps[i]));
		}
		return ps;
	}

	vector<CvPoint3D32f> initXs(float pts[][3], int num) {
		vector<CvPoint3D32f> Xs;
		for (int i = 0; i < num; i++) {
			Xs.push_back(cvPoint3D32f(pts[i][0], pts[i][1], pts[i][2]));
		}
		return Xs;
	}

	//===== END ==== Transformation between Point and Matrix ==========================/

	string matString(const CvMat* M) {
		ostringstream oss;
		oss<<M->rows<<" x "<<M->cols<<endl;
		double* data = (double*)M->data.db;
		for (int i = 0; i < M->rows; i++) {
			for (int j = 0; j < M->cols; j++) {
				oss<<data[i * M->cols + j]<<" ";
			}
			oss<<endl;
		}
		return oss.str();
	}

	//assert M : 3 x 1
	void assertH3(const CvMat* M) {
		assert(M->rows == 3 && M->cols == 1);
	}

	//assert M : 4 x 1
	void assertH4(const CvMat* M) {
		assert(M->rows == 4 && M->cols == 1);
	}

	CvMat* getIdentity() {
		CvMat* I = cvCreateMat(3, 3, CV_64FC1);
		cvSetIdentity(I);
		return I;
	}

	CvMat* getZero(int rows, int cols) {
		CvMat* M = cvCreateMat(rows, cols, CV_64FC1);
		cvZero(M);
		return M;
	}

	CvMat* cloneMat(const CvMat* M) {
		CvMat* nM = cvCreateMat(M->rows, M->cols, M->type);
		cvCopy(M, nM);
		return nM;
	}

	CvMat* sub(const CvMat* A, const CvMat* B) {
		CvMat* C = cvCreateMat(A->rows, A->cols, A->type);
		cvSub(A, B, C);
		return C;
	}

	CvMat* scale(const CvMat* M, double s) {
		CvMat* nM = cvCreateMat(M->rows, M->cols, M->type);
		cvScale(M, nM, s);
		return nM;
	}

	void clearMat(CvMat** M) {
		if (*M != NULL) {
			cvReleaseMat(M);
			*M = NULL;
		}
	}

	}
}
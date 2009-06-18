#include "cvmat_utils.h"

#include <iostream>
#include <sstream>
#include <cassert>

using namespace std;

namespace fvision {
void svd(const CvMat* A, CvMat** W, CvMat** U, CvMat** V) {
	CvMat* cA = cvCloneMat(A);

	*U = cvCreateMat(A->rows, A->rows, A->type);
	*V = cvCreateMat(A->cols, A->cols, A->type);
	*W = cvCreateMat(A->rows, A->cols, A->type);
	
	cvSVD(cA, *W, *U, *V);

	cvReleaseMat(&cA);
}

CvMat* null(const CvMat* A, CvMat* X) {

	if (X == NULL) {
		X = cvCreateMat(A->cols, 1, A->type);
	} else {
		assert(A->cols == X->rows && X->cols == 1);
	}
	
	int n = MIN(A->rows, A->cols);
	CvMat* u = cvCreateMat(A->rows, A->rows, A->type);
	CvMat* v = cvCreateMat(A->cols, A->cols, A->type);
	CvMat* w = cvCreateMat(A->rows, A->cols, A->type);
	
	CvMat* cA = cvCloneMat(A);
	cvSVD(cA, w, u, v);
	cvReleaseMat(&cA);

	for (int i = 0; i < v->rows; i++) X->data.db[i] = v->data.db[(i + 1)*v->cols - 1];

	cvReleaseMat(&w);
	cvReleaseMat(&u);
	cvReleaseMat(&v);

	return X;
}

CvMat* leftNull(const CvMat* A, CvMat* X) {
	CvMat* At = transpose(A);
	CvMat* n = null(At, X);
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

void normalizeVector(const CvMat* src, CvMat* dst) {
	assert(dst->rows == src->rows && dst->cols == src->cols);
	double d = cvNorm(src);
	for (int i = 0; i < src->rows; i++) {
		cvmSet(dst, i, 0, cvmGet(src, i, 0) / d);
	}
}

void normalizeMatrix2(CvMat* m) {
	int sign = 1;
	if (cvmGet(m, 0, 0) < 0) sign = -1;
	double v = cvNorm(m) * sign;
	cvScale(m, m, 1/v);
}

void normalizeMatrix(CvMat* m) {
	double v = cvmGet(m, m->rows - 1, m->cols - 1);
	cvScale(m, m, 1 / v);
}

CvMat* getNormalizedMatrix(const CvMat* m) {
	CvMat* nm = cvCloneMat(m);
	normalizeMatrix(nm);
	return nm;
}

CvMat* getNormalizedMatrix2(const CvMat* m) {
	CvMat* nm = cvCloneMat(m);
	normalizeMatrix2(nm);
	return nm;
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

CvMat* getDiagMat(double a, double b, double c) {
	CvMat* m = getIdentity();
	cvmSet(m, 0, 0, a);
	cvmSet(m, 1, 1, b);
	cvmSet(m, 2, 2, c);
	return m;
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

void copyCols(const CvMat* src, CvMat* submat, int start, int end) {
	copySubRect(src, submat, cvRect(start, 0, end - start, src->height));
}

void copyCol(const CvMat* src, CvMat* submat, int index) {
	copyCols(src, submat, index, index + 1);
}

void setCols(CvMat* dst, const CvMat* submat, int start, int end) {
	setSubRect(dst, submat, cvRect(start, 0, end - start, dst->height));
}

void setCol(CvMat* dst, const CvMat* submat, int col) {
	setSubRect(dst, submat, cvRect(col, 0, 1, dst->height));
}

CvMat* getRows(const CvMat* src, int start, int end, CvMat* buf) {
	if (buf == NULL) {
		buf = cvCreateMat(end - start, src->cols, src->type);
	}
	copySubRect(src, buf, cvRect(0, start, src->cols, end - start));
	return buf;
}

CvMat* getRow(const CvMat* src, int index, CvMat* buf) {
	return getRows(src, index, index + 1, buf);
}

void setRow(CvMat* dst, const CvMat* submat, int row) {
	setSubRect(dst, submat, cvRect(0, row, dst->width, 1));
}

CvMat* getCols(const CvMat* src, int start, int end) {
	CvMat* submat = cvCreateMat(src->rows, end - start, src->type);
	copySubRect(src, submat, cvRect(start, 0, end - start, src->rows));
	return submat;
}

CvMat* getCol(const CvMat* src, int index) {
	return getCols(src, index, index + 1);
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

CvMat* getIdentity(int n) {
	CvMat* I = cvCreateMat(n, n, CV_64FC1);
	cvSetIdentity(I);
	return I;
}

CvMat* getZero(int rows, int cols) {
	CvMat* M = cvCreateMat(rows, cols, CV_64FC1);
	cvZero(M);
	return M;
}

CvMat* cloneMat(const CvMat* M) {
	if (M == NULL) return NULL;
	return cvCloneMat(M);
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

void clearMats(CvMat** M1, CvMat** M2, CvMat** M3, CvMat** M4, CvMat** M5) {
	if (M1 != NULL) clearMat(M1);
	if (M2 != NULL) clearMat(M2);
	if (M3 != NULL) clearMat(M3);
	if (M4 != NULL) clearMat(M4);
	if (M5 != NULL) clearMat(M5);
}

void clearMats(CvMat** M1, CvMat** M2, CvMat** M3, CvMat** M4, CvMat** M5, CvMat** M6, CvMat** M7, CvMat** M8, CvMat** M9, CvMat** M10) {
	if (M1 != NULL) clearMat(M1);
	if (M2 != NULL) clearMat(M2);
	if (M3 != NULL) clearMat(M3);
	if (M4 != NULL) clearMat(M4);
	if (M5 != NULL) clearMat(M5);
	if (M6 != NULL) clearMat(M6);
	if (M7 != NULL) clearMat(M7);
	if (M8 != NULL) clearMat(M8);
	if (M9 != NULL) clearMat(M9);
	if (M10 != NULL) clearMat(M10);
}

void clearMatVector(vector<CvMat*>& mats) {
	for (size_t i = 0; i < mats.size(); i++) {
		cvReleaseMat(&(mats[i]));
		mats[i] = NULL;
	}
}

CvMat* crossProduct(const CvMat* m1, CvMat* m2) {
	CvMat* cm = cvCreateMat(m1->rows, m1->cols, m1->type);
	cvCrossProduct(m1, m2, cm);
	return cm;
}

//=======================================================
CvMat* stackMatricesVertically(const vector<CvMat*>& mats) {
	if (mats.size() == 0) return NULL;
	int rows = mats[0]->rows;
	int cols = mats[0]->cols;
	int n = (int)mats.size();
	CvMat* vmat = cvCreateMat(rows * n, cols, mats[0]->type);
	for (int i = 0; i < n; i++) {
		setSubRect(vmat, mats[i], cvRect(0, i * rows, cols, rows));
	}
	return vmat;
}

CvMat* operator* (const CvMat& A, const CvMat& B) {
	return matMul(&A, &B);
}

CvMat* createMatWithValues(double a, double b, double c) {
	CvMat* m = cvCreateMat(3, 1, CV_64FC1);
	cvmSet(m, 0, 0, a);
	cvmSet(m, 1, 0, b);
	cvmSet(m, 2, 0, c);
	return m;
}

bool isMatHeaderEqual(const CvMat* m1, const CvMat* m2) {
	return (m1->rows == m2->rows && m1->cols == m2->cols && m1->type == m2->type);
}

}
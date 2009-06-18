#ifndef FVISION_CVMAT_UTILS_H_
#define FVISION_CVMAT_UTILS_H_

#include <vector>
#include <string>

#include "cv.h"
#include "CvMatWrapper.h"

// Matrix type should be CV_64FC1

namespace fvision {

//A = U*W*V'
void svd(const CvMat* A, CvMat** W, CvMat** U, CvMat** V);

/** AX = 0
 * 
 * @param X buffer to restore the result, if NULL, then create a new one
 */
CvMat* null(const CvMat* A, CvMat* X = NULL);

CvMat* leftNull(const CvMat* A, CvMat* X = NULL);

CvMat* transpose(const CvMat* A);

CvMat* inv(const CvMat* A);

CvMat* matMul(const CvMat* A, const CvMat* B);

CvMat* matMul(const CvMat* A, const CvMat* B, const CvMat* C);

CvMat* diagpseudoinverse(const CvMat* D);

CvMat* pseudoinverse(CvMat* A);

void normalizeVector(CvMat* v);

void normalizeVector(const CvMat* src, CvMat* dst);

//m' = m / norm of m
//set the first element to be positive
//change m
void normalizeMatrix2(CvMat* m);

//m' = m / last element of m
//change m
void normalizeMatrix(CvMat* m);

//m' = m / last element of m
//set the first element to be positive
//don't change m
CvMat* getNormalizedMatrix(const CvMat* m);

//m' = m / norm of m
//don't change m
CvMat* getNormalizedMatrix2(const CvMat* m);

CvMat* getCrossMatrix(CvMat* v);

void setSubRect(CvMat* dst, const CvMat* submat, const CvRect& rect);

/** Copy a part of src to submat
 *
 */
void copySubRect(const CvMat* src, CvMat* submat, const CvRect& rect);

void copyCols(const CvMat* src, CvMat* submat, int start, int end);

void copyCol(const CvMat* src, CvMat* submat, int index);

void setCols(CvMat* dst, const CvMat* submat, int start, int end);

void setCol(CvMat* dst, const CvMat* submat, int col);

CvMat* getRows(const CvMat* src, int start, int end, CvMat* buf = NULL);

CvMat* getRow(const CvMat* src, int index, CvMat* buf = NULL);

void setRow(CvMat* dst, const CvMat* submat, int row);

CvMat* getCols(const CvMat* src, int start, int end);

CvMat* getCol(const CvMat* src, int index);

CvMat* hmatFromMat(const CvMat* m);

CvMat* hmatToMat(const CvMat* hm);

std::vector<CvPoint3D32f> initXs(float pts[][3], int num);

std::string matString(const CvMat* M);

//assert M : 3 x 1
void assertH3(const CvMat* M);

//assert M : 4 x 1
void assertH4(const CvMat* M);

//3x3 identity matrix
CvMat* getIdentity();

//nxn identity matrix
CvMat* getIdentity(int n);

CvMat* createMatWithValues(double a, double b, double c);

CvMat* getDiagMat(double a, double b, double c);

CvMat* getZero(int rows, int cols);

CvMat* cloneMat(const CvMat* M);

CvMat* sub(const CvMat* A, const CvMat* B);

CvMat* scale(const CvMat* M, double s);

//m1 x m2
CvMat* crossProduct(const CvMat* m1, CvMat* m2);

//if (*M != NULL) release M;
void clearMat(CvMat** M);

void clearMats(CvMat** M1, CvMat** M2, CvMat** M3 = NULL, CvMat** M4 = NULL, CvMat** M5 = NULL);

void clearMats(CvMat** M1, CvMat** M2, CvMat** M3, CvMat** M4, CvMat** M5, CvMat** M6, CvMat** M7 = NULL, CvMat** M8 = NULL, CvMat** M9 = NULL, CvMat** M10 = NULL);

void clearMatVector(std::vector<CvMat*>& mats);

//================

CvMat* stackMatricesVertically(const std::vector<CvMat*>& mats);


//operator overloading
CvMat* operator* (const CvMat& A, const CvMat& B);

bool isMatHeaderEqual(const CvMat* m1, const CvMat* m2);

}
#endif // FVISION_CVMAT_UTILS_H_
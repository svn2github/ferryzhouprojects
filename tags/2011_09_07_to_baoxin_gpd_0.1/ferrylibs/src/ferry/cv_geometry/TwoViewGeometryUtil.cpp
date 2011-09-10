#include ".\twoviewgeometryutil.h"

#include "BasicUtil.h"

namespace ferry {
	namespace cv_geometry {
	//F = e'xP'P+
	CvMat* calcF(CvMat* P1, CvMat* P2) {
		
		CvMat* C = CvMatUtil::null(P1);
		cout<<"C "<<CvMatUtil::matString(C)<<endl;
		cout<<"P1 C: "<<CvMatUtil::matString(CvMatUtil::matMul(P1, C))<<endl;
		
		CvMat* e2 = CvMatUtil::matMul(P2, C);
		cout<<"e2: "<<CvMatUtil::matString(e2)<<endl;
		
		CvMat* e2x = CvMatUtil::getCrossMatrix(e2);
		CvMat* P1P = CvMatUtil::pseudoinverse(P1);

		CvMat* F = CvMatUtil::matMul(e2x, P2, P1P);

		cvReleaseMat(&C);
		cvReleaseMat(&e2);
		cvReleaseMat(&e2x);

		return F;
	}

	CvMat* transF(CvMat* F, CvMat* H1, CvMat* H2) 
	{
		CvMat* H2t = transpose(H2);
		CvMat* nF = matMul(H2t, F, H1);
		cvReleaseMat(&H2t);
		return nF;
	}

	CvMat* transFWithFlipY(CvMat* F, double h) 
	{
		double mh[] = {1, 0, 0, 0, -1, h - 1, 0, 0, 1};
		CvMat H = cvMat(3, 3, CV_64FC1, mh);
		return transF(F, &H, &H);
	}

	CvMat* transFWithScale(CvMat* F, double scale) 
	{
		double mh[] = {1/scale, 0, 0, 0, 1/scale, 0, 0, 0, 1};
		CvMat H = cvMat(3, 3, CV_64FC1, mh);
		return transF(F, &H, &H);
	}

	CvMat* transH(CvMat* H, CvMat* MH) {
		CvMat* inv_MH = cvCreateMat(3, 3, CV_64FC1);
		cvInvert(MH, inv_MH);
		CvMat* nH = matMul(inv_MH, H, MH);
		cvReleaseMat(&inv_MH);
		return nH;
	}

	CvMat* transHWithFlipY(CvMat* H, double h) {
		double mh[] = {1, 0, 0, 0, -1, h - 1, 0, 0, 1};
		CvMat MH = cvMat(3, 3, CV_64FC1, mh);
		return transH(H, &MH);
	}

	CvMat* transHWithScale(CvMat* H, double scale) {
		double mh[] = {1/scale, 0, 0, 0, 1/scale, 0, 0, 0, 1};
		CvMat MH = cvMat(3, 3, CV_64FC1, mh);
		return transH(H, &MH);
	}

	//H = [1, 0, 0; 0, -1, h - 1; 0, 0, 1]
	CvMat* getHWithFlipY(double h) {
		CvMat* H = cvCreateMat(3, 3, CV_64FC1);
		double mh[] = {1, 0, 0, 0, -1, h - 1, 0, 0, 1};
		cvSetData(H, mh, CV_AUTOSTEP);
		return H;
	}

	//H = [scale, 0, 0; 0, scale, 0; 0, 0, 1]
	CvMat* getHWithScale(double scale) {
		CvMat* H = cvCreateMat(3, 3, CV_64FC1);
		double mh[] = {scale, 0, 0, 0, scale, 0, 0, 0, 1};
		cvSetData(H, mh, CV_AUTOSTEP);
		return H;
	}

	CvMat* getHWithRotate(CvMat* K, double theta) {

		//float theta = 5 * 3.1415 / 180;
		double tanTheta = tan(theta);
		double mh[] = {1, 0, tanTheta,  0, 1, 0,  -tanTheta, 0, 1};
		CvMat MH = cvMat(3, 3, CV_64FC1, mh);

		CvMat* invK = inv(K);
		CvMat* H = matMul(K, &MH, inv(K));
		
		cvReleaseMat(&invK);
		return H;
	}

	CvMat* getHWithTranslation(double xv, double yv) {
		CvMat* H = cvCreateMat(3, 3, CV_64FC1);
		double mh[] = {1, 0, xv, 0, 1, yv, 0, 0, 1};
		cvSetData(H, mh, CV_AUTOSTEP);
		return H;
	}

	void randomlySampleNPointPairs(const vector<CvPoint2D32f>& x1s, const vector<CvPoint2D32f>& x2s, 
		vector<CvPoint2D32f>& sx1s, vector<CvPoint2D32f>& sx2s, int n) {
	
		vector<int> is = ranis((int)x1s.size(), n);
		for (int i = 0; i < n; i++) {
			int index = is[i];
			sx1s.push_back(x1s[index]);
			sx2s.push_back(x2s[index]);
		}
	}

	CvMat* getFM(CvMat* F, double a) {
		CvMat* e2 = leftNull(F);
		CvMat* e2x = getCrossMatrix(e2);
		CvMat* e2xF = matMul(e2x, F);

		double v[] = {a, 0, 0};
		CvMat V = cvMat(3, 1, CV_64FC1, v);
		CvMat* e2Vt = matMul(e2, transpose(&V));

		CvMat* M = cvCreateMat(3, 3, CV_64FC1);
		cvAdd(e2xF, e2Vt, M);

		//==DEBUG
		cout<<"F: "<<F<<endl;
		cout<<"e2xF: "<<e2xF<<endl;
		cout<<"e2xM: "<<matMul(e2x, M)<<endl;
		cout<<"M: "<<M<<endl;
		cout<<"det(e2xF): "<<cvDet(e2xF)<<endl;
		cout<<"det(M): "<<cvDet(M)<<endl;
		//==

		cvReleaseMat(&e2);
		cvReleaseMat(&e2x);
		cvReleaseMat(&e2xF);
		cvReleaseMat(&e2Vt);

		return M;
	}

	CvPoint2D32f transformPoint(CvMat* H, const CvPoint2D32f& p) {
		CvMat* pm = CvMatUtil::hmatFromPoint2D(p);
		CvMat* tpm = matMul(H, pm);
		CvPoint2D32f tp = CvMatUtil::hmatToPoint2D(tpm);
		cvReleaseMat(&pm);
		cvReleaseMat(&tpm);
		return tp;
	}

	vector<CvPoint2D32f> transformPoints(CvMat* H, const vector<CvPoint2D32f>& xs) {
		vector<CvPoint2D32f> nxs;
		for (int i = 0; i < (int)xs.size(); i++) {
			nxs.push_back(transformPoint(H, xs[i]));
		}
		return nxs;
	}

	double computeHomographyDistance(CvMat* H, const CvPoint2D32f& x1, const CvPoint2D32f& x2) {
		CvPoint2D32f hx1 = transformPoint(H, x1);
		return pointDistance(hx1, x2);
	}

	CvMat* estimateEFromFK(const CvMat* F, const CvMat* K1, const CvMat* K2) {
		CvMat* K2T = transpose(K2);
		CvMat* E = matMul(K2T, F, K1);
		cvReleaseMat(&K2T);
		return E;
	}

	CvMat* getStandardNP1() {
		CvMat* NP = cvCreateMat(3, 4, CV_64FC1);
		cvZero(NP);
		CvMat* I = getIdentity();
		setCols(NP, I, 0, 3);
		cvReleaseMat(&I);
		return NP;
	}

	CvMat* getP(const CvMat* K, const CvMat* R, const CvMat* t) {
		CvMat* NP = cvCreateMat(3, 4, CV_64FC1);
		setCols(NP, R, 0, 3);
		setCol(NP, t, 3);

		CvMat* P = matMul(K, NP);

		cvReleaseMat(&NP);
		
		return P;
	}

	//[f, 0, width/2; 0, -f, height/2; 0, 0, 1] f = alpha * width
	CvMat* getK(int width, int height, double alpha) {
		double f = alpha * width;
		CvMat* K = getIdentity();
		cvmSet(K, 0, 0, f);
		cvmSet(K, 1, 1, -f);
		cvmSet(K, 0, 2, (double)width / 2);
		cvmSet(K, 1, 2, (double)height / 2);
		return K;
	}


	}
}

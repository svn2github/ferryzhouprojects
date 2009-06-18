#include "cholesky.h"

namespace fvision {

	bool choleskyDecompose(CvMat* A, CvMat* R, bool isUpper) {
		
		cvZero(R);

		int n = A->rows;
		double aii;
		
		if (isUpper) {
			//compute the cholesky factorization A = U' * U

		} else {
			//compute the cholesky factorization A = L * L'
			//rii = sqrt(aii - sum(rki * rki, k = 1.. i-1))
			//rij = (aij - sum(rki * rkj, k = 1..i-1)) / rii
			for (int i = 0; i < n; i++) {

				//compute rii
				aii = cvmGet(A, i, i);
				for (int k = 0; k < i; k++) {
					aii -= cvmGet(R, k, i) * cvmGet(R, k, i);
				}
				if (aii < 0) return false;
				aii = sqrt(aii);
				cvmSet(R, i, i, aii);

				if (i == n - 1) return true;

				//compue rij
				for (int j = i + 1; j < n; j++) {
					double aij = cvmGet(A, i, j);
					for (int k = 0; k < i; k++) {
						aij -= cvmGet(R, k, i) * cvmGet(R, k, j);
					}
					aij /= aii;
					cvmSet(R, i, j, aij);
				}
			}
		}

		return true;
	}

}
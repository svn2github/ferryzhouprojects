#include ".\mathutil.h"

namespace ferry { namespace util {

double clip(double v, double minv, double maxv) {
	//return min(maxv, max(minv, v));
	if (v < minv) return minv;
	else if (v > maxv) return maxv;
	else return v;
}

	float ranf() {
		float d = (float) rand() / RAND_MAX;
		return d;
	}

	int rani(int max) {
		return (int)(ranf() * max - 0.5);
	}

	float gaussianRand(float m, float s)	/* normal random variate generator */
	{				        /* mean m, standard deviation s */
		float x1, x2, w, y1;
		static float y2;
		static int use_last = 0;

		if (use_last)		        /* use value from previous call */
		{
			y1 = y2;
			use_last = 0;
		}
		else
		{
			do {
				x1 = 2.0 * ranf() - 1.0;
				x2 = 2.0 * ranf() - 1.0;
				w = x1 * x1 + x2 * x2;
			} while ( w >= 1.0 );

			w = sqrt( (-2.0 * log( w ) ) / w );
			y1 = x1 * w;
			y2 = x2 * w;
			use_last = 1;
		}

		return( m + y1 * s );
	}

	vector<int> ranis(int max, int num) {
		assert(max > 0 && num <= max);
		vector<int> is;
		for (int i = 0; i < num; i++) {
			is.push_back(rani(max));
		}
		return is;
	}
}}
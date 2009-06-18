#include "fassert.h"

#include <iostream>
#include <cassert>

using namespace std;

namespace fvision {

	bool isImageHeaderEqual(const IplImage* im1, const IplImage* im2) {
		if (im1 == NULL || im2 == NULL) return false;
		return (im1->width     ==  im2->width
			 && im1->height    ==  im2->height
			 && im1->nChannels ==  im2->nChannels
			 && im1->depth     ==  im2->depth);
	}


	bool boolAssertNotNull(void* pointer, const char* message) {
		if (pointer == NULL) {
			cerr<<message<<endl;
			return false;
		}
		return true;
	}

	void exitIfNull(void* pointer, const std::string& message) {
		if (pointer == NULL) {
			cerr<<message<<endl;
			exit(1);
		}
	}

	void exitIfFalse(bool v, const char* message) {
		if (!v) {
			cerr<<message<<endl;
			exit(1);
		}
	}

	void exitIf(bool v, const std::string& message) {
		if (v) {
			cerr<<message<<endl;
			exit(1);
		}
	}
}
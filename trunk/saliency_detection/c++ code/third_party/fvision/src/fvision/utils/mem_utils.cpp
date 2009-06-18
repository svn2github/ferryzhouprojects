#include "mem_utils.h"

namespace fvision {

void fvision::releaseImageVector(std::vector<IplImage*>& images) {
	for (size_t i = 0; i < images.size(); i++) {
		cvReleaseImage(&images[i]);
		images[i] = NULL;
	}
}

}
#include "PointTracker.h"

namespace fvision {

PointTracker::PointTracker(void) {
	ptdb = new CvPoint2D32fDBImpl();
}

PointTracker::~PointTracker(void) {
	delete ptdb;
}

}
#ifndef FVISION_POINT_TRACKER_H_
#define FVISION_POINT_TRACKER_H_

#include <fvision/db.h>

#include <cv.h>

namespace fvision {

typedef MultipleFrameIndexedObjectsDatabase<CvPoint2D32f> CvPoint2D32fDB;
typedef MultipleFrameIndexedObjectsDatabaseMapImpl<CvPoint2D32f> CvPoint2D32fDBImpl;

/** Interface for tracking points in an image sequence
 * 
 * Input: an image sequence
 * Output: point tracking results. type: Point2D32fDB
 */
class PointTracker {
public:
	PointTracker(void);
	virtual ~PointTracker(void);

public:
	/** sub class need to implement the interface
	 * update the tracking results when each frame is added
	 */
	virtual void addFrame(int frameId, const IplImage* frame) = 0;

	const CvPoint2D32fDB* getTrackingResults() { return ptdb; }

protected:
	CvPoint2D32fDB* ptdb;

};

}
#endif // FVISION_POINT_TRACKER_H_
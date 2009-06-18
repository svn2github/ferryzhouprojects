#ifndef FVISION_IMIO_AVI_WRITER_H_
#define FVISION_IMIO_AVI_WRITER_H_

#include <cv.h>
#include <highgui.h>

namespace fvision {

class AVIWriter {
private:
	AVIWriter();

public:
	~AVIWriter(void);

	//return NULL if fail
	static AVIWriter* create(const char* path, int width, int height);

public:
	void writeFrame(const IplImage* frame);

private:
	//const char* path;
	CvVideoWriter* pvw;
};

}
#endif // FVISION_IMIO_AVI_WRITER_H_
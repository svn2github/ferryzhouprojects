#pragma once

#include <string>

#include <ferry/util/StringUtil.h>
#include <ferry/imutil/io/UnifiedImageSequenceReader.h>
#include <ferry/util/Observable.h>

using namespace std;

using namespace ferry::util;
using namespace ferry::imutil::io;

namespace ferry { namespace imutil { namespace convert {

class VideoConverter : public Observable
{
public:
	VideoConverter(void) {
		stop = false;
	}

	~VideoConverter(void) {
	}

public:
	void run(const char* src, const char* dst, int startFrame = 0, int endFrame = 0) {
		UnifiedImageSequenceReader* puisr = new UnifiedImageSequenceReader(src);
		if (puisr->isFailed()) throw "can't read source!";

		IplImage* frame = puisr->nextFrame();
		if (frame == NULL) return; //no data at all

		string dir, filenamestr, suffix;
		decomposePath(dst, dir, filenamestr, suffix);
		
		int type = 0; //dir. 1 -> avi

		CvVideoWriter* pvw = NULL;
		if (suffix.length() == 0) { //dir
			type = 0;
		} else if (strcmp(suffix.c_str(), "avi") == 0) {
			type = 1;
			pvw = cvCreateVideoWriter(dst, -1, 30, cvSize(frame->width, frame->height));
			if (!pvw) throw "can't write avi!";
		} else {
			throw "you can only specify a folder or an avi!";
		}

		char filebuf[300];
		int i = -1;
		do {
			i++;
			if (i < startFrame) continue;
			if (i > endFrame || stop) break;

			if (type == 0) {
				sprintf(filebuf, "%s/im_%05d.bmp", dst, i);
				cvSaveImage(filebuf, frame);
			} else if (type == 1) {
				cvWriteFrame(pvw, frame);
			}

			curIndex = i;
			notifyObservers();
		} while ((frame = puisr->nextFrame()) != NULL);

		puisr->close();
		delete puisr;
		if (pvw) cvReleaseVideoWriter(&pvw);
	}

	void setStop(bool b) {
		stop = b;
	}

	const char* getStatus() {
		return status;
	}

	int getCurIndex() {
		return curIndex;
	}

private:
	char imageSuffix[10]; //bmp or jpg

private:
	char status[200];
	bool stop;
	int curIndex;
};

} } }
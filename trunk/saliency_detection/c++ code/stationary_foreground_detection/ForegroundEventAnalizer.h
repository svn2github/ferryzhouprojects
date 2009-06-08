#pragma once

#include "PixelEventManager.h"
#include <cv.h>
#include <vector>
#include <iosfwd>

class ForegroundEventAnalizer {
public:
	ForegroundEventAnalizer(void);
	~ForegroundEventAnalizer(void);
public:
	void setStartIndex(int index);
	void setALLEventsOutputStream(std::ostream* os);
	void setSalientEventsOutputStream(std::ostream* os);

	void addFrame(const IplImage* binaryImage);

private:
	void initializePixelEventManagers();
	void processBinaryImage(const IplImage* binaryImage);

private:
	int startIndex;
	int curIndex;

	std::ostream* allEventsOutputStream;
	std::ostream* salientEventsOutputStream;

	int width;
	int height;
	std::vector<PixelEventManager*> pixelEventManagers;
};

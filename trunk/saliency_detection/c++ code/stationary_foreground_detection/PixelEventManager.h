#pragma once

#include "PixelEvent.h"
#include <cv.h>

//input 0 or 1 each time
//check whether last event is finished and can be output
class PixelEventManager {
public:
	//(x, y) is the pixel position
	PixelEventManager(int x, int y);
	~PixelEventManager(void);

public:
	void input(uchar v);

	bool isLastEventJustFinished() const;

	PixelEvent getLastEvent() const;

private:
	int x;
	int y;
	int curIndex;
	int lastStartIndex;
	int lastEndIndex;

	static const int tolerationTime = 2;

	//0 means beginning state
	int status;
	bool finishedSignal;

	PixelEvent lastEvent;
};


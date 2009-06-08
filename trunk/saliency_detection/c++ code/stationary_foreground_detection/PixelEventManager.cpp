#include "PixelEventManager.h"

PixelEventManager::PixelEventManager(int x, int y) {
	this->x = x;
	this->y = y;
	curIndex = -1;
	lastStartIndex = -1;
	lastEndIndex = -1;
	status = 0;
	finishedSignal = false;

	lastEvent.x = x;
	lastEvent.y = y;
}

PixelEventManager::~PixelEventManager(void) {
}

void PixelEventManager::input(uchar v) {
	if (v < 128) v = 0;
	curIndex++;
	if (status == 0) {
		if (finishedSignal) finishedSignal = false;
		if (v == 0) return;
		else {
			status = 1;
			lastStartIndex = curIndex;
		}
	} else if (status == 1) {
		if (v == 1) return;
		else {
			status = 2;
			lastEndIndex = curIndex;
		}
	} else if (status == 2) {
		if (v == 0) {
			if (curIndex - lastEndIndex <= tolerationTime) {//stay at the same status
				return;
			} else { //an event is finished
				finishedSignal = true;
				lastEvent.startIndex = lastStartIndex;
				lastEvent.endIndex = lastEndIndex;
				lastStartIndex = -1;
				lastEndIndex = -1;
				status = 0;
			}
		} else { 
			status = 1;
			return;
		}
	}
}

bool PixelEventManager::isLastEventJustFinished() const {
	return finishedSignal;
}

PixelEvent PixelEventManager::getLastEvent() const {
	return lastEvent;
}

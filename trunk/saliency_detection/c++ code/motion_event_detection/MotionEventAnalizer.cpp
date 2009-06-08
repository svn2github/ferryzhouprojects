#include "MotionEventAnalizer.h"

#include <cassert>
#include <iostream>

MotionEventAnalizer::MotionEventAnalizer(void) {
	curIndex = -1;
	globalStartIndex = 0;
	status = 0;
	this->finishedSignal = false;
}

MotionEventAnalizer::~MotionEventAnalizer(void) {
}

void MotionEventAnalizer::inputSignal(int v) {
	assert (v == 0 || v == 1 || v == -1);

	curIndex++;

	if (status == 0) {
		if (finishedSignal) finishedSignal = false;
		if (v == 0 || v == -1) return;
		else {
			status = 1;
			lastEvent.startIndex = curIndex;
		}
	} else if (status == 1) {
		if (v == 1) return;
		else if (v == 0 || v == -1) {
			finishedSignal = true;
			status = 0;
			lastEvent.endIndex = curIndex;
		}
	}
}

bool MotionEventAnalizer::isLastEventJustFinished() const {
	return finishedSignal;
}

MotionEvent MotionEventAnalizer::getLastEvent() const {
	return lastEvent;
}

std::ostream& operator<<(std::ostream& os, const MotionEvent& event) {
	os<<event.startIndex<<" "<<event.endIndex;	
	return os;
}
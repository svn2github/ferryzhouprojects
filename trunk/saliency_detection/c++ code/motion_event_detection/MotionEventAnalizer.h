#pragma once

#include <iosfwd>

class MotionEvent {
public:
	void shiftIndex(int offset) {
		startIndex += offset;
		endIndex += offset;
	}
public:
	int startIndex;
	int endIndex;
};

std::ostream& operator<< (std::ostream& os, const MotionEvent& event);

//for each binary image
//if event is ready
//output event
class MotionEventAnalizer {
public:
	MotionEventAnalizer(void);
	~MotionEventAnalizer(void);
public:
	void inputSignal(int v);

	bool isLastEventJustFinished() const;

	MotionEvent getLastEvent() const;

private:
	int globalStartIndex;
	int curIndex;

	//0 means beginning state
	int status;
	bool finishedSignal;

	MotionEvent lastEvent;
};

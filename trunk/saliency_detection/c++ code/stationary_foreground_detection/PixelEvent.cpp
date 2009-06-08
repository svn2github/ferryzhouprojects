#include "PixelEvent.h"

#include <iostream>
using namespace std;

int PixelEvent::salientLength = 100;

PixelEvent::PixelEvent(void)
{
}

PixelEvent::~PixelEvent(void)
{
}

bool PixelEvent::isSalient() const {
	return (endIndex - startIndex) >= salientLength;
}

void PixelEvent::shiftIndex(int offset) {
	startIndex += offset;
	endIndex += offset;
}

ostream& operator<<(ostream& os, const PixelEvent& event) {
	os<<event.x<<" "<<event.y<<" "<<event.startIndex<<" "<<event.endIndex;
	return os;
}
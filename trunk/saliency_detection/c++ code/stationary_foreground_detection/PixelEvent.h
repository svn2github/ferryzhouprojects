#pragma once

#include <iosfwd>

class PixelEvent {
public:
	PixelEvent(void);
	~PixelEvent(void);

public:
	bool isSalient() const;
	void shiftIndex(int offset);

public:
	int x;
	int y;
	int startIndex;
	int endIndex;

	static int salientLength;
};

std::ostream& operator<< (std::ostream& os, const PixelEvent& event);
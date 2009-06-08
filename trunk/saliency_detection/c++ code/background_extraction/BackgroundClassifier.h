#pragma once

class BackgroundClassifier {
public:
	BackgroundClassifier(void);
	~BackgroundClassifier(void);
public:
	void testFrame(const IplImage* frame, bool* results);
};

#pragma once

#include <cv.h>

//input path regex
//start index
//end index
class IndexedImageSequenceReader {
public:
	IndexedImageSequenceReader(void);
	~IndexedImageSequenceReader(void);

private:
	IplImage* image;
};

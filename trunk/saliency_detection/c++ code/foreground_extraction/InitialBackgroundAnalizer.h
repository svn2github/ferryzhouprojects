#ifndef INITIAL_BACKGROUND_ANALIZER_H_
#define INITIAL_BACKGROUND_ANALIZER_H_

#include <cv.h>

//input a set of frames
//output a background image and associated standard deviation
class InitialBackgroundAnalizer {
public:
	virtual ~InitialBackgroundAnalizer(void) {}
	virtual void addTrainingFrame(const IplImage* image) = 0;

	//return a color image if the input is a color image
	//this is responsible for deleting the output
	virtual const IplImage* getBackgroundImage() = 0;
};

#endif // INITIAL_BACKGROUND_ANALIZER_H_

#pragma once

#include "ImageInputHandler.h"
#include "ImageOutputHandler.h"

//
class ForegroundSaliencyAnalizer {
public:
	ForegroundSaliencyAnalizer(void);
	~ForegroundSaliencyAnalizer(void);
public:
	void setImageInputHandler(ImageInputHandler* imageInputHandler);

private:
	
};

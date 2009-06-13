#include "Test_InitialBackgroundAnalizer.h"

#include "ImageInputHandlerFactory.h"

#include "InitialBackgroundAnalizerClusterImpl.h"

#include <highgui.h>

#include <iostream>

using namespace image_io;
using namespace std;

Test_InitialBackgroundAnalizer::Test_InitialBackgroundAnalizer(void) {
}

Test_InitialBackgroundAnalizer::~Test_InitialBackgroundAnalizer(void) {
}

void Test_InitialBackgroundAnalizer::test() {

	InitialBackgroundAnalizer* analizer = new InitialBackgroundAnalizerClusterImpl();

	const char* pathRegex = "H:\\zj\\data\\surveillance\\accident5\\im_%04d.jpg";
	int startIndex = 0;
	int endIndex = 200;

	ImageInputHandler* inputHandler = ImageInputHandlerFactory().createPathRegexImageInputHandler(pathRegex, startIndex, endIndex);

	IplImage* image = NULL;

	int i = 0;
	while ((image = inputHandler->read()) != NULL) {
		analizer->addTrainingFrame(image);
		if (i % 10 == 0) cout<<"processing "<<i<<" frames............."<<endl;
		i++;
	}

	const IplImage* backgroundImage = analizer->getBackgroundImage();

	cvNamedWindow("background");
	cvShowImage("background", backgroundImage);
}
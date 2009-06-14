#include <image_io.h>
#include <image_filter.h>

#include <highgui.h>

using namespace image_io;
using namespace image_filter;

void main() {

	const char* pathRegex = "H:\\zj\\data\\surveillance\\PETS2000\\%04d.jpg";
	int startIndex = 0;
	int endIndex = 100;

	ImageInputHandler* rawInputHandler = ImageInputHandlerFactory().createPathRegexImageInputHandler(pathRegex, startIndex, endIndex);

	ImageFilter* filter = ResizeImageFilter::createWithNewRatio(0.5);

	ImageInputHandler* inputHandler = new FilteredImageInputHandler(rawInputHandler, filter);


	ImageOutputHandler* outputHandler1 = ImageOutputHandlerFactory().createWindowImageOutputHandler("dst");

	ImageFilter* outputFilter = ResizeImageFilter::createWithNewRatio(0.5);

	ImageOutputHandler* outputHandler2 = new FilteredImageOutputHandler(ImageOutputHandlerFactory().createWindowImageOutputHandler("dst2"), outputFilter);

	SequenceImageOutputHandler* outputHandler = new SequenceImageOutputHandler();
	outputHandler->addImageOutputHandler(outputHandler1);
	outputHandler->addImageOutputHandler(outputHandler2);

	run(inputHandler, outputHandler, 30);

	delete inputHandler;
	delete outputHandler;

	cvWaitKey(0);
}
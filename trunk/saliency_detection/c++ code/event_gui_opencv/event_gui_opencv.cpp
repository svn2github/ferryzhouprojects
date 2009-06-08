//input original image path regex
//input salience image path regex
//input events file
//motion events are labeled with light green
//stationary events are labeled with red
//background is black

static char usage[] = 
"\n event_gui_opencv version 0.1 \n\
\n\
usage: %s -i <original_image_path_regex> -r <raw_saliency_path_regex> -b <binary_saliency_path_regex> -e <event_direcotry> [options] \n\
\n\
input image sequences path regex\n\
show saliency gui\n\
\n\
options:\n\
-S <start_index> start index\n\
-E <end_index>   original image end index\n\
-q               quiet (turn off debugging output)\n\
\
Example:\n\
";

#include <IndexRange.h>

#include <fvision/utils.h>

#include "XGetopt.h"

#include <string>
#include <vector>
#include <sstream>

//-i 
char* originalImagePathRegex = NULL;
//-r
char* rawSaliencyImagePathRegex = NULL;
//-b
char* binarySaliencyImagePathRegex = NULL;
//-e
char* eventDirectory = NULL;
//-S
int imageStartIndex = 0;
//-E
int imageEndIndex = 1000;
//-q
bool verbose = true;

using namespace fvision;
using namespace std;

void processCommandLine(int argc, char* argv[]) {
	int c = 0;
	while ((c = getopt(argc, argv, "i:r:b:e:S:E:q")) != EOF) {
		switch (c) {
			case 'i': originalImagePathRegex = argv[optind - 1]; break;
			case 'r': rawSaliencyImagePathRegex = argv[optind - 1]; break;
			case 'b': binarySaliencyImagePathRegex = argv[optind - 1]; break;
			case 'e': eventDirectory = argv[optind - 1]; break;
			case 'S': imageStartIndex = atoi(argv[optind - 1]); break;
			case 'E': imageEndIndex = atoi(argv[optind - 1]); break;
			case 'q': verbose = false; break;
			default:
				cerr<<"Ignoring unrecognized option "<<argv[optind-1]<<endl;
		}
	}
	if (optind != argc) {
		printf(usage, argv[0]);
		exit(1);
	}
}

void checkInput() {
	fvision::exitIfNull(originalImagePathRegex, "please input original image path regex with -i <regex>!");
	//fvision::exitIfNull(rawSaliencyImagePathRegex, "please input rawSaliencyImagePathRegex with -r <regex>!");
	//fvision::exitIfNull(binarySaliencyImagePathRegex, "please input binarySaliencyImagePathRegex with -b <regex>!");
	fvision::exitIfNull(eventDirectory, "please input eventDirectory with -e <dir>!");
	if (imageStartIndex < 0) {
		cout<<"start index should be a positive number!"<<endl;
		exit(1);
	}
	if (imageEndIndex < 0) {
		cout<<"end index should be a positive number!"<<endl;
		exit(1);
	}
}

void setParameters() {
	originalImagePathRegex = "H:\\zj\\data\\surveillance\\caviar1\\Meet_Crowd%03d.jpg";
	binarySaliencyImagePathRegex = "H:\\zj\\data\\surveillance\\results\\caviar1\\foreground2\\raw_binary\\im_%05d.jpg";
	rawSaliencyImagePathRegex = "H:\\zj\\data\\surveillance\\results\\caviar1\\foreground2\\raw_distances\\im_%05d.jpg";
	eventDirectory = "H:\\zj\\data\\surveillance\\results\\caviar1\\foreground2\\events\\";
}

//event data
std::vector<IndexRange> motionEvents(0);
//std::vector<IndexRange> stationaryForegroundEvents;

const int PATH_SIZE = 500;
char originalImagePath[PATH_SIZE];
char rawSaliencyImagePath[PATH_SIZE];
char binarySaliencyImagePath[PATH_SIZE];

//overlay of original and saliency
IplImage* mainImage;
IplImage* mainWindowSizeBuffer;

CvSize mainImageSize;
CvSize originalImageSize;

int curFrameIndex = 0;
int* frameSaliency;

char* mainWindowName = "main";
char* saliencyBarWindowName = "saliency index";
int frameLength;
double lengthRatio; // = barWidth / frameWidth; frameWidth = (imageEndIndex - imageStartIndex + 1)
CvSize saliencyBarSize;
IplImage* saliencyBarImage;
IplImage* saliencyWindowImage;
int barIndex = 0;    //barIndex = (curIndex - imageStartIndex) * lengthRatio. curIndex = barIndex / lengthRatio + imageStartIndex
int* barSaliency; //have same width with bar

int frameIndexToBarIndex(int frameIndex) {
	return (int)(frameIndex * lengthRatio);
}

int frameIndexFromBarIndex(int barIndex) {
	return (int)(barIndex / lengthRatio);
}

void updateSaliencyBarWindow() {
	cvCopy(saliencyBarImage, saliencyWindowImage);
	cvLine(saliencyWindowImage, cvPoint(barIndex, 0), cvPoint(barIndex, saliencyBarSize.height - 1), CV_RGB(255, 0, 0));
	cvShowImage(saliencyBarWindowName, saliencyWindowImage);
}

void updateMainWindow() {
	//load original image
	sprintf_s(originalImagePath, PATH_SIZE, originalImagePathRegex, curFrameIndex);
	IplImage* src = cvLoadImage(originalImagePath, 1);
	if (src == NULL) {
		cout<<"can not load image: "<<originalImagePath<<" !"<<endl;
		return;
	}

	showImage("original", src);

	if (mainImage == NULL) {
		mainImage = cvCloneImage(src);
		mainWindowSizeBuffer = cvCloneImage(src);
	}

	if (binarySaliencyImagePathRegex != NULL) {
		sprintf_s(binarySaliencyImagePath, PATH_SIZE, binarySaliencyImagePathRegex, curFrameIndex);
		IplImage* binarySaliencyImage = cvLoadImage(binarySaliencyImagePath, 1);
		if (binarySaliencyImage->width != mainImage->width) {
			cvResize(binarySaliencyImage, mainWindowSizeBuffer);
		} else {
			cvCopy(binarySaliencyImage, mainWindowSizeBuffer);
		}
		cvAddWeighted(src, 0.7, mainWindowSizeBuffer, 0.4, 0, mainImage);
		cvReleaseImage(&binarySaliencyImage);
		showImage("saliency", mainWindowSizeBuffer);
	}

	showImage(mainWindowName, mainImage);
	cvReleaseImage(&src);
}

//set curIndex
//update bar image
//update main window
void onSaliencyBarMouse(int event, int x, int y, int flags, void* param) {
	if (event == CV_EVENT_LBUTTONUP) {
		barIndex = x;
		curFrameIndex = frameIndexFromBarIndex(barIndex);
		updateSaliencyBarWindow();
		updateMainWindow();
	}	
}

void loadMotionEvents() {
	string motionEventFile = string(eventDirectory) + "motion_events.txt";
	fvision::loadVWithNoNumber(motionEventFile, motionEvents);
}

void assignSaliency(const std::vector<IndexRange>& ranges, int* frameSaliency, int* barSaliency) {
	for (size_t i = 0; i < ranges.size(); i++) {
		IndexRange range = ranges[i];
		range.shiftIndex(-imageStartIndex);
		for (int j = range.startIndex; j < range.endIndex; j++) {
			if (j >= frameLength) break;
			frameSaliency[j] = 1;
			barSaliency[frameIndexToBarIndex(j)] = 1;
		}
	}
}

void drawSaliencyBarImage(IplImage* im, const int* barSaliency) {
	for (int i = 0; i < saliencyBarSize.width; i++) {
		if (barSaliency[i] == 1) {
			cvLine(im, cvPoint(i, 0), cvPoint(i, saliencyBarSize.height - 1), CV_RGB(0, 255, 0));
		}
	}
}
//retrieve event files
//initialize saliency data
//retrieve the first original image
//set windowsize
void initialize() {

	saliencyBarSize = cvSize(500, 30);
	saliencyBarImage = cvCreateImage(saliencyBarSize, 8, 3);
	cvZero(saliencyBarImage);

	frameLength = imageEndIndex - imageStartIndex + 1;
	lengthRatio = (double)saliencyBarSize.width / frameLength;

	//initializeEventData();
	loadMotionEvents();

	frameSaliency = new int[frameLength];
	barSaliency = new int[saliencyBarSize.width];
	memset(frameSaliency, 0, sizeof(int) * frameLength);
	memset(barSaliency, 0, sizeof(int) * saliencyBarSize.width);

	assignSaliency(motionEvents, frameSaliency, barSaliency);

	drawSaliencyBarImage(saliencyBarImage, barSaliency);

	saliencyWindowImage = cvCloneImage(saliencyBarImage);

	cvNamedWindow(saliencyBarWindowName, 1);
	cvSetMouseCallback(saliencyBarWindowName, onSaliencyBarMouse, NULL);
	cvShowImage(saliencyBarWindowName, saliencyWindowImage);
}

void main(int argc, char* argv[]) {

	processCommandLine(argc, argv);
	//setParameters();
	checkInput();

	initialize();

	cvWaitKey(0);
}




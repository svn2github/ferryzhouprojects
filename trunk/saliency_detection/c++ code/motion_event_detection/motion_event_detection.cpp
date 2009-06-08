//input a binary image sequence
//output a file containts the motion events
//
//event definition is simple: if the image contains non zero pixels, then it's an event
//we use range to represent event
//
// two status
// S0 -(1)-> S(1) set startIndex
// S0 -(0)-> S(0) 
// S0 -(-1)-> S(0)
// S1 -(0)-> S(0) set endIndex, output
// S1 -(1)-> S(1)
// S1 -(-1)-> S(1) set endIndex, output
//-1 means finished

#include "MotionEventAnalizer.h"
#include <cv.h>

static char usage[] = 
"\n motion_event_detection version 0.1 \n\
\n\
usage: %s -i <input_path_regex> -E <end_index> -o <output_path_prefex> -m <method> [options] \n\
<method> = \"<salient_event_threshold>\"\n\
\n\
reads an binary image sequence\n\
output motion events: \n\
1) motion_events.txt  \n\
\n\
options:\n\
-S <start_index> default 0\n\
-I <intervel>    default 1\n\
-q               quiet (turn off debugging output)\n\
\
Example:\n\
";

#include <fvision/utils.h>

#include "XGetopt.h"

#include <string>
#include <vector>
#include <sstream>

//-i 
char* inputPathRegex = NULL;
//-o
char* outputDirectory = NULL;
//-S
int startIndex = 0;
//-I
int interval = 1;
//-E
int endIndex = 100000;
//-m
char* methodString = NULL;
//-q
bool verbose = true;

using namespace fvision;
using namespace std;

void processCommandLine(int argc, char* argv[]) {
	int c = 0;
	while ((c = getopt(argc, argv, "i:o:S:I:E:m:q")) != EOF) {
		switch (c) {
			case 'i': inputPathRegex = argv[optind - 1]; break;
			case 'o': outputDirectory = argv[optind - 1]; break;
			case 'S': startIndex = atoi(argv[optind - 1]); break;
			case 'I': interval = atoi(argv[optind - 1]); break;
			case 'E': endIndex = atoi(argv[optind - 1]); break;
			case 'm': methodString = argv[optind - 1]; break;
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
	fvision::exitIfNull(inputPathRegex, "please input input path regex with -i <regex>!");
	fvision::exitIfNull(outputDirectory, "please input output path with -i <path>!");
	fvision::exitIfNull(methodString, "please input method with -m <method>!");
}

void setParameters() {
	inputPathRegex = "H:\\zj\\data\\surveillance\\results\\caviar1\\foreground2\\raw_binary\\im_%05d.jpg";
	outputDirectory = "H:\\zj\\data\\surveillance\\results\\caviar1\\foreground2\\events\\";
	methodString = "5000";
}

int salientEventThreshold = 100;

void parseMethod(const char* methodString) {
	istringstream iss(methodString);
	iss>>salientEventThreshold;	
	if (salientEventThreshold < 0) {
		cout<<"the salientEventThreshold should be positive number!"<<endl;
		exit(1);
	}
}

int getSignal(const IplImage* binaryImage) {

	assert (binaryImage != NULL);

	CvScalar sumScalar = cvSum(binaryImage);

	double sum = sumScalar.val[0];

	int v = 0; //isEvent
	if (sum > salientEventThreshold) v = 1;

	return v;
}

void main(int argc, char* argv[]) {

	processCommandLine(argc, argv);
	//setParameters();
	checkInput();
	parseMethod(methodString);

	MotionEventAnalizer analizer;

	const int PATH_SIZE = 300;
	char inputPath[PATH_SIZE];
	char outputPath[PATH_SIZE];

	sprintf_s(outputPath, PATH_SIZE, "%smotion_events.txt", outputDirectory);
	ofstream motionEventOutputStream(outputPath);
	if (motionEventOutputStream.fail()) {
		cout<<"create file failed: "<<outputPath<<endl;
		exit(1);
	}
	for (int i = startIndex; i < endIndex; i++) {
		sprintf_s(inputPath, PATH_SIZE, inputPathRegex, i);
		IplImage* src = cvLoadImage(inputPath, 0);
		int v = 0;
		if (src == NULL) {
			cout<<"finished at "<<i<<" th frame"<<endl;
			v = -1;
		} else {
			v = getSignal(src);
		}
		analizer.inputSignal(v);
		if (analizer.isLastEventJustFinished()) {
			MotionEvent event = analizer.getLastEvent();
			event.shiftIndex(startIndex);
			motionEventOutputStream<<event<<endl;
		}

		if (src == NULL) break;

		if (i % 10 == 0) cout<<"processing "<<i<<" ..........."<<endl;
		cvReleaseImage(&src);
	}

	cvWaitKey(0);
}


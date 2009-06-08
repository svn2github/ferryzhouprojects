//input a set of binary grid images
//255 indicate foreground
//
//aggregate events for every pixel
//classify events based on time
//
//each pixel corresponds to an event list
//
//streaming events maybe necessary
//i.e. at any time, each pixel only maintain one event
//old events will be stored
//salient events will also be stored
//
//two output stream handle
//

static char usage[] = 
"\n stationary_foreground_detection version 0.1 \n\
\n\
usage: %s -i <input_path_regex> -E <end_index> -o <output_path> -m <method> [options] \n\
<method> = \"<salient_event_length>\"\n\
\n\
reads an binary image sequence\n\
output all pixel events as well as salient events: \n\
1) all_events.txt  \n\
2) salient_events.txt \n\
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

#include "ForegroundEventAnalizer.h"

void checkInput() {
	fvision::exitIfNull(inputPathRegex, "please input input path regex with -i <regex>!");
	fvision::exitIfNull(outputDirectory, "please input output path with -i <path>!");
	fvision::exitIfNull(methodString, "please input method with -m <method>!");
}

void setParameters() {
	inputPathRegex = "H:\\zj\\data\\surveillance\\results\\caviar1\\foreground2\\raw_binary\\im_%05d.jpg";
	outputDirectory = "H:\\zj\\data\\surveillance\\results\\caviar1\\foreground2\\events\\";
	methodString = "100";
}

int salientEventLength = 100;

void parseMethod(const char* methodString) {
	istringstream iss(methodString);
	iss>>salientEventLength;	
	if (salientEventLength < 0) {
		cout<<"the salientEventLength should be positive number!"<<endl;
		exit(1);
	}
}

void main(int argc, char* argv[]) {

	processCommandLine(argc, argv);
	//setParameters();
	checkInput();
	parseMethod(methodString);

	ForegroundEventAnalizer analizer;
	PixelEvent::salientLength = salientEventLength;

	const int PATH_SIZE = 300;
	char inputPath[PATH_SIZE];
	char outputPath[PATH_SIZE];

	analizer.setStartIndex(startIndex);

	sprintf_s(outputPath, PATH_SIZE, "%sall_pixel_events.txt", outputDirectory);
	ofstream allEventOutputStream(outputPath);
	if (allEventOutputStream.fail()) {
		cout<<"create file failed: "<<outputPath<<endl;
		exit(1);
	}
	analizer.setALLEventsOutputStream(&allEventOutputStream);

	sprintf_s(outputPath, PATH_SIZE, "%ssalient_pixel_events.txt", outputDirectory);
	ofstream salientEventOutputStream(outputPath);
	if (salientEventOutputStream.fail()) {
		cout<<"create file failed: "<<outputPath<<endl;
		exit(1);
	}
	analizer.setSalientEventsOutputStream(&salientEventOutputStream);

	for (int i = startIndex; i < endIndex; i++) {
		sprintf_s(inputPath, PATH_SIZE, inputPathRegex, i);
		IplImage* src = cvLoadImage(inputPath, 0);
		if (src == NULL) {
			cout<<"finished at "<<i<<" th frame"<<endl;
			break;
		}
		analizer.addFrame(src);

		if (i % 10 == 0) cout<<"processing "<<i<<" ..........."<<endl;
		cvReleaseImage(&src);
	}

	cvWaitKey(0);
}


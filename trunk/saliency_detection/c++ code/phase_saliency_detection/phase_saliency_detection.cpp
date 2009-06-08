//phase based method
//f = fft(im)
//p = angle(f)
//s = g * ||ifft(e(ip))||^2

static char usage[] = 
"\n phase_saliency_detection version 0.1 \n\
\n\
usage: %s -i <input_path_regex> -E <end_index> -o <output_path> -m <method> [options] \n\
<method> = \"<image_size_division_factor> <segment_length> <motion_step> <final_smooth_size>\"\n\
\n\
reads an image sequence\n\
output several image sequences: \n\
1) raw saliency image (gray scale, grid size) \n\
\n\
options:\n\
-S <start_index> default 0\n\
-I <intervel>    default 1\n\
-n <num>         index regex string (\"%0<n>d\") (default 5)\n\
-d               draw converted images\n\
-q               quiet (turn off debugging output)\n\
\
Example:\n\
";

#include <fvision/utils.h>

#include <XGetopt.h>

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
int endIndex = 1e+6;
//-n
int indexStringSize = 5;
//-m
char* methodString = NULL;
//-d
bool drawImage = false;
//-q
bool verbose = true;

using namespace fvision;
using namespace std;

void processCommandLine(int argc, char* argv[]) {
	int c = 0;
	while ((c = getopt(argc, argv, "i:o:S:I:E:n:m:dq")) != EOF) {
		switch (c) {
			case 'i': inputPathRegex = argv[optind - 1]; break;
			case 'o': outputDirectory = argv[optind - 1]; break;
			case 'S': startIndex = atoi(argv[optind - 1]); break;
			case 'I': interval = atoi(argv[optind - 1]); break;
			case 'E': endIndex = atoi(argv[optind - 1]); break;
			case 'n': indexStringSize = atoi(argv[optind - 1]); break;
			case 'm': methodString = argv[optind - 1]; break;
			case 'd': drawImage = true; break;
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
	if (indexStringSize < 1 || indexStringSize > 9) {
		cout<<"num should be in [1, 9]!"<<endl;
		exit(1);
	}
}

void setParameters() {
	inputPathRegex = "H:\\zj\\data\\surveillance\\pets2000\\%04d.jpg";
	outputDirectory = "H:\\zj\\data\\surveillance\\results\\phase\\pets2000\\";
	methodString = "8 200 2 7";
	indexStringSize = 5;
}

double imageSizeDivisionFactor = 8;
int segmentLength = 300;
int motionStep = 2;
int finalSmoothSize = 7;

bool isNumberAPositiveOdd(int n) {
	return (n > 0) && (n % 2 == 1);
}

void parseMethod(const char* methodString) {
	istringstream iss(methodString);
	iss>>imageSizeDivisionFactor>>segmentLength>>motionStep>>finalSmoothSize;
	if (!isNumberAPositiveOdd(finalSmoothSize)) {
		cout<<"the smooth kernel size should be a positive odd number!"<<endl;
		exit(1);
	}
	if (segmentLength < 1) {
		cout<<"segmentLength should positive"<<endl;
		exit(1);
	}
}

#include "Test_PhaseSaliencyCalculator.h"
#include "PhaseSaliencyAnalizer.h"

void main(int argc, char* argv[]) {

	//Test_PhaseSaliencyCalculator().test2d();
	//Test_PhaseSaliencyCalculator().test3d();

	processCommandLine(argc, argv);
	//setParameters();
	checkInput();
	parseMethod(methodString);

	PhaseSaliencyAnalizer analizer;
	analizer.setInput(inputPathRegex, startIndex, endIndex);
	analizer.setOutput(outputDirectory, indexStringSize);

	analizer.setImageSizeScalingFactor(imageSizeDivisionFactor);
	analizer.setSegmentSize(segmentLength);
	analizer.setMotionStep(motionStep);
	analizer.setFinalSmoothKernelSize(finalSmoothSize);


	analizer.run();

	cvWaitKey(0);
}


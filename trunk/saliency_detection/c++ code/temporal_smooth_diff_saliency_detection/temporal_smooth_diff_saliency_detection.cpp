//input original image sequence; pathRegex, startIndex, endIndex
//output the corresponding saliency images
//
//pipelines
//resize -> temporal smooth -> smooth diff -> smooth (smooth diff) -> output
//

static char usage[] = 
"\n background_extraction version 0.1 \n\
\n\
usage: %s -i <input_path_regex> -E <end_index> -o <output_path> -m <method> [options] \n\
<method> = \"<kernel_size> <image_size_division_factor> <saliency_value_scaling_factor> <final_smooth>\"\n\
<final_smooth> = on | off\n\
\n\
reads an image sequence\n\
output several image sequences: \n\
1) raw saliency image (gray scale, grid size) \n\
\n\
options:\n\
-S <start_index> default 0\n\
-I <intervel>    default 1\n\
-n <num>         index regex string (\"%0<n>d\")\n\
-d               draw converted images\n\
-q               quiet (turn off debugging output)\n\
\
Example:\n\
";

#include "VideoTemporalSaliencyAnalizer.h"
#include "GeneralKernel1D.h"


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
	inputPathRegex = "H:\\zj\\data\\surveillance\\caviar1\\Meet_Crowd%03d.jpg";
	outputDirectory = "H:\\zj\\data\\surveillance\\results\\caviar1\\temporal\\";
	methodString = "7 8 20 on";
	indexStringSize = 5;
}

int kernelSize = 7;
double imageSizeDivisionFactor = 8;
double saliencyValueScalingFactor = 20;
bool finalSmooth = true;

bool isNumberAPositiveOdd(int n) {
	return (n > 0) && (n % 2 == 1);
}

void parseMethod(const char* methodString) {
	istringstream iss(methodString);
	char finalSmoothString[300];
	iss>>kernelSize>>imageSizeDivisionFactor>>saliencyValueScalingFactor>>finalSmoothString;
	if (!isNumberAPositiveOdd(kernelSize)) {
		cout<<"the kernel size should be a positive odd number!"<<endl;
		exit(1);
	}
	if (kernelSize >= 20) {
		cout<<"kernel size should be less than 20"<<endl;
		exit(1);
	}
	if (strcmp(finalSmoothString, "on") == 0) {
		finalSmooth = true;
	} else if ( strcmp(finalSmoothString, "off") == 0) {
		finalSmooth = false;
	} else {
		cout<<"final smooth should be 'on' or 'off'"<<endl;
		exit(1);
	}

}

void main(int argc, char* argv[]) {

	processCommandLine(argc, argv);
	//setParameters();
	checkInput();
	parseMethod(methodString);

	VideoTemporalSaliencyAnalizer analizer;
	analizer.setInput(inputPathRegex, startIndex, endIndex);
	analizer.setOutput(outputDirectory, indexStringSize);
	
	Kernel1D* kernel = GeneralKernel1D::createMeanSmoothKernel(kernelSize);

	analizer.setKernel(kernel);

	analizer.setImageSizeScalingFactor(imageSizeDivisionFactor);
	analizer.setSaliencyValueScalingFactor(saliencyValueScalingFactor);
	analizer.setFinalSmooth(finalSmooth);


	analizer.run();

	cvWaitKey(0);
}

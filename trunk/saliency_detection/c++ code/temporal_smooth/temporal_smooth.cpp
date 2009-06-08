//input an image sequence
//output an image sequence with temporally smoothed
//input the size of the filter

static char usage[] = 
"\n sequence_processor version 0.1 \n\
\n\
usage: %s -i <input_path_regex> -E <end_index> -o <output_path_regex> -m <method> [options] \n\
\n\
reads an image sequence\n\
output another image sequence\n\
\n\
options:\n\
-S <start_index> default 0\n\
-I <intervel>    default 1\n\
-d               draw converted images\n\
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
char* outputPathRegex = NULL;
//-S
int startIndex = 0;
//-I
int interval = 1;
//-E
int endIndex = 100000;
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
	while ((c = getopt(argc, argv, "i:o:S:I:E:m:dq")) != EOF) {
		switch (c) {
			case 'i': inputPathRegex = argv[optind - 1]; break;
			case 'o': outputPathRegex = argv[optind - 1]; break;
			case 'S': startIndex = atoi(argv[optind - 1]); break;
			case 'I': interval = atoi(argv[optind - 1]); break;
			case 'E': endIndex = atoi(argv[optind - 1]); break;
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

//buffer.size() == filter.size
//filter is a set of weights, multiply to the buffer
vector<IplImage*> buffer;
vector<double> filter;
int sideSize = 0;
IplImage* dst = NULL;

const int PATH_SIZE = 300;
char inputPath[PATH_SIZE];
char outputPath[PATH_SIZE];

void initializeBuffer() {
	buffer.resize(filter.size());
	for (int i = -sideSize; i <= sideSize; i ++) {
		int frameIndex = startIndex + i * interval;
		sprintf_s(inputPath, PATH_SIZE, inputPathRegex, frameIndex);
		IplImage* src = cvLoadImage(inputPath, 0);
		if (src == NULL) {
			sprintf_s(inputPath, PATH_SIZE, inputPathRegex, startIndex);
			src = cvLoadImage(inputPath, 0);
			if (src == NULL) {
				cout<<"can not load "<<inputPathRegex<<endl;
				exit(1);
			}
		}
		buffer[i + sideSize] = src;
	}	
}

void smoothTemporally() {
	if (dst == NULL) dst = cvCloneImage(buffer[0]);
	cvZero(dst);
	for (size_t i = 0; i < filter.size(); i++) {
		cvAddWeighted(dst, 1.0, buffer[i], filter[i], 0, dst);
	}
}

int realEndIndex = -1;

vector<double> parseMethod(const char* methodString) {
	vector<double> filter;
	istringstream iss(methodString);
	char method[100];
	int size = 1;
	iss>>method>>size;
	if (size < 0 || size % 2 != 1) {
		cout<<"the filter size should be positive odd number!"<<endl;
		exit(1);
	}
	sideSize = (size - 1) / 2;
	if (strcmp(method, "blur") == 0) {
		filter.resize(size, 1.0 / size);
	} else {
		cout<<"do not support "<<method<<endl;
		exit(1);
	}
	return filter;
}

void checkInput() {
	fvision::exitIfNull(inputPathRegex, "please input input path regex with -i <regex>!");
	fvision::exitIfNull(outputPathRegex, "please input output path regex with -i <regex>!");
	fvision::exitIfNull(methodString, "please input method with -m <method>!");
}

void setParameters() {
	inputPathRegex = "H:\\zj\\data\\surveillance\\caviar1\\Meet_Crowd%03d.jpg";
	outputPathRegex = "H:\\zj\\data\\surveillance\\results\\caviar1\\temporal_blur\\im_%04d.jpg";
	methodString = "blur 7";
}

void main(int argc, char* argv[]) {

	//processCommandLine(argc, argv);
	setParameters();
	checkInput();

	filter = parseMethod(methodString);

	initializeBuffer();
	for (int i = startIndex; i < endIndex; i += interval) {

		smoothTemporally();

		sprintf_s(outputPath, PATH_SIZE, outputPathRegex, i);
		cvSaveImage(outputPath, dst);

		if (realEndIndex > 0 && i == realEndIndex) break;

		if (i % 10 == 0) cout<<"processing "<<i<<" ..........."<<endl;

		sprintf_s(inputPath, PATH_SIZE, inputPathRegex, i + interval * sideSize);
		if (realEndIndex > 0) {
			sprintf_s(inputPath, PATH_SIZE, inputPathRegex, realEndIndex);
		}
		IplImage* src = cvLoadImage(inputPath, 0);
		if (src == NULL) {
			cout<<"finished at "<<i<<" th frame"<<endl;
			realEndIndex = i + interval * (sideSize - 1);
			sprintf_s(inputPath, PATH_SIZE, inputPathRegex, realEndIndex);
			src = cvLoadImage(inputPath, 0);
		}

		cvReleaseImage(&(buffer[0]));
		buffer.erase(buffer.begin());
		buffer.push_back(src);
	}

}

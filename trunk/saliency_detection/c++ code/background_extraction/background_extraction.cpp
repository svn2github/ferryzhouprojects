//for all the video
//do background extraction and statistics
//output a mean field image and the corresponding variance image
//
//then for each new image, we can do statistics
//
//we seperate the image to a set of grid cells
//for each grid patch, we do clustering on the time domain
//the biggest cluster will deem as the background
//

static char usage[] = 
"\n background_extraction version 0.1 \n\
\n\
usage: %s -i <input_path_regex> -E <end_index> -o <output_path> -m <method> [options] \n\
<method> = \"<block_size> <threshold> <num_of_training_frames>\"\n\
\n\
reads an image sequence\n\
output several image sequences: \n\
  1) raw distance image (gray scale, grid size) \n\
  2) binary distance image (gray scale, grid size)\n\
  3) foreground block image (color, intput image size)\n\
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
//-n
int indexStringSize = 4;
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

#include "BackgroundAnalizer.h"
#include "ImageBlockCluster.h"

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
	outputDirectory = "H:\\zj\\data\\surveillance\\results\\caviar1\\foreground2\\";
	methodString = "8 30.0 100";
	indexStringSize = 5;
}

size_t blockSize = 4;
double threshold = 20.0;
size_t trainingCount = 200;

void parseMethod(const char* methodString) {
	istringstream iss(methodString);
	iss>>blockSize>>threshold>>trainingCount;	
	if (blockSize < 0) {
		cout<<"the blockSize should be positive number!"<<endl;
		exit(1);
	}
}

void main(int argc, char* argv[]) {

	processCommandLine(argc, argv);
	//setParameters();
	checkInput();
	parseMethod(methodString);

	BackgroundAnalizer analizer(blockSize);
	ImageBlockCluster::threshold = threshold;

	const int PATH_SIZE = 300;
	char inputPath[PATH_SIZE];
	char outputPath[PATH_SIZE];

	char indexStringRegex[] = "%04d";
	if (indexStringSize != 4) indexStringRegex[2] = indexStringSize + '0';

	const char* rawDistanceDirName = "raw_distances";
	const char* foregroundBlockDirName = "foreground_blocks";

	string rawDistanceDir = string(outputDirectory) + rawDistanceDirName;
	string foregroundBlockDir = string(outputDirectory) + foregroundBlockDirName;

	string rawDistancePathRegex = rawDistanceDir + "/im_" + indexStringRegex + ".png";
	string foregroundBlockPathRegex = foregroundBlockDir + "/im_" + indexStringRegex + ".jpg";

	fvision::JFile jfile;
	if (!jfile.isFileExists(rawDistanceDir.c_str())) {
		jfile.createDirectory(rawDistanceDir.c_str());
	}
	if (!jfile.isFileExists(foregroundBlockDir.c_str())) {
		jfile.createDirectory(foregroundBlockDir.c_str());
	}

	//trainning
	IplImage* frame = NULL;
	for (int i = 0; i < (int)trainingCount; i++) {
		int frameIndex = startIndex + i * interval;
		sprintf_s(inputPath, PATH_SIZE, inputPathRegex, frameIndex);
		frame = cvLoadImage(inputPath, 0);
		if (frame == NULL) {
			cout<<"finished at "<<i<<" th frame"<<endl;
			exit(1);
		}
		if (i % 10 == 0) cout<<"processing "<<i<<" ..........."<<endl;
		analizer.addFrame(frame);
		cvReleaseImage(&frame);
	}

	cout<<"finished training.........."<<endl;

	const IplImage* rawDistanceImage = NULL;
	const IplImage* foreground = NULL;

	//classify
	for (int i = startIndex; i < endIndex; i += interval) {
		sprintf_s(inputPath, PATH_SIZE, inputPathRegex, i);
		IplImage* src = cvLoadImage(inputPath, 1);
		if (src == NULL) {
			cout<<"finished at "<<i<<" th frame"<<endl;
			break;
		}
		analizer.testFrame(src);
		foreground = analizer.getForegroundBlockImage();
		rawDistanceImage = analizer.getRawDistanceImage();

		sprintf_s(outputPath, PATH_SIZE, rawDistancePathRegex.c_str(), i);
		cvSaveImage(outputPath, rawDistanceImage);
		sprintf_s(outputPath, PATH_SIZE, foregroundBlockPathRegex.c_str(), i);
		cvSaveImage(outputPath, foreground);

		if (i % 10 == 0) cout<<"processing "<<i<<" ..........."<<endl;
		cvReleaseImage(&src);
	}

	cvWaitKey(0);
}

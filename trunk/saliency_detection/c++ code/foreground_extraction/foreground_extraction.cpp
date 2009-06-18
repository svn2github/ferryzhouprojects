//resize the original image
//model each pixel
//cluster pixel, compute cluster mean
//upgrade background gradually b(t+1) = (1 - w) * b(t) + w * p(t+1)
//make background clusters (up to N)

//the difference of the background pixel value and original image pixel value is the saliency

#include "Test_InitialBackgroundAnalizer.h"

static char usage[] = 
"\n foreground_extraction version 0.1 \n\
\n\
usage: %s -i <input_path_regex> -E <end_index> -o <output_path> -m <method> [options] \n\
<method> = \"<division_factor> <lower_threshold> <higher_threshold> <num_of_training_frames> <update_weight>\"\n\
\n\
reads an image sequence\n\
output several image sequences: \n\
1) raw distance image (gray scale, grid size) \n\
2) binary distance image (gray scale, grid size)\n\
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
	if (optind != argc || argc == 1) {
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
	//inputPathRegex = "H:\\zj\\data\\surveillance\\caviar1\\Meet_Crowd%03d.jpg";
	//outputDirectory = "H:\\zj\\data\\surveillance\\results\\caviar1\\foreground2\\";
	inputPathRegex = "H:\\zj\\data\\surveillance\\samsung2\\im_%05d.jpg";
	outputDirectory = "H:\\zj\\data\\surveillance\\results\\samsung2\\";
	methodString = "4 30.0 50.0 50 0.05";
	indexStringSize = 5;
	//startIndex = 1;
	//drawImage = true;
}

double divisionFactor = 4;
double lowerThreshold = 30;
double higherThreshold = 60.0;
size_t trainingCount = 200;
double updateWeight = 0.05;

void parseMethod(const char* methodString) {
	istringstream iss(methodString);
	iss>>divisionFactor>>lowerThreshold>>higherThreshold>>trainingCount>>updateWeight;
	cout<<"division_factor: "<<divisionFactor<<endl;
	cout<<"lower_threshold: "<<lowerThreshold<<endl;
	cout<<"higher_threshold: "<<higherThreshold<<endl;
	cout<<"trainning_frame_count: "<<trainingCount<<endl;
	cout<<"updating_weight: "<<updateWeight<<endl;
	if (divisionFactor < 0) {
		cout<<"the divisionFactor should be positive number!"<<endl;
		exit(1);
	}
	if (updateWeight < 0 || updateWeight > 1) {
		cout<<"update weight should be in [0, 1]!"<<endl;
		exit(1);
	}
}

#include "ForegroundSaliencyAnalizerImpl.h"
#include "image_io.h"
#include "image_filter.h"

using namespace image_io;
using namespace image_filter;

ImageInputHandler* configInputHandler();
ImageOutputHandler* configOutputHandler(const char* outputPathRegex);

void main(int argc, char* argv[]) {

	processCommandLine(argc, argv);
	//setParameters();
	checkInput();
	parseMethod(methodString);

	ForegroundSaliencyAnalizerImpl* analizer = new ForegroundSaliencyAnalizerImpl();
	analizer->setBackgroundUpdatingRatio(updateWeight);
	analizer->setThresholds(higherThreshold, lowerThreshold);
	analizer->setTrainingFrameCount(trainingCount);

	char indexStringRegex[] = "%05d";
	if (indexStringSize != 5) indexStringRegex[2] = indexStringSize + '0';

	const char* rawDistanceDirName = "raw_saliency";
	const char* binarySaliencyDirName = "binary_saliency";

	string rawDistanceDir = string(outputDirectory) + rawDistanceDirName;
	string binarySaliencyDir = string(outputDirectory) + binarySaliencyDirName;

	string rawDistancePathRegex = rawDistanceDir + "/im_" + indexStringRegex + ".png";
	string binarySaliencyPathRegex = binarySaliencyDir + "/im_" + indexStringRegex + ".png";

	fvision::JFile jfile;
	if (!jfile.isFileExists(rawDistanceDir.c_str())) {
		jfile.createDirectory(rawDistanceDir.c_str());
	}
	if (!jfile.isFileExists(binarySaliencyDir.c_str())) {
		jfile.createDirectory(binarySaliencyDir.c_str());
	}

	ImageInputHandler* inputHandler = configInputHandler();
	ImageOutputHandler* outputHandler = configOutputHandler(binarySaliencyPathRegex.c_str());

	analizer->setImageInputHandler(inputHandler);
	analizer->setImageOutputHandler(outputHandler);

	try {
		analizer->run();
	} catch (const image_io::ImageIoException& e) {
		cout<<e.what()<<endl;
	}

	cvWaitKey(0);
}

ImageInputHandler* configInputHandler() {
	SequenceImageFilter* imageFilter = new SequenceImageFilter();
	imageFilter->addFilter(ResizeImageFilter::createWithNewRatio(1 / divisionFactor));
	imageFilter->addFilter(SmoothImageFilter::create());
	return new FilteredImageInputHandler(
		ImageInputHandlerFactory().createPathRegexImageInputHandler(inputPathRegex, startIndex, endIndex, interval),
		imageFilter
		);
}

ImageOutputHandler* configOutputHandler(const char* outputPathRegex) {
	ImageOutputHandler* saveOutputHandler = ImageOutputHandlerFactory().createPathRegexImageOutputHandler(outputPathRegex, startIndex, interval);
	SequenceImageOutputHandler* outputHandler = new SequenceImageOutputHandler();
	outputHandler->addImageOutputHandler(saveOutputHandler);
	if (drawImage) {
		ImageOutputHandler* windowOutputHandler = new FilteredImageOutputHandler(
			ImageOutputHandlerFactory().createWindowImageOutputHandler("binary_saliency", 10),
			ResizeImageFilter::createWithNewRatio(divisionFactor)
			);
		outputHandler->addImageOutputHandler(windowOutputHandler);
	}
	return outputHandler;
}
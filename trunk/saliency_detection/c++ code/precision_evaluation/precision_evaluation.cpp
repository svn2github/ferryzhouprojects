//given a ground truth range file and a experiment result range file
//generate precision report
//two sets of 0 1 0 1
//
//first a generate image of the events
//then 

static char usage[] = 
"\n precision_evaluation version 0.1 \n\
\n\
usage: %s -g <ground_truth_file> -i <exp_results_file> -o <output_file_path> [options] \n\
\n\
reads a ground truth salienct events range file and an experiment result file\n\
output precision_report \n\
\n\
options:\n\
-q               quiet (turn off debugging output)\n\
\
Example:\n\
";

#include <XGetopt.h>

//#include <fvision/utils.h>
#include <iostream>
#include <range_utils.h>
#include <fstream>

//using namespace fvision;
using namespace std;

//g
char* groundTruthFile = NULL;
//i
char* experimentResultFile = NULL;
//o
char* outputFilePath = NULL;
//q
bool verbose = true;

void processCommandLine(int argc, char* argv[]) {
	int c = 0;
	while ((c = getopt(argc, argv, "g:i:o:q")) != EOF) {
		switch (c) {
			case 'g': groundTruthFile = argv[optind - 1]; break;
			case 'i': experimentResultFile = argv[optind - 1]; break;
			case 'o': outputFilePath = argv[optind - 1]; break;
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

void setParameters() {
	groundTruthFile = "test_data/ground_truth.txt";
	experimentResultFile = "test_data/exp.txt";
	outputFilePath = "test_data/precision.txt";
}

template <typename T>
void loadE(const std::string& filename, T& t) throw (std::string) {
	std::ifstream ifs(filename.c_str());
	if (ifs.fail()) throw (filename);
	ifs>>t;
	ifs.close();
}


void main(int argc, char* argv[]) {

	processCommandLine(argc, argv);
	//setParameters();

	IndexRangeList groundTruthRanges;
	try {
		loadE(groundTruthFile, groundTruthRanges);
		if (verbose) {
			cout<<"ground truth ranges =============>"<<endl;
			cout<<groundTruthRanges<<endl;
		}

		IndexRangeList expResultsRanges;
		loadE(experimentResultFile, expResultsRanges);
		if (verbose) {
			cout<<"experiment ranges =============>"<<endl;
			cout<<expResultsRanges<<endl;
		}

		int groundTruthRangeLength = getTotalRangeLength(groundTruthRanges);
		int expRangeLength = getTotalRangeLength(expResultsRanges);
		int overlap = getRangeListOverlap(groundTruthRanges, expResultsRanges);

		double precision = (double)overlap / expRangeLength;
		double recall = (double)overlap / groundTruthRangeLength;

		ofstream ofs(outputFilePath);
		if (ofs.fail()) {
			cout<<"can not write to "<<outputFilePath<<endl;
			exit(1);
		}
		ofs<<"precision: "<<precision<<endl;
		ofs<<"recall: "<<recall<<endl;
		ofs<<"ground truth length: "<<groundTruthRangeLength<<endl;
		ofs<<"experiment result length: "<<expRangeLength<<endl;
		ofs<<"overlap: "<<overlap<<endl;
		ofs.close();
	}catch (const std::string& exp) {
		cout<<"error: "<<exp.c_str()<<endl;
		exit(1);
	}
}


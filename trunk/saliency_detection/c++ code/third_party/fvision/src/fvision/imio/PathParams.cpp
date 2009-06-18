#include "PathParams.h"

using namespace std;

namespace fvision {

PathParams::PathParams (
	const std::string& dir,
	const std::string& filename,
	const std::string& suffixRegex,
	const std::string& extension,
	int startIndex,
	int endIndex
	) {

	this->dir = dir;
	this->filename = filename;
	this->suffixRegex = suffixRegex;
	this->extension = extension;
	this->startIndex = startIndex;
	this->endIndex = endIndex;

	char last = dir[dir.length() - 1];
	if (last != '/' && last != '\\') {
		this->dir += "/";
	}
}

PathParams::~PathParams(void)
{
}

std::string PathParams::getPath(int index) const {
	std::string seperator = "";
	char last = dir[dir.length() - 1];
	if (last != '/' && last != '\\') {
		seperator += "/";
	}
	return dir + seperator + getFilename(index) + "." + extension;
}

std::string PathParams::getFilename(int index) const {
	char suffix[300];
	sprintf_s(suffix, 300, suffixRegex.c_str(), index);
	string str = filename + suffix;
	return str;
}

}
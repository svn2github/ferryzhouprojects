#include "IndexRangeImagePathIterator.h"

#include <cassert>

using namespace std;

namespace fvision {

IndexRangeImagePathIterator::IndexRangeImagePathIterator(const PathParams& pathParams) {
	///need to do verification in the future
	this->pathParams = pathParams;
	assert(pathParams.startIndex <= pathParams.endIndex);
	assert(pathParams.suffixRegex.size() < 300);
	curIndex = pathParams.startIndex - 1;
}

bool IndexRangeImagePathIterator::hasNext() const {
	if (curIndex < pathParams.endIndex) return true;
	else return false;
}

std::string IndexRangeImagePathIterator::next() {
	curIndex++;
	return getPath(curIndex);
}

std::string IndexRangeImagePathIterator::getFilename() const {
	return getFilename(curIndex);
}

std::string IndexRangeImagePathIterator::getPath(int index) const {
	string dir = pathParams.dir;
	char last = dir[dir.length() - 1];
	if (last != '/' && last != '\\') {
		dir += "/";
	}
	return dir + "/" + getFilename(index) + "." + pathParams.extension;
}

std::string IndexRangeImagePathIterator::getFilename(int index) const {
	char suffix[300];
	sprintf_s(suffix, 300, pathParams.suffixRegex.c_str(), index);
	string str = pathParams.filename + suffix;
	return str;
}

}
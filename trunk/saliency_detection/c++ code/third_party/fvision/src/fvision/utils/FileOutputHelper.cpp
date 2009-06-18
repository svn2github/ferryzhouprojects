#include "FileOutputHelper.h"

#include "file_utils.h"

using namespace std;

namespace fvision {

FileOutputHelper::FileOutputHelper(void) {
	dir     = "";
	prefix  = "";
	suffix  = "";
	extension  = "txt";
}

FileOutputHelper::~FileOutputHelper(void) {
}

void FileOutputHelper::setDir(const string& dir) {
	this->dir = dir;
	if (dir.length() == 0) return;
	char last = dir[dir.length() - 1];
	if (last != '/' && last != '\\') {
		this->dir += "/";
	}
	JFile file;
	file.createDirectory(dir.c_str());
}

void FileOutputHelper::setPrefix(const string& prefix) {
	this->prefix = prefix;
}

void FileOutputHelper::setSuffix(const string& suffix) {
	this->suffix = suffix;
}

std::string FileOutputHelper::getFilePath(const std::string& filename) const {
	assert(filename.length() != 0);
	return dir + prefix + filename + suffix + "." + extension;
}

void FileOutputHelper::setExtension(const std::string& extension) {
	this->extension = extension;
}

}
#include "drawing_ImageDemoHelper.h"

#include "drawing.h"

using namespace std;

namespace fvision {

ImageDemoHelper::ImageDemoHelper() {
	show    = true;
	save    = true;
	dir     = "";
	prefix  = "";
	suffix  = "";
	extension  = "jpg";

	showInSameWindow = false;
	interval = 30;
}

void ImageDemoHelper::setDir(const string& dir) {
	this->dir = dir;
	if (dir.length() == 0) return;
	char last = dir[dir.length() - 1];
	if (last != '/' && last != '\\') {
		this->dir += "/";
	}
}

void ImageDemoHelper::setPrefix(const string& prefix) {
	this->prefix = prefix;
}

void ImageDemoHelper::setSuffix(const string& suffix) {
	this->suffix = suffix;
}

void ImageDemoHelper::setShow(bool show) {
	this->show = show;
}

void ImageDemoHelper::setSave(bool save) {
	this->save = save;
}

void ImageDemoHelper::setExtension(const std::string& extension) {
	this->extension = extension;
}

void ImageDemoHelper::demo(const IplImage* src, const string& filename) const {
	assert(filename.length() != 0);

	string filepath = dir + prefix + filename + suffix + "." + extension;

	if (show) {
		if (!showInSameWindow) {
			showImage(filepath.c_str(), src);
		}
		else {
			showImage(filename.c_str(), src);
			cvWaitKey(interval);
		}
	}
	if (save) cvSaveImage(filepath.c_str(), src);
}

void ImageDemoHelper::setShowInSameWindow(bool sameWindow, int interval) {
	this->showInSameWindow = sameWindow;
	this->interval = interval;
}

}
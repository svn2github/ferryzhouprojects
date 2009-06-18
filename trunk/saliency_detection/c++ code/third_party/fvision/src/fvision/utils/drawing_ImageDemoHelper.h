#pragma once

#include <cv.h>

#include <string>

namespace fvision {

/** show (or/and) save an image, by default, show = save = true
 * 
 * the full outpath of the image is ("%s%s%s%s.%s", dir, prefix, filename, suffix, extension)
 * assume dir end with '/' or '\', if not, append '/'
 *
 * by default:
 *    dir     = "."
 *    prefix  = ""
 *    suffix  = ""
 *    extension  = "jpg"
 */
class ImageDemoHelper {
public:
	ImageDemoHelper();

	//Default destructor

	//Default copy constructor
	//Default copy assignment

public:
	///if dir != "", then assume dir end with '/' or '\', if not, append '/'
	void setDir    (const std::string& dir);
	void setPrefix (const std::string& prefix);
	void setSuffix (const std::string& suffix);
	void setExtension(const std::string& extension);

	void setShow   (bool show);
	void setSave   (bool save);
	void setShowInSameWindow(bool sameWindow, int interval = 30); //default false

	std::string getDir() const { return dir; }

public:
	///assert filename.length() != 0
	void demo(const IplImage* src, const std::string& filename) const;

private:
	std::string  dir;
	std::string  suffix;
	std::string  prefix;
	std::string  extension;

	bool show;
	bool save;

	bool showInSameWindow;
	int interval;
};

}
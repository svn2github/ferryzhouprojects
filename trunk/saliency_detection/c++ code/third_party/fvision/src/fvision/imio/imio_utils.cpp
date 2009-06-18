#include "imio_utils.h"

#include <fvision/utils/string_utils.h>

using namespace std;

namespace fvision {

FileType getFileType(const char* path) {
	string dir, filenamestr, suffix;
	decomposePath(path, dir, filenamestr, suffix);
	if (suffix.length() == 0) { //dir
		return FOLDER;
	} else if (strcmp(suffix.c_str(), "avi") == 0) {
		return AVI;
	} else { //regard as still image
		return STILL;
	}
}

}

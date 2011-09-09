#include ".\stringutil.h"

#include "math.h"

namespace ferry { namespace util {

string trim(std::string& s,const string& drop) {
	string r=s.erase(s.find_last_not_of(drop)+1);
	return r.erase(0,r.find_first_not_of(drop));
}

bool decomposePair(const string& s, string& pre, string& post) {
	size_t index = s.find_first_of('=');
	if (index == string::npos) {
		return false;
	}
	pre.append(trim(s.substr(0, index)));
	post.append(trim(s.substr(index + 1)));
	return true;
}

char* cloneString(const char* s) {

	if (s == NULL) return NULL;
	char* ns = new char[strlen(s) + 1];
	return strcpy(ns, s);
}

void decomposePath(const char* path, string& dir, string& filename, string& filetype) {
	string spath = string(path);
	int dotpos = (int)spath.rfind('.');
	int slash1 = (int)spath.rfind('/');
	int slash2 = (int)spath.rfind('\\');
	int slashpos = (slash1 >= slash2) ? slash1 : slash2;
	if (dotpos > slashpos) {
		filetype.append(spath.substr(dotpos + 1));
		filename.append(spath.substr(slashpos + 1, dotpos - slashpos - 1));
	}
	if (slashpos > 0) dir.append(spath.substr(0, slashpos));
}

const char* getFileName(const char* path, string& filename) {
	string dir, filetype;
	decomposePath(path, dir, filename, filetype);
	return filename.c_str();
}

}}
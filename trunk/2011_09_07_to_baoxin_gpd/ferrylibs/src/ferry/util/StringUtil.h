#pragma once
#pragma warning(disable:4786) //turn off STL complains

#include <string>

using namespace std;

namespace ferry { namespace util {

string trim(string& s,const string& drop = " ");

// "a = b" => <a, b>
bool decomposePair(const string& s, string& pre, string& post);

char* cloneString(const char* s);

// xxxx/ssss/yy.zz => xxxx/ssss yy zz
void decomposePath(const char* path, string& dir, string& filename, string& filetype);

const char* getFileName(const char* path, string& filename);
}}
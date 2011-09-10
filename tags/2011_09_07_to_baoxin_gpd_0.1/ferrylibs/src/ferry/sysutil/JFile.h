
#pragma once

#pragma warning(disable:4786) //turn off STL complains

#include <vector>

#include <windows.h>

using namespace std;

namespace ferry { namespace sysutil {

class JFile
{
public:
	JFile();
	~JFile(void);

public:
	vector<string> listFiles(const char* path);
	vector<string> listDirectories(const char* path);

	bool createDirectory(const char* path);
	bool deleteDirectory(const char* path);

	bool createFile(const char* path);
	bool deleteFile(const char* path);

	bool isFileExists(const char* path);

};

}}
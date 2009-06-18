#pragma once

#include <vector>
#include <string>

namespace fvision {

class JFile {
public:
	JFile();
	~JFile(void);

public:
	std::vector<std::string> listFiles(const char* path);
	std::vector<std::string> listDirectories(const char* path);

	bool createDirectory(const char* path);
	bool deleteDirectory(const char* path);

	bool createFile(const char* path);
	bool deleteFile(const char* path);

	bool isFileExists(const char* path);

	const std::string& getErrorMessage() const { return errorMessage; }
private:
	std::string errorMessage;
};

}
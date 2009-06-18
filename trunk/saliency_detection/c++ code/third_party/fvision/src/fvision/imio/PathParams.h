#pragma once

#include <string>

namespace fvision {

class PathParams {
public:
	PathParams() {
	}

	PathParams(
		const std::string& dir,
		const std::string& filename,
		const std::string& suffixRegex,
		const std::string& extension,
		int startIndex,
		int endIndex
		);
	~PathParams(void);

public:
	std::string getPath(int index) const;
	std::string getFilename(int index) const;

public:
	std::string dir;
	std::string filename;
	std::string suffixRegex;
	std::string extension;
	int startIndex;
	int endIndex;
};

}
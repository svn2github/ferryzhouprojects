#pragma once

#include "io_template.h"

#include <string>

namespace fvision {

/**
 * if directory does not exist, create one
 * 
 */
class FileOutputHelper {
public:
	FileOutputHelper(void);
	~FileOutputHelper(void);

public:
	///if dir != "", then assume dir end with '/' or '\', if not, append '/'
	void setDir    (const std::string& dir);
	void setPrefix (const std::string& prefix);
	void setSuffix (const std::string& suffix);
	void setExtension(const std::string& extension);

	std::string getFilePath(const std::string& filename) const ;

	template<typename T>
	void save(const std::string& filename, const T& object) const {
		std::string filepath = getFilePath(filename);
		fvision::save(filepath, object);
	}

private:
	std::string dir;
	std::string prefix;
	std::string suffix;
	std::string extension;
};

}
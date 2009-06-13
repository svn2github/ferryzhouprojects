#ifndef IMAGE_IO_EXCEPTION_H_
#define IMAGE_IO_EXCEPTION_H_

#include <exception>
#include <string>

namespace image_io {

class ImageIoException : public std::runtime_error {
public:
	ImageIoException(const std::string& filePath);
	virtual ~ImageIoException(void) throw() {}
	
public:
	virtual const char* what() const throw() { return message.c_str(); }
	std::string getFileName() const { return filePath; }

private:
	std::string filePath;
	std::string message;
};

}

#endif // IMAGE_IO_EXCEPTION_H_

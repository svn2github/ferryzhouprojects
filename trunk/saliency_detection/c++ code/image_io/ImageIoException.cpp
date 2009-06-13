#include "ImageIoException.h"

using namespace std;

namespace image_io {

ImageIoException::ImageIoException(const std::string& filePath) : runtime_error(""){
	this->filePath = filePath;
}


}
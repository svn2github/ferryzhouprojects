#ifndef FVISION_IMIO_IMAGE_PATH_ITERATOR_H_
#define FVISION_IMIO_IMAGE_PATH_ITERATOR_H_

#include <string>

namespace fvision {

class ImagePathIterator {
public:
	virtual ~ImagePathIterator(void) {
	}
public:
	virtual bool hasNext() const = 0;
	virtual std::string next() = 0;
	virtual std::string getFilename() const = 0;
};

}
#endif // FVISION_IMIO_IMAGE_PATH_ITERATOR_H_
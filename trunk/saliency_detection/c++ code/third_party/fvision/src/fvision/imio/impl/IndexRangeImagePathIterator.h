#ifndef FVISION_IMIO_INDEX_RANGE_IMAGE_PATH_ITERATOR_H_
#define FVISION_IMIO_INDEX_RANGE_IMAGE_PATH_ITERATOR_H_

#include "../ImagePathIterator.h"
#include "../PathParams.h"

#include <string>

namespace fvision {

//based on PathParams
class IndexRangeImagePathIterator : public ImagePathIterator {
public:
	IndexRangeImagePathIterator(const PathParams& pathParams);
	~IndexRangeImagePathIterator(void) {
	}

public:
	virtual bool hasNext() const ;
	virtual std::string next() ;
	virtual std::string getFilename() const ;

private:
	std::string getPath(int index) const ;
	std::string getFilename(int index) const ;

private:
	PathParams pathParams;

	int curIndex;
};

}
#endif // FVISION_IMIO_INDEX_RANGE_IMAGE_PATH_ITERATOR_H_
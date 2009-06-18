#ifndef FVISION_IMIO_IMAGE_SEQUENCE_READER_FACTORY_H_
#define FVISION_IMIO_IMAGE_SEQUENCE_READER_FACTORY_H_

#include "ImageSequenceReader.h"
#include "ImageSequenceBuffer.h"

#include "PathParams.h"

#include <string>

namespace fvision {

class ImageSequenceInputParams {
public:
	enum PATH_TYPE {
		FILE,
		PATH_PARAMS
	};
	PATH_TYPE type;
	std::string path;
	PathParams pathParams;
};

class ImageSequenceReaderFactory {
public:
	ImageSequenceReaderFactory(void);
	virtual ~ImageSequenceReaderFactory(void);

public:
	virtual ImageSequenceReader* create(const std::string& path);
	virtual ImageSequenceBuffer* createImageSequenceBuffer(const std::string& path, int leftSpan = 0, int rightSpan = 10);
	virtual ImageSequenceReader* createWithPathParams(const PathParams& pathParams);
	virtual ImageSequenceReader* createBlankImageSequenceReader(const CvSize& size, int depth, int channels, CvScalar color);
	virtual ImageSequenceReader* create(const ImageSequenceInputParams& params);
};

}
#endif // FVISION_IMIO_IMAGE_SEQUENCE_READER_FACTORY_H_
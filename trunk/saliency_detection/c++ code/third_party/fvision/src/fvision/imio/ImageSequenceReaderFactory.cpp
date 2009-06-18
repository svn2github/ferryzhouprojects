#include "ImageSequenceReaderFactory.h"

#include "impl/UnifiedImageSequenceReader.h"
#include "impl/ImageSequenceBufferImpl.h"
#include "impl/IndexRangeImagePathIterator.h"
#include "impl/IteratorImageSequenceReader.h"
#include "impl/BlankImageSequenceReader.h"

namespace fvision {

ImageSequenceReaderFactory::ImageSequenceReaderFactory(void) {
}

ImageSequenceReaderFactory::~ImageSequenceReaderFactory(void)
{
}

ImageSequenceReader* ImageSequenceReaderFactory::create(const std::string& path) {
	return new UnifiedImageSequenceReader(path.c_str());
}

ImageSequenceReader* ImageSequenceReaderFactory::create(const ImageSequenceInputParams& params) {
	switch(params.type) {
		case ImageSequenceInputParams::FILE: return create(params.path);
		case ImageSequenceInputParams::PATH_PARAMS: return createWithPathParams(params.pathParams);
		default: return create(params.path);
	}
}

ImageSequenceBuffer* ImageSequenceReaderFactory::createImageSequenceBuffer(const std::string& path, int leftSpan, int rightSpan) {
	return new ImageSequenceBufferImpl(new UnifiedImageSequenceReader(path.c_str()), leftSpan, rightSpan);
}

ImageSequenceReader* ImageSequenceReaderFactory::createWithPathParams(const PathParams& pathParams) {
	return new IteratorImageSequenceReader(new IndexRangeImagePathIterator(pathParams));
}

ImageSequenceReader* ImageSequenceReaderFactory::createBlankImageSequenceReader(const CvSize& size, int depth, int channels, CvScalar color) {
	return new BlankImageSequenceReader(size, depth, channels, color);
}
}
#include ".\unifiedimagesequencereader.h"

#include "AVIImageSequenceReader.h"
#include "StillImageSequenceReader.h"
#include "DirImageSequenceReader.h"

namespace fvision {

UnifiedImageSequenceReader::UnifiedImageSequenceReader(const char* path) : ImageSequenceReader(path) {
	FileType type = getFileType(path);
	switch(type) {
		case FOLDER: pisr = new DirImageSequenceReader(path);   break;
		case AVI:    pisr = new AVIImageSequenceReader(path);   break;
		case STILL:  pisr = new StillImageSequenceReader(path); break;
		default:     pisr = new StillImageSequenceReader(path); break;
	}
}
}

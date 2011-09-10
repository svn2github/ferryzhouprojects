#include ".\sievefilter.h"

namespace ferry { namespace imutil { namespace filter {
	IplImage* SieveFilter::processSinglePass(IplImage* src, IplImage* dst) {
		if (src->depth == IPL_DEPTH_8U) return processTemplate ((uchar*)0, src, dst);
		if (src->depth == IPL_DEPTH_8S) return processTemplate((char*)0, src, dst);
		if (src->depth == IPL_DEPTH_16U) return processTemplate((ushort*)0, src, dst);
		if (src->depth == IPL_DEPTH_16S) return processTemplate((short*)0, src, dst);
		if (src->depth == IPL_DEPTH_32S) return processTemplate((float*)0, src, dst);
		if (src->depth == IPL_DEPTH_32F) return processTemplate((float*)0, src, dst);
		if (src->depth == IPL_DEPTH_64F) return processTemplate((double*)0, src, dst);
		
		return dst;
	}

} } }

#pragma once

#include <cv.h>

#include <string>

// assert something with message. If not true, print out message.

namespace fvision {

	/**
	 * 		    im1->width     ==  im2->width
	 *		 && im1->height    ==  im2->height
	 *		 && im1->nChannels ==  im2->nChannels
	 *		 && im1->depth     ==  im2->depth
	 *
	 */
	bool isImageHeaderEqual(const IplImage* im1, const IplImage* im2);

	/// if not true, print message and return false, otherwise return true
	bool boolAssertNotNull(void* pointer, const char* message);

	/// if not true, print message and exit
	void exitIfNull(void* pointer, const std::string& message);

	/// if not true, print message and exit
	void exitIfFalse(bool v, const char* message);

	/// if true, print message and exit
	void exitIf(bool v, const std::string& message);
}
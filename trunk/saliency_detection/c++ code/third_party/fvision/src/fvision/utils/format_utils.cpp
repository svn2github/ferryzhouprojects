#include "format_utils.h"

#include <cstdio>

namespace fvision {

const char* FormatHelper::getNumberString(int id) {
	sprintf_s(idString, 20, "%04d", id);
	return idString;
}

const char* FormatHelper::getNumberString(int id, const char* format) {
	sprintf_s(idString, 20, format, id);
	return idString;
}
}
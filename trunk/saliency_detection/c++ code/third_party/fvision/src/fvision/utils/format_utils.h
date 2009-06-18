#pragma once

namespace fvision {

class FormatHelper {
public:
	const char* getNumberString(int id);
	const char* getNumberString(int id, const char* format);

private:
	char idString[20];
};
}
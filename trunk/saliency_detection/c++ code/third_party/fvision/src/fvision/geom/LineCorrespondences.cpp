#include "LineCorrespondences.h"

using namespace std;

namespace fvision {

LineCorrespondences::LineCorrespondences(void)
{
	frameId1 = 0;
	frameId2 = 1;
}

LineCorrespondences::~LineCorrespondences(void)
{
}

std::ostream& operator<<(std::ostream& os, const LineCorrespondences& lc) {
	os<<lc.frameId1<<" "<<lc.frameId2<<endl;
	os<<lc.ids.size()<<endl;
	for (size_t i = 0; i < lc.ids.size(); i++) {
		os<<lc.ids[i]<<" "<<lc.lines1[i]<<" "<<lc.lines2[i]<<endl;
	}
	return os;
}

std::istream& operator>>(std::istream& is, LineCorrespondences& lc) {
	is>>lc.frameId1>>lc.frameId2;
	int size = 0;
	is>>size;
	lc.ids.resize(size);
	lc.lines1.resize(size);
	lc.lines2.resize(size);
	for (int i = 0; i < size; i++) {
		is>>lc.ids[i]>>lc.lines1[i]>>lc.lines2[i];
	}
	return is;
}

}
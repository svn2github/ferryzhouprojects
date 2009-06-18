#pragma once

namespace fvision {
	
template <typename T>
void deepDelete(const vector<T*>& v) {
	for (size_t i = 0; i < v.size(); i++) {
		delete v[i];
	}
}


}
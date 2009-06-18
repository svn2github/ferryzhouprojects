#ifndef FVISION_ITERATOR_TEMPLATE_H_
#define FVISION_ITERATOR_TEMPLATE_H_

#include <vector>

namespace fvision {

template <typename T>
class ScalarRangeIterator {
public:
	ScalarRangeIterator<T>(const T& start, const T& step, const T& end) {
		this->start = start;
		this->step = step;
		this->end = end;
		init();
	}

public:
	void init() { cursor = start; }

	bool hasNext() { return cursor <= end; }

	T next() { 
		T v = currentValue();
		increaseCursor();
		return v;
	}

	T currentValue() { return cursor; }

private:
	void increaseCursor() { cursor += step; }

private:
	T start;
	T step;
	T end;

	T cursor;
};

//T is the return type for every dimension
//cast all values to be T type
template <typename T>
class MultiRangeIterator {
public:
	// iter0, iter1, iter2, ..., itern
	// the first iter changes last
	//
	MultiRangeIterator<T>(const std::vector<ScalarRangeIterator<T>>& iterators) {
		this->iterators = iterators;
		currentValues.resize(iterators.size());
		init();
	}

public:
	void init() { 
		for (size_t i = 0; i < iterators.size(); i++) {
			iterators[i].init();
			if (i < iterators.size() - 1) currentValues[i] = iterators[i].next();
			else currentValues[i] = iterators[i].currentValue();
		}
	}

	bool hasNext() {
		if (iterators.size() == 0) return false;
		for (size_t i = 0; i < iterators.size(); i++) {
			if (iterators[i].hasNext()) return true;
		}
		return false;
	}

	const std::vector<T>& next() {
		updateCurrentValues();
		return currentValues;
	}

private:
	void updateCurrentValues() {
		for (size_t i = iterators.size() - 1; i >=0 ; i--) {
			if (iterators[i].hasNext()) {
				currentValues[i] = iterators[i].next();
				return;
			} else {
				iterators[i].init();
				currentValues[i] = iterators[i].next();
			}
		}
		throw ("illigal status: already in the end");
	}

private:
	std::vector<ScalarRangeIterator<T>> iterators;
	vector<T> currentValues;
};

}

#endif // FVISION_ITERATOR_TEMPLATE_H_
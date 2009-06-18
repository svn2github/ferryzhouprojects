#ifndef FVISION_TEMPLATE_UTILS_H_
#define FVISION_TEMPLATE_UTILS_H_

#include <vector>
#include <map>
#include <algorithm>
#include <set>

namespace fvision {

template<typename T>
std::vector<T> getSubSet(const std::vector<T>& ts, const std::vector<int>& indices) {
	std::vector<T> nts(indices.size());
	for (size_t i = 0; i < indices.size(); i++) {
		nts[i] = ts[indices[i]];
	}
	return nts;
};

template<typename T>
std::vector<T> getSubSetValues(const std::map<int, T>& ts, const std::vector<int>& indices) {
	std::vector<T> nts(indices.size());
	for (size_t i = 0; i < indices.size(); i++) {
		std::map<int, T>::const_iterator citer = ts.find(indices[i]);
		if (citer == ts.end()) continue;
		else nts[i] = citer->second;
	}
	return nts;
};

template <typename First, typename Second>
void decomposePairs(const std::vector<std::pair<First, Second> >& pairs, std::vector<First>& firsts, std::vector<Second>& seconds) {
	firsts.clear();
	seconds.clear();
	for (size_t i = 0; i < pairs.size(); i++) {
		firsts.push_back(pairs[i].first);
		seconds.push_back(pairs[i].second);
	}
}

template <typename First, typename Second>
void composePairs(std::vector<std::pair<First, Second> >& pairs, const std::vector<First>& firsts, const std::vector<Second>& seconds) {
	assert(firsts.size() == seconds.size());
	pairs.clear();
	std::vector<First>::const_iterator fiter = firsts.begin();
	std::vector<Second>::const_iterator siter = seconds.begin();
	for (; fiter != firsts.end(); ++fiter, ++siter) {
		pairs.push_back(std::pair<First, Second>(*fiter, *siter));
	}
}

template <typename Key, typename Value>
std::vector<Key> getAllKeys(const std::map<Key, Value>& m) {
	std::vector<Key> keys;
	std::map<Key, Value>::const_iterator iter = m.begin();
	for (; iter != m.end(); iter++) keys.push_back(iter->first);
	return keys;
}

template <typename Key, typename Value>
std::vector<Value> getAllValues(const std::map<Key, Value>& m) {
	std::vector<Value> values;
	std::map<Key, Value>::const_iterator iter = m.begin();
	for (; iter != m.end(); iter++) values.push_back(iter->second);
	return values;
}

template <typename Key, typename Value>
void getAllKeysAndValues(const std::map<Key, Value>& m, std::vector<Key>& keys, std::vector<Value>& values) {
	keys.clear();
	values.clear();
	std::map<Key, Value>::const_iterator iter = m.begin();
	for (; iter != m.end(); iter++)  {
		keys.push_back(iter->first);
		values.push_back(iter->second);
	}
}

template <typename T, typename UnaryOperator>
std::vector<T> transformVector(const std::vector<T>& vs, const UnaryOperator& op) {
	std::vector<T> nvs(vs.size());
	std::transform(vs.begin(), vs.end(), nvs.begin(), op);
	return nvs;
}

template <typename T, typename Transformer>
std::vector<T> copyTransform(const std::vector<T>& vs, const Transformer* transformer) {
	std::vector<T> nvs(vs.size());
	for (size_t i = 0; i < vs.size(); i++) {
		nvs[i] = transformer->transform(vs[i]);
	}
	return nvs;
}

template <typename T>
T getMax(const std::vector<T>& vs) {
	return *max_element(vs.begin(), vs.end());
}

template <typename T>
T getMin(const std::vector<T>& vs) {
	return *min_element(vs.begin(), vs.end());
}

template <typename T>
T getMax(const std::set<T>& vs) {
	return *max_element(vs.begin(), vs.end());
}

template <typename T>
T getMin(const std::set<T>& vs) {
	return *min_element(vs.begin(), vs.end());
}

template <typename T, typename Transformer>
void modifyTransform(std::vector<T>& vs, const Transformer* transformer) {
	for (size_t i = 0; i < vs.size(); i++) {
		vs[i] = transformer->transform(vs[i]);
	}
}

template <typename T>
std::vector<T> operator* (const std::vector<T>& vs, T n) {
	std::vector<T> nvs(vs.size());
	for (size_t i = 0; i < vs.size(); i++) {
		nvs[i] = vs[i] * n;
	}
	return nvs;
}

template <typename Iterator, typename UnaryOperator>
void doForEach(Iterator* iter, UnaryOperator& op) {
	while (iter->hasNext()) {
		op(iter->next());
	}
}


}
#endif // FVISION_TEMPLATE_UTILS_H_
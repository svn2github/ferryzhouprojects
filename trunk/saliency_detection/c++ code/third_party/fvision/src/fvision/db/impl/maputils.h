#pragma once

#include <set>
#include <map>
#include <vector>

namespace fvision{
	namespace db {
template <typename Key, typename Value>
std::set<Key> getAllKeys(const std::map<Key, Value>& m) {
	std::set<Key> keys;
	std::map<Key, Value>::const_iterator iter = m.begin();
	for (; iter != m.end(); iter++) keys.insert(iter->first);
	return keys;
}

template <typename Key, typename Value>
std::vector<Value> getAllValues(const std::map<Key, Value>& m) {
	std::vector<Value> values;
	std::map<Key, Value>::const_iterator iter = m.begin();
	for (; iter != m.end(); iter++) values.push_back(iter->second);
	return values;
}

template <typename Key1, typename Key2, typename Value>
bool removeItemInDoubleLevelMap(
	std::map<Key1, std::map<Key2, Value>>& doubleLevelMap, 
	const Key1& key1, 
	const Key2& key2) {

	std::map<Key1, std::map<Key2, Value>>::iterator iter = doubleLevelMap.find(key1);
	if (iter == doubleLevelMap.end()) return false;
	std::map<Key2, Value>& map2 = iter->second;
	std::map<Key2, Value>::iterator iter2 = map2.find(key2);
	if (iter2 == map2.end()) return false;
	map2.erase(iter2);
	return true;
}

template <typename Key1, typename Key2, typename Value>
bool insertItemInDoubleLevelMap(
	std::map<Key1, std::map<Key2, Value>>& doubleLevelMap, 
	const Key1& key1, 
	const Key2& key2, 
	const Value& value) {

	std::map<Key1, std::map<Key2, Value>>::iterator iter = doubleLevelMap.find(key1);
	if (iter == doubleLevelMap.end()) {
		std::map<Key2, Value> map2;
		map2[key2] = value;
		doubleLevelMap[key1] = map2;
	} else {
		std::map<Key2, Value>& map2 = iter->second;
		std::map<Key2, Value>::iterator iter2 = map2.find(key2);
		if (iter2 == map2.end()) {
			map2[key2] = value;
		} else {
			return false; //exists!
		}
	}
	return true;
}
	}
}
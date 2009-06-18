#pragma once

#include "../MultipleFrameIndexedObjectsDatabase.h"
#include "maputils.h"

#include <map>
#include <cassert>

namespace fvision {

template<typename TObject>
class MultipleFrameIndexedObjectsDatabaseMapImpl : public MultipleFrameIndexedObjectsDatabase<TObject> {
public:
	typedef std::map<int, Item> ItemMap;
	typedef std::map<int, std::map<int, int>> ItemIdMap;

private:
	ItemMap itemMap;
	ItemIdMap frameIdObjectIdMap; ///frameId -> objectId => itemId
	ItemIdMap objectIdFrameIdMap; ///objectId -> frameid => itemId

	int maxItemId;

public:
	MultipleFrameIndexedObjectsDatabaseMapImpl<TObject>() {
		maxItemId = 0;
	}

	virtual ~MultipleFrameIndexedObjectsDatabaseMapImpl<TObject> (void) {}

public:
	class MapItemIterator : public MultipleFrameIndexedObjectsDatabase<TObject>::ItemIterator {

	public:
		MapItemIterator(typename ItemMap::iterator begin, typename ItemMap::iterator end) {
			this->iter = begin;
			this->end = end;
		}

	public:
		bool hasNext() {
			return iter != end;
		}

		Item* next() {
			Item* item = &(iter->second);
			iter++;
			return item;
		}

	private:
		typename ItemMap::iterator iter;
		typename ItemMap::iterator end;
	};

	class ConstMapItemIterator : public MultipleFrameIndexedObjectsDatabase<TObject>::ConstItemIterator {

	public:
		ConstMapItemIterator(typename ItemMap::const_iterator begin, typename ItemMap::const_iterator end) {
			this->iter = begin;
			this->end = end;
		}

	public:
		bool hasNext() {
			return iter != end;
		}

		const Item* next() {
			const Item* item = &(iter->second);
			iter++;
			return item;
		}

	private:
		typename ItemMap::const_iterator iter;
		typename ItemMap::const_iterator end;
	};

	ItemIterator* getItemIterator() {
		return new MapItemIterator(itemMap.begin(), itemMap.end());
	}

	ConstItemIterator* getItemIterator() const {
		return new ConstMapItemIterator(itemMap.begin(), itemMap.end());
	}

public:
	virtual size_t size() const {
		return itemMap.size();
	}

	virtual std::set<int> getAllFrameIds() const {
		return fvision::db::getAllKeys(frameIdObjectIdMap);
	}

	virtual std::set<int> getAllObjectIds() const {
		return fvision::db::getAllKeys(objectIdFrameIdMap);
	}

	virtual std::set<int> getAllObjectIdsInFrame(int frameId) const {
		ItemIdMap::const_iterator iter = frameIdObjectIdMap.find(frameId);
		if (iter == frameIdObjectIdMap.end()) return std::set<int>();
		else return fvision::db::getAllKeys(iter->second);
	}
	
	virtual std::vector<TObject> getAllObjectsInFrame(int frameId) const {
		std::vector<int> objectIds;
		std::vector<TObject> objects;
		getIndexedObjectsByFrameId(frameId, objectIds, objects);
		return objects;
	}
	
	virtual void getIndexedObjectsByFrameId(
		int frameId, 
		std::vector<int>& objectIds, 
		std::vector<TObject>& objects) const {

		objectIds.clear();
		objects.clear();

		ItemIdMap::const_iterator iter = frameIdObjectIdMap.find(frameId);
		if (iter == frameIdObjectIdMap.end()) return;
		std::map<int, int>::const_iterator fiter = iter->second.begin();
		for (; fiter != iter->second.end(); fiter++) {
			int objectId = fiter->first;
			int itemId = fiter->second;
			objectIds.push_back(objectId);
			ItemMap::const_iterator itemIter = itemMap.find(itemId);
			assert(itemIter != itemMap.end());
			objects.push_back(itemIter->second.object);
		}
	}

	virtual void getIndexedObjectsByObjectId(
		int objectId, 
		std::vector<int>& frameIds, 
		std::vector<TObject>& objects) const {

		frameIds.clear();
		objects.clear();

		ItemIdMap::const_iterator iter = objectIdFrameIdMap.find(objectId);
		if (iter == objectIdFrameIdMap.end()) return;
		std::map<int, int>::const_iterator fiter = iter->second.begin();
		for (; fiter != iter->second.end(); fiter++) {
			int frameId = fiter->first;
			int itemId = fiter->second;
			frameIds.push_back(frameId);
			ItemMap::const_iterator itemIter = itemMap.find(itemId);
			assert(itemIter != itemMap.end());
			objects.push_back(itemIter->second.object);
		}
	}

	virtual size_t getObjectFrequence(int objectId) {
		ItemIdMap::const_iterator iter = objectIdFrameIdMap.find(objectId);
		if (iter == objectIdFrameIdMap.end()) return 0;
		return iter->second.size();
	}

	virtual std::set<int> getAllFrameIdsByObjectId(int objectId) const {
		ItemIdMap::const_iterator iter = objectIdFrameIdMap.find(objectId);
		if (iter == objectIdFrameIdMap.end()) return std::set<int>();
		return fvision::db::getAllKeys(iter->second);
	}

	virtual bool itemExists(int frameId, int objectId) const {
		return getItemId(frameId, objectId) >= 0;
	}

	///return false if <frameId, objectId> does not exist
	virtual bool getObjectByFrameIdAndObjectId(int frameId, int objectId, TObject& object) const {
		int itemId = getItemId(frameId, objectId);
		if (itemId < 0) return false;
		object = getItem(itemId);
		return true;
	}

	///assert <frameId, objectId> can be found
	virtual TObject getObjectByFrameIdAndObjectId(int frameId, int objectId) const {
		int itemId = getItemId(frameId, objectId);
		assert(itemId >= 0);
		return getItem(itemId);
	}

	///assert all object ids can be found
	virtual std::vector<TObject> getObjectsSubsetInFrame(
		int frameId, 
		const std::vector<int>& objectIds) const {

		std::vector<TObject> objects;

		ItemIdMap::const_iterator iter = frameIdObjectIdMap.find(frameId);
		if (iter == frameIdObjectIdMap.end()) return objects;
		
		const std::map<int, int>& objectIdItemIdMap = iter->second;
		for (size_t i = 0; i < objectIds.size(); i++) {
			int objectId = objectIds[i];
			std::map<int, int>::const_iterator oIter = objectIdItemIdMap.find(objectId);
			assert(oIter != objectIdItemIdMap.end());
			int itemId = oIter->second;
			objects.push_back(getItem(itemId));
		}

		return objects;
	}

public:
	///return false if the item of <frameId, objectId> exists
	virtual bool insertItem(int frameId, int objectId, const TObject& object) {

		if (itemExists(frameId, objectId)) return false;
		
		int itemId = maxItemId;
		itemMap[itemId] = Item(frameId, objectId, object);
		fvision::db::insertItemInDoubleLevelMap(frameIdObjectIdMap, frameId, objectId, itemId);
		fvision::db::insertItemInDoubleLevelMap(objectIdFrameIdMap, objectId, frameId, itemId);

		maxItemId++;
		return true;
	}

	///return false if the item of <frameId, objectId> does not exist
	virtual bool deleteItem(int frameId, int objectId) {

		int itemId = getItemId(frameId, objectId);
		if (itemId < 0) return false;

		removeItemInItemMap(itemId);
		removeItemInFrameIdObjectIdMap(frameId, objectId);
		removeItemInObjectIdFrameIdMap(frameId, objectId);
		return true;
	}

private:
	void removeItemInItemMap(int itemId) {
		ItemMap::iterator iter = itemMap.find(itemId);
		itemMap.erase(iter);
	}

	void removeItemInFrameIdObjectIdMap(int frameId, int objectId) {
		fvision::db::removeItemInDoubleLevelMap(frameIdObjectIdMap, frameId, objectId);
	}

	void removeItemInObjectIdFrameIdMap(int frameId, int objectId) {
		fvision::db::removeItemInDoubleLevelMap(objectIdFrameIdMap, objectId, frameId);		
	}

public:
	///return false if the item of <frameId, objectId> does not exist
	virtual bool updateItem(int frameId, int objectId, const TObject& object) {
		int itemId = getItemId(frameId, objectId);
		if (itemId < 0) return false;
		itemMap[itemId].object = object;
		return true;
	}

	virtual void clear() {
		itemMap.clear();
		frameIdObjectIdMap.clear();
		objectIdFrameIdMap.clear();
	}

public:
	///not implemented yet
	virtual std::string getErrorMessage() const {
		return errorMessage;
	}

private:
	///return -1 if can not find
	int getItemId(int frameId, int objectId) const {

		ItemIdMap::const_iterator iter = frameIdObjectIdMap.find(frameId);
	
		if (iter == frameIdObjectIdMap.end()) return -1;

		std::map<int, int>::const_iterator fiter = iter->second.find(objectId);

		if (fiter == iter->second.end()) return -1;

		return fiter->second;
	}

	const TObject& getItem(int itemId) const {
		ItemMap::const_iterator iter = itemMap.find(itemId);
		assert(iter != itemMap.end());
		return iter->second.object;
	}

private:
	std::string errorMessage;
};

}
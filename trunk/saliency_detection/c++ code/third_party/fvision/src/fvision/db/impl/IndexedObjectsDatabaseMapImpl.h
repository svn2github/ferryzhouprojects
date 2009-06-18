#pragma once

#include "../IndexedObjectsDatabase.h"

#include <map>

#include <cassert>

namespace fvision {

/**
 * use map to implement
 */
template<typename TObject>
class IndexedObjectsDatabaseMapImpl : public IndexedObjectsDatabase<TObject> {
public:
	typedef std::map<int, Item> ItemMap;

public:
	IndexedObjectsDatabaseMapImpl<TObject>(void) {
		maxId = 0;
	}
	virtual ~IndexedObjectsDatabaseMapImpl<TObject>(void) {
	}

public:
	class MapItemIterator : public IndexedObjectsDatabase<TObject>::ItemIterator {

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

	ItemIterator* getItemIterator() {
		return new MapItemIterator(itemMap.begin(), itemMap.end());
	}

	class ConstMapItemIterator : public IndexedObjectsDatabase<TObject>::ConstItemIterator {

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

	ConstItemIterator* getItemIterator() const {
		return new ConstMapItemIterator(itemMap.begin(), itemMap.end());
	}

public:
	virtual size_t size() const {
		return itemMap.size();
	}

	virtual std::set<int> getAllIds() const {
		std::set<int> ids;
		ItemMap::const_iterator iter = itemMap.begin();
		for ( ; iter != itemMap.end(); iter++) {
			ids.insert(iter->first);
		}
		return ids;
	}

	virtual std::vector<TObject> getAllObjects() const {
		std::vector<TObject> objects;
		ItemMap::const_iterator iter = itemMap.begin();
		for ( ; iter != itemMap.end(); iter++) {
			objects.push_back(iter->second.object);
		}
		return objects;
	}

	virtual bool idExists(int id) const {
		ItemMap::const_iterator iter = itemMap.find(id);
		return iter != itemMap.end();
	}

	virtual bool getObjectById(int id, TObject& object) const {
		ItemMap::const_iterator iter = itemMap.find(id);
		if (iter != itemMap.end()) {
			object = iter->second.object;
			return true;
		} else {
			return false;
		}
	}

	virtual TObject getObjectById(int id) const {
		ItemMap::const_iterator iter = itemMap.find(id);
		assert(iter != itemMap.end());
		return iter->second.object;
	}
	
	virtual TObject* getObjectPointerById(int id) {
		ItemMap::iterator iter = itemMap.find(id);
		assert(iter != itemMap.end());
		return &(iter->second.object);
	}

	virtual const TObject* getObjectPointerById(int id) const {
		ItemMap::const_iterator iter = itemMap.find(id);
		assert(iter != itemMap.end());
		return &(iter->second.object);
	}

public:
	virtual bool insertItem(int id, const TObject& object) {
		if (idExists(id)) return false;
		itemMap[id] = Item(id, object);
		if (id >= maxId) maxId = id + 1;
		return true;
	}

	virtual int insertItem(const TObject& object) {
		itemMap[maxId] = Item(maxId, object);
		maxId++;
		return maxId - 1;
	}

	virtual bool deleteItem(int id) {
		size_t numRemoved = itemMap.erase(id);
		return numRemoved == 1;
	}

	virtual bool updateItem(int id, const TObject& object) {
		ItemMap::iterator iter = itemMap.find(id);
		if (iter != itemMap.end()) {
			iter->second.object = object;
			return true;
		} else {
			return false;
		}
	}

	virtual void clear() {
		itemMap.clear();
	}

public:
	virtual IndexedObjectsDatabase<TObject>* clone() {
		IndexedObjectsDatabaseMapImpl<TObject>* db = new IndexedObjectsDatabaseMapImpl<TObject>();
		db->itemMap = this->itemMap;
		db->maxId = this->maxId;
		this->errorMessage = this->errorMessage;
		return db;
	}

public:
/*	virtual void readFromStream(std::istream& is) {
		size_t size = 0;
		is>>size;
		for (size_t i = 0; i < size; i++) {
			Item item;
			is>>item.id>>item.object;
			itemMap[item.id] = item;
		}
	}

	virtual void writeToStream(std::ostream& os) const {
		os<<itemMap.size()<<std::endl;
		ItemMap::const_iterator iter = itemMap.begin();
		for( ; iter != itemMap.end(); iter++) {
			os<<iter->first<<" "<<iter->second.object<<std::endl;
		}
	}
*/
	///no implementation yet
	std::string getErrorMessage() const {
		return errorMessage;
	}

private:
	typename ItemMap itemMap;
	std::string errorMessage;

	int maxId;
};

}
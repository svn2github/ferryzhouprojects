#pragma once

//#include "DatabaseException.h"

#include <vector>
#include <set>
#include <iostream>
#include <cassert>

namespace fvision {

/**
 * interface
 * a table with each row is
 * <id, object>
 *
 */
template<typename TObject>
class IndexedObjectsDatabase {
public:
	virtual ~IndexedObjectsDatabase<TObject>(void) {
	}

public:
	struct Item {
	public:
		Item() {
		}

		Item(int id, const TObject& object) {
			this->id = id;
			this->object = object;
		}

	public:
		int id;
		TObject object;
	};

	class ItemIterator {
	public:
		virtual bool hasNext() = 0;
		virtual Item* next() = 0;
	};

	virtual ItemIterator* getItemIterator() = 0;

	class ConstItemIterator {
	public:
		virtual bool hasNext() = 0;
		virtual const Item* next() = 0;
	};

	virtual ConstItemIterator* getItemIterator() const = 0;

public:
	virtual size_t size() const = 0;

	virtual std::set<int> getAllIds() const = 0;

	std::vector<int> getIdVector() const {
		std::set<int> idSet = getAllIds();
		std::vector<int> idVector(idSet.begin(), idSet.end());
		return idVector;
	}

	virtual std::vector<TObject> getAllObjects() const = 0;

	virtual std::vector<TObject> getObjectSubset(const std::vector<int>& objectIds) const {
		std::vector<TObject> objects;
		for (size_t i = 0; i < objectIds.size(); i++) {
			objects.push_back(getObjectById(objectIds[i]));
		}
		return objects;
	}

	///return false if no id found
	virtual bool getObjectById(int id, TObject& object) const = 0;

	///assert id exists
	virtual TObject getObjectById(int id) const = 0;

	virtual bool idExists(int id) const = 0;

	virtual bool itemExists(int id) const { return idExists(id); }

	///assert id exists
	virtual TObject* getObjectPointerById(int id) = 0;

	///assert id exists
	virtual const TObject* getObjectPointerById(int id) const = 0;

public:
	///if id exists, return false and do not do update
	virtual bool insertItem(int id, const TObject& object) = 0;

	virtual bool insertItem(const Item* item) {
		return insertItem(item->id, item->object);
	}

	virtual void insertOrUpdateItem(int id, const TObject& object) {
		if (idExists(id)) updateItem(id, object);
		else insertItem(id, object);
	}

	///if failed, then no item is inserted; otherwise, all items inserted
	virtual bool insertItems(const std::vector<int> objectIds, const std::vector<TObject>& objects) {
		assert (objectIds.size() == objects.size());
		for (size_t i = 0; i < objectIds.size(); i++) {
			if (idExists(objectIds[i])) return false;
		}
		for (size_t i = 0; i < objectIds.size(); i++) {
			insertItem(objectIds[i], objects[i]);
		}
		return true;
	}

	//insert an item, return the assigned id
	virtual int insertItem(const TObject& object) = 0;

	///return false id does not exist
	virtual bool deleteItem(int id) = 0;

	///return false id does not exist
	virtual bool updateItem(int id, const TObject& object) = 0;

	virtual void clear() = 0;

public:
	virtual IndexedObjectsDatabase<TObject>* clone() = 0;

public:
	virtual std::string getErrorMessage() const = 0;

public:
	//virtual void readFromStream(std::istream& is) = 0;

	//virtual void writeToStream(std::ostream& os) const = 0;
};

}
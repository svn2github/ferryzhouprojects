#pragma once

#include <vector>
#include <set>
#include <cassert>

namespace fvision {

/**
 * interface
 * each frame has a set of objects, with each object is associated with an id 
 * and each frame is accociated with an id
 *
 * it can be represented by a table with three columns <frameId, objectId, object>
 *
 */
template<typename TObject>
class MultipleFrameIndexedObjectsDatabase {

public:
	virtual ~MultipleFrameIndexedObjectsDatabase<TObject>(void) {
	}

public:
	struct Item {
	public:
		Item() {
		}

		Item(int frameId, int objectId, const TObject& object) {
			this->frameId = frameId;
			this->objectId = objectId;
			this->object = object;
		}

		int frameId;
		int objectId;
		TObject object;
	};

	class ItemIterator {
	public:
		virtual bool hasNext() = 0;
		virtual Item* next() = 0;
	};

	class ConstItemIterator {
	public:
		virtual bool hasNext() = 0;
		virtual const Item* next() = 0;
	};

	virtual ItemIterator* getItemIterator() = 0;
	virtual ConstItemIterator* getItemIterator() const = 0;

public:

	virtual size_t size() const = 0;

	virtual std::set<int> getAllFrameIds() const = 0;

	std::vector<int> getAllFrameIdVector() const {
		std::set<int> idSet = getAllFrameIds();
		std::vector<int> idVector(idSet.begin(), idSet.end());
		return idVector;
	}

	virtual std::set<int> getAllObjectIds() const = 0;

	virtual std::set<int> getAllObjectIdsInFrame(int frameId) const = 0;

	std::vector<int> getAllObjectIdVectorInFrame(int frameId) const {
		std::set<int> idSet = getAllObjectIdsInFrame(frameId);
		std::vector<int> idVector(idSet.begin(), idSet.end());
		return idVector;
	}

	virtual std::vector<TObject> getAllObjectsInFrame(int frameId) const = 0;

	virtual void getIndexedObjectsByFrameId(
		int frameId, 
		std::vector<int>& objectIds, 
		std::vector<TObject>& objects) const = 0;

	virtual void getIndexedObjectsByObjectId(
		int objectId, 
		std::vector<int>& frameIds, 
		std::vector<TObject>& objects) const = 0;

	virtual size_t getObjectFrequence(int objectId) = 0;

	virtual std::set<int> getAllFrameIdsByObjectId(int objectId) const = 0;

	///assert all object ids can be found
	virtual std::vector<TObject> getObjectsSubsetInFrame(
		int frameId, 
		const std::vector<int>& objectIds) const = 0;

	virtual std::vector<TObject> getObjectsSubsetByObjectId(
		int objectId, 
		const std::vector<int>& frameIds) const {
		std::vector<TObject> objects;
		for (size_t i = 0; i < frameIds.size(); i++) {
			objects.push_back(getObjectByFrameIdAndObjectId(frameIds[i], objectId));
		}
		return objects;
	}

	virtual bool itemExists(int frameId, int objectId) const = 0;

	///return false if <frameId, objectId> does not exist
	virtual bool getObjectByFrameIdAndObjectId(int frameId, int objectId, TObject& object) const = 0;

	///assert <frameId, objectId> can be found
	virtual TObject getObjectByFrameIdAndObjectId(int frameId, int objectId) const = 0;

public:
	///return false if the item of <frameId, objectId> exists
	virtual bool insertItem(int frameId, int objectId, const TObject& object) = 0;

	virtual bool insertItem(const Item* item) {
		return insertItem(item->frameId, item->objectId, item->object);
	}

	virtual void insertOrUpdateItem(int frameId, int objectId, const TObject& object) {
		if (itemExists(frameId, objectId)) updateItem(frameId, objectId, object);
		else insertItem(frameId, objectId, object);
	}

	virtual void insertOrUpdateItemsByFrame(int frameId, const std::vector<int> objectIds, const std::vector<TObject>& objects) {
		assert (objectIds.size() == objects.size());
		for (size_t i = 0; i < objectIds.size(); i++) {
			insertOrUpdateItem(frameId, objectIds[i], objects[i]);
		}
	}

	virtual void insertOrUpdateItemsByObject(int objectId, const std::vector<int> frameIds, const std::vector<TObject>& objects) {
		assert (frameIds.size() == objects.size());
		for (size_t i = 0; i < frameIds.size(); i++) {
			insertOrUpdateItem(frameIds[i], objectId, objects[i]);
		}
	}

	///if failed, then no item is inserted; otherwise, all items inserted
	virtual bool insertItems(int frameId, const std::vector<int> objectIds, const std::vector<TObject>& objects) {
		assert (objectIds.size() == objects.size());
		for (size_t i = 0; i < objectIds.size(); i++) {
			if (itemExists(frameId, objectIds[i])) return false;
		}
		for (size_t i = 0; i < objectIds.size(); i++) {
			insertItem(frameId, objectIds[i], objects[i]);
		}
		return true;
	}

	///return false if the item of <frameId, objectId> does not exist
	virtual bool deleteItem(int frameId, int objectId) = 0;

	virtual void deleteFrame(int frameId) {
		std::set<int> objectIds = getAllObjectIdsInFrame(frameId);
		std::set<int>::iterator iter = objectIds.begin();
		for ( ; iter != objectIds.end(); iter++) {
			deleteItem(frameId, *iter);
		}
	}

	virtual void deleteObject(int objectId) {
		std::set<int> frameIds = getAllFrameIdsByObjectId(objectId);
		std::set<int>::iterator iter = frameIds.begin();
		for ( ; iter != frameIds.end(); iter++) {
			deleteItem(*iter, objectId);
		}
	}

	///return false if the item of <frameId, objectId> does not exist
	virtual bool updateItem(int frameId, int objectId, const TObject& object) = 0;

	virtual void clear() = 0;

public:
	virtual std::string getErrorMessage() const = 0;

public:
	//virtual void readFromStream(std::istream& is) = 0;

	//virtual void writeToStream(std::ostream& os) const = 0;
};

}
#pragma once

#include "IndexedObjectsDatabase.h"
#include "MultipleFrameIndexedObjectsDatabase.h"

#include <iostream>

namespace fvision {

template<typename TObject>
std::ostream& operator<< (std::ostream& os, const IndexedObjectsDatabase<TObject>* db) {
	os<<db->size()<<std::endl;
	IndexedObjectsDatabase<TObject>::ConstItemIterator* iter = db->getItemIterator();
	while (iter->hasNext()) {
		const IndexedObjectsDatabase<TObject>::Item* item = iter->next();
		os<<item->id<<" "<<item->object<<std::endl;
	}
	return os;
}

template<typename TObject>
std::ostream& operator<< (std::ostream& os, const MultipleFrameIndexedObjectsDatabase<TObject>* db) {
	os<<db->size()<<std::endl;
	MultipleFrameIndexedObjectsDatabase<TObject>::ConstItemIterator* iter = db->getItemIterator();
	while (iter->hasNext()) {
		const MultipleFrameIndexedObjectsDatabase<TObject>::Item* item = iter->next();
		os<<item->frameId<<" "<<item->objectId<<" "<<item->object<<std::endl;
	}
	return os;
}

template<typename TObject>
std::istream& operator>> (std::istream& is, IndexedObjectsDatabase<TObject>* db) {
	int size = 0;
	is>>size;
	IndexedObjectsDatabase<TObject>::Item item;
	for (int i = 0; i < size; i++) {
		is>>item.id>>item.object;
		db->insertItem(&item);
	}
	return is;
}

template<typename TObject>
std::istream& operator>> (std::istream& is, MultipleFrameIndexedObjectsDatabase<TObject>* db) {
	int size = 0;
	is>>size;
	MultipleFrameIndexedObjectsDatabase<TObject>::Item item;
	for (int i = 0; i < size; i++) {
		is>>item.frameId>>item.objectId>>item.object;
		db->insertItem(&item);
	}
	return is;
}


}
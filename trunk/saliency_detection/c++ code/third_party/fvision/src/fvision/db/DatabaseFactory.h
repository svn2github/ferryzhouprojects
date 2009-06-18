#pragma once

#include "IndexedObjectsDatabase.h"
#include "MultipleFrameIndexedObjectsDatabase.h"

#include "impl/IndexedObjectsDatabaseMapImpl.h"
#include "impl/MultipleFrameIndexedObjectsDatabaseMapImpl.h"

namespace fvision {

class DatabaseFactory {
public:
	template<typename TObject>
	IndexedObjectsDatabase<TObject>* createIndexedObjectsDatabase(){
		return new IndexedObjectsDatabaseMapImpl<TObject>();
	}

	template<typename TObject>
	MultipleFrameIndexedObjectsDatabase<TObject>* createMultipleFrameIndexedObjectsDatabase() {
		return new MultipleFrameIndexedObjectsDatabaseMapImpl<TObject>();
	}
};

}
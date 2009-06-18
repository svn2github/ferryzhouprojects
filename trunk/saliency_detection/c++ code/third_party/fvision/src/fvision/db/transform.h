#pragma once

#include "IndexedObjectsDatabase.h"
#include "MultipleFrameIndexedObjectsDatabase.h"

namespace fvision {

template <typename TDB, typename UnaryOperator>
void modifyTransformDatabase(TDB* db, const UnaryOperator& op) {
	TDB::ItemIterator* iter = db->getItemIterator();
	while (iter->hasNext()) {
		TDB::Item* item = iter->next();
		item->object = op(item->object);
	}
}

template <typename TObject, typename FuncObject>
void doForEachDBItem(const IndexedObjectsDatabase<TObject>* db, FuncObject* fo) {
	IndexedObjectsDatabase<TObject>::ConstItemIterator* iter = db->getItemIterator();
	while (iter->hasNext()) {
		const IndexedObjectsDatabase<TObject>::Item* item = iter->next();
		fo->doOnItem(item->id, item->object);
	}
}

template <typename TObject, typename FuncObject>
void doForEachDBItem(const MultipleFrameIndexedObjectsDatabase<TObject>* db, FuncObject* fo) {
	MultipleFrameIndexedObjectsDatabase<TObject>::ConstItemIterator* iter = db->getItemIterator();
	while (iter->hasNext()) {
		const MultipleFrameIndexedObjectsDatabase<TObject>::Item* item = iter->next();
		fo->doOnItem(item->frameId, item->objectId, item->object);
	}
}

template <typename TDB>
void copyTransformDatabase(const TDB* src, TDB* dst) {
	TDB::ConstItemIterator* iter = src->getItemIterator();
	while (iter->hasNext()) {
		const TDB::Item* item = iter->next();
		dst->insertItem(item);
	}
}

template <typename TDB>
void copyDatabase(const TDB* src, TDB* dst) {
	TDB::ConstItemIterator* iter = src->getItemIterator();
	while (iter->hasNext()) {
		const TDB::Item* item = iter->next();
		dst->insertItem(item);
	}
}

template <typename TObject>
void copyDatabase(const MultipleFrameIndexedObjectsDatabase<TObject>* src, 
				  const std::set<int>& frameIds,
				  MultipleFrameIndexedObjectsDatabase<TObject>* dst)
{
	std::set<int>::const_iterator iter = frameIds.begin();
	for (; iter != frameIds.end(); iter++) {
		int frameId = *iter;
		vector<int> objectIds;
		vector<TObject> objects;
		src->getIndexedObjectsByFrameId(frameId, objectIds, objects);
		dst->insertItems(frameId, objectIds, objects);
	}
}

template <typename TInputObject, typename TOutputObject, typename UnaryOperator>
void copyTransformDatabase(
	const MultipleFrameIndexedObjectsDatabase<TInputObject>* src, 
	MultipleFrameIndexedObjectsDatabase<TOutputObject>* dst, 
	const UnaryOperator& op) {

	typedef MultipleFrameIndexedObjectsDatabase<TInputObject> TInputDB;
	typedef MultipleFrameIndexedObjectsDatabase<TOutputObject> TOutputDB;

	TInputDB::ConstItemIterator* iter = src->getItemIterator();
	while (iter->hasNext()) {
		const TInputDB::Item* item = iter->next();
		TOutputObject output = op(item->object);
		dst->insertItem(item->frameId, item->objectId, output);
	}
}

template <typename TInputObject, typename TOutputObject, typename UnaryOperator>
void copyTransformDatabase(
	const IndexedObjectsDatabase<TInputObject>* src, 
	IndexedObjectsDatabase<TOutputObject>* dst, 
	const UnaryOperator& op) 
{
	typedef IndexedObjectsDatabase<TInputObject> TInputDB;
	typedef IndexedObjectsDatabase<TOutputObject> TOutputDB;

	TInputDB::ConstItemIterator* iter = src->getItemIterator();
	while (iter->hasNext()) {
		const TInputDB::Item* item = iter->next();
		TOutputObject output = op(item->object);
		dst->insertItem(item->id, output);
	}
}

template <typename TObject, typename UnaryOperator>
void modifyTransformFrame (
	MultipleFrameIndexedObjectsDatabase<TObject>* db,
	const UnaryOperator& op,
	int frameId ) 
{
	std::vector<int> objectIds;
	std::vector<TObject> objects;
	db->getIndexedObjectsByFrameId(frameId, objectIds, objects);
	for (size_t i = 0; i < objectIds.size(); i++) {
		int objectId = objectIds[i];
		TObject nObject = op(objects[i]);
		db->updateItem(frameId, objectId, nObject);
	}
}

/**
*	transform all frames
*	different frames has different transformations
*	@see UnaryOperatorGenerator
*/
template <typename TObject, typename TFrameObject, typename UnaryOperatorGenerator>
void modifyTransformFrames (
	MultipleFrameIndexedObjectsDatabase<TObject>* db,
	const IndexedObjectsDatabase<TFrameObject>* frameObjects,
	const UnaryOperatorGenerator* generator) 
{
	IndexedObjectsDatabase<TFrameObject>::ConstItemIterator* iter = frameObjects->getItemIterator();
	while (iter->hasNext()) {
		const IndexedObjectsDatabase<TFrameObject>::Item* item = iter->next();
		int frameId = item->id;
		modifyTransformFrame(db, generator->generate(item->object), frameId);
	}
}

template <typename TObject, typename UnaryOperator>
void modifyTransformFrameWithPointer (
	MultipleFrameIndexedObjectsDatabase<TObject>* db,
	UnaryOperator* op,
	int frameId ) 
{
	std::vector<int> objectIds;
	std::vector<TObject> objects;
	db->getIndexedObjectsByFrameId(frameId, objectIds, objects);
	for (size_t i = 0; i < objectIds.size(); i++) {
		int objectId = objectIds[i];
		TObject nObject = op->operator()(objects[i]);
		db->updateItem(frameId, objectId, nObject);
	}
	delete op;
}

template <typename TObject, typename TFrameObject, typename UnaryOperatorGenerator>
void modifyTransformFramesWithPointer (
	MultipleFrameIndexedObjectsDatabase<TObject>* db,
	const IndexedObjectsDatabase<TFrameObject>* frameObjects,
	const UnaryOperatorGenerator* generator) 
{

	IndexedObjectsDatabase<TFrameObject>::ConstItemIterator* iter = frameObjects->getItemIterator();
	while (iter->hasNext()) {
		const IndexedObjectsDatabase<TFrameObject>::Item* item = iter->next();
		int frameId = item->id;
		modifyTransformFrameWithPointer(db, generator->generate(item->object), frameId);
	}
}

/**
*	get all objects in a certain frame of src
*  transform
*	insert transformed objects to dst
*/
template <typename TObject, typename UnaryOperator>
void copyTransformFrame(
	const MultipleFrameIndexedObjectsDatabase<TObject>* src,
	MultipleFrameIndexedObjectsDatabase<TObject>* dst,
	const UnaryOperator& op,
	int frameId ) 
{
	std::vector<int> objectIds;
	std::vector<TObject> objects;
	src->getIndexedObjectsByFrameId(frameId, objectIds, objects);
	for (size_t i = 0; i < objectIds.size(); i++) {
		int objectId = objectIds[i];
		TObject nObject = op(objects[i]);
		dst->insertItem(frameId, objectId, nObject);
	}
}


template <typename TObject, typename UnaryOperator>
void copyTransformFrameWithPointer (
	const MultipleFrameIndexedObjectsDatabase<TObject>* src,
	MultipleFrameIndexedObjectsDatabase<TObject>* dst,
	UnaryOperator* op,
	int frameId ) 
{
	std::vector<int> objectIds;
	std::vector<TObject> objects;
	src->getIndexedObjectsByFrameId(frameId, objectIds, objects);
	for (size_t i = 0; i < objectIds.size(); i++) {
		int objectId = objectIds[i];
		TObject nObject = op->operator()(objects[i]);
		dst->insertItem(frameId, objectId, nObject);
	}
	delete op;
}

template <typename TObject, typename TFrameObject, typename UnaryOperatorGenerator>
void copyTransformFramesWithPointer (
	const MultipleFrameIndexedObjectsDatabase<TObject>* src,
	MultipleFrameIndexedObjectsDatabase<TObject>* dst,
	const IndexedObjectsDatabase<TFrameObject>* frameObjects,
	const UnaryOperatorGenerator* generator) 
{

	IndexedObjectsDatabase<TFrameObject>::ConstItemIterator* iter = frameObjects->getItemIterator();
	while (iter->hasNext()) {
		const IndexedObjectsDatabase<TFrameObject>::Item* item = iter->next();
		int frameId = item->id;
		copyTransformFrameWithPointer(src, dst, generator->generate(item->object), frameId);
	}
}

}
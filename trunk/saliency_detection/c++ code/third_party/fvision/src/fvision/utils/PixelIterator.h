#pragma once

#include "CvPointIterator.h"

namespace fvision {

class PixelIterator {
public:
	virtual bool hasNext() const = 0;
	virtual uchar* next() = 0;
};

class ConstPixelIterator {
public:
	virtual bool hasNext() const = 0;
	virtual const uchar* next() = 0;
};

class PointBasedPixelIterator : public PixelIterator {
public:
	PointBasedPixelIterator(IplImage* im, CvPointIterator* iter);

public:
	bool hasNext() const;
	uchar* next();
	const CvPoint& getPoint();

private:
	IplImage* im;
	CvPointIterator* iter;
	CvPoint p;
};

class ConstPointBasedPixelIterator : public ConstPixelIterator {
public:
	ConstPointBasedPixelIterator(const IplImage* im, CvPointIterator* iter);

public:
	bool hasNext() const;
	const uchar* next();
	const CvPoint& getPoint();

private:
	const IplImage* im;
	CvPointIterator* iter;
	CvPoint p;
};

class ImagePixelIterator : public PixelIterator {
public:
	ImagePixelIterator(IplImage* im);

public:
	bool hasNext() const;
	uchar* next();
	const CvPoint& getPoint();

private:
	IplImage* im;
	CvPoint p;
	uchar* pdata;
	uchar* pdataRowBegin;
};

class ConstImagePixelIterator : public ConstPixelIterator {
public:
	ConstImagePixelIterator(const IplImage* im);

public:
	bool hasNext() const;
	const uchar* next();
	const CvPoint& getPoint();

private:
	const IplImage* im;
	CvPoint p;
	const uchar* pdata;
	const uchar* pdataRowBegin;
};

}
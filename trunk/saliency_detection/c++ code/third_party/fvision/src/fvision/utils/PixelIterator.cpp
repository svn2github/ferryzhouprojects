#include "PixelIterator.h"

#include <cassert>

namespace fvision {

PointBasedPixelIterator::PointBasedPixelIterator(IplImage* im, CvPointIterator* iter) {
	assert(im != NULL);
	assert(iter != NULL);
	this->im = im;
	this->iter = iter;
}

bool PointBasedPixelIterator::hasNext() const {
	return iter->hasNext();
}

uchar* PointBasedPixelIterator::next() {
	p = iter->next();
	return (uchar*)im->imageData + p.y * im->widthStep + p.x * im->nChannels;
}

const CvPoint& PointBasedPixelIterator::getPoint() {
	return p;
}

ConstPointBasedPixelIterator::ConstPointBasedPixelIterator(const IplImage* im, CvPointIterator* iter) {
	assert(im != NULL);
	assert(iter != NULL);
	this->im = im;
	this->iter = iter;
}

bool ConstPointBasedPixelIterator::hasNext() const {
	return iter->hasNext();
}

const uchar* ConstPointBasedPixelIterator::next() {
	p = iter->next();
	return (const uchar*)im->imageData + p.y * im->widthStep + p.x * im->nChannels;
}

const CvPoint& ConstPointBasedPixelIterator::getPoint() {
	return p;
}



ImagePixelIterator::ImagePixelIterator(IplImage* im) {
	assert(im != NULL);
	this->im = im;
	p.x = 0;
	p.y = 0;
	pdataRowBegin = (uchar*)im->imageData;
	pdata = pdataRowBegin;
}

bool ImagePixelIterator::hasNext() const {
	return (p.y < im->height) && (p.x < im->width);
}

uchar* ImagePixelIterator::next() {
	p.x++;
	pdata += im->nChannels;
	if (p.x == im->width) {
		p.y++;
		p.x = 0;
		pdataRowBegin += im->widthStep;
		pdata = pdataRowBegin;
	}	
	return pdata;
}

const CvPoint& ImagePixelIterator::getPoint() {
	return p;
}

ConstImagePixelIterator::ConstImagePixelIterator(const IplImage* im) {
	assert(im != NULL);
	this->im = im;
	p.x = 0;
	p.y = 0;
	pdataRowBegin = (const uchar*)im->imageData;
	pdata = pdataRowBegin;
}

bool ConstImagePixelIterator::hasNext() const {
	return (p.y < im->height) && (p.x < im->width);
}

const uchar* ConstImagePixelIterator::next() {
	p.x++;
	pdata += im->nChannels;
	if (p.x == im->width) {
		p.y++;
		p.x = 0;
		pdataRowBegin += im->widthStep;
		pdata = pdataRowBegin;
	}	
	return pdata;
}

const CvPoint& ConstImagePixelIterator::getPoint() {
	return p;
}

}
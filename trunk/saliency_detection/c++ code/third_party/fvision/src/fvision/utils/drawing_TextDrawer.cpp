#include "drawing_TextDrawer.h"

namespace fvision {

TextDrawer::TextDrawer(const CvFont& font, const CvScalar& color) {
	this->font = font;
	this->color = color;
}

TextDrawer::TextDrawer() {
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4f, 0.4f);
	this->color = CV_RGB(0, 255, 0);
}

TextDrawer::~TextDrawer(void) {
}

void TextDrawer::draw(IplImage* im, const CvPoint& position, const char* text) const {
	cvPutText(im, text, position, &font, color);
}

void TextDrawer::draw(IplImage* im, const CvPoint2D32f& position, const char* text) const {
	draw(im, cvPointFrom32f(position), text);
}
void TextDrawer::setColor(const CvScalar& color) {
	this->color = color;
}
}
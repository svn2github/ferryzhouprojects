#include "SaliencyBarWidget.h"

#include <cassert>

SaliencyBarWidget::SaliencyBarWidget(QWidget *parent) : QWidget(parent) {
	rowHeight = defaultRowHeight;
	barWidth = defaultBarWidth;
	saliencyEvents = NULL;
}

SaliencyBarWidget::~SaliencyBarWidget(void) {
}

QSize SaliencyBarWidget::sizeHint() const {
		
}

void SaliencyBarWidget::setSaliencyEvents(SaliencyEvents* events) {
	assert(events != NULL);
	this->saliencyEvents = events;
	initializeOnNewSaliencyEvents();
}

void SaliencyBarWidget::eventPicked(int frameIndex, int eventType) {
	
}

void SaliencyBarWidget::setFrame(int frameIndex) {

}
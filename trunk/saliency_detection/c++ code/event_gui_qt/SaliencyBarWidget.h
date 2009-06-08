#ifndef SALIENCY_BAR_WIDGET_H_
#define SALIENCY_BAR_WIDGET_H_

#include "SaliencyEvents.h"

#include <QColor>
#include <QImage>
#include <QWidget>

//like a slider
//background is the saliency image
//the saliency image can not be resized
//containts a set of saliency types
//start frame
//end frame
//events data, a set of range lists
//a ruler bar on the head, showing time and current index
//
class SaliencyBarWidget : public QWidget {

	Q_OBJECT

public:
	SaliencyBarWidget(QWidget *parent = 0);

	QSize sizeHint() const;

	void setSaliencyEvents(SaliencyEvents* events);

signals:
	void eventPicked(int frameIndex, int eventType);

slots:
	void setFrame(int frameIndex);

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	void initializeOnNewSaliencyEvents();
	
private:
	SaliencyEvents* saliencyEvents;
	QImage backgroundImage;
	int frameIndex;

	QRect saliencyViewRegion;

	int rowHeight;
	int barWidth;

	static const int defaultRowHeight = 20;
	static const int defaultBarWidth = 500;
};

#endif // SALIENCY_BAR_WIDGET_H_

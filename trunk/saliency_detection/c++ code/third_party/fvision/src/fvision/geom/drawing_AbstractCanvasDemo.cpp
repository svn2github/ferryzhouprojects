#include "drawing_AbstractCanvasDemo.h"

namespace fvision {

AbstractCanvasDemo::AbstractCanvasDemo(const CvSize& imSize, float scale) {
	canvas = new DemoCanvas();
	canvas->setUp(imSize, scale);
}

AbstractCanvasDemo::AbstractCanvasDemo() {
	canvas = new DemoCanvas();
	canvas->setUp(cvSize(500, 500), 100.0);
}

AbstractCanvasDemo::~AbstractCanvasDemo(void) {
	delete canvas;
}

void AbstractCanvasDemo::setDemoHelper(const ImageDemoHelper& demoHelper) {
	this->demoHelper = demoHelper;
}

void AbstractCanvasDemo::setCanvas(const CvSize& imSize, float scale) {
	canvas->setUp(imSize, scale);
}
}
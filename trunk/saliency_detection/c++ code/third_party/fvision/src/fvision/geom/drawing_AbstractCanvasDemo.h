#pragma once

#include "drawing_DemoCanvas.h"

#include <fvision/utils.h>

namespace fvision {

class AbstractCanvasDemo {
public:
	AbstractCanvasDemo(); //default size = (500, 500), scale = 10.0
	AbstractCanvasDemo(const CvSize& imSize, float scale);
	virtual ~AbstractCanvasDemo(void);

public:
	void setCanvas(const CvSize& imSize, float scale);
	void setDemoHelper(const ImageDemoHelper& demoHelper);

public:
	DemoCanvas* canvas;
	ImageDemoHelper demoHelper;
};

}
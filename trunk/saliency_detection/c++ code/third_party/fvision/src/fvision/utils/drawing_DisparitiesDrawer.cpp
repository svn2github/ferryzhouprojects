#include "drawing_DisparitiesDrawer.h"

#include "drawing.h"
#include "drawing_PointDrawer.h"
#include "drawing_LineDrawer.h"
#include "Colors.h"

using namespace std;

namespace fvision {

DisparitiesDrawer::DisparitiesDrawer(void) {
	pd = new PointDrawer("o");
	ld = new LineDrawer(Colors::RED);
}

DisparitiesDrawer::~DisparitiesDrawer(void) {
	delete pd;
	delete ld;
}

}
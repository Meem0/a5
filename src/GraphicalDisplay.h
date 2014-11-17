#ifndef GRAPHICAL_DISPLAY_H
#define GRAPHICAL_DISPLAY_H

#include "Display.h"
//#include "window.h"

class GraphicalDisplay: public Display {
public:
	// GraphicalDisplay(Board*, XWindow*)

private:
	// does nothing
	void drawInfo();

	void draw(Pos);

	//XWindow* _window;
};

#endif

#ifndef GRAPHICAL_DISPLAY_H
#define GRAPHICAL_DISPLAY_H

#include "Display.h"
//#include "window.h"

class GraphicalDisplay: public Display {
public:
	// GraphicalDisplay(Board*, XWindow*);

	// draws squares that have been updated since the previous draw
	void draw();

	~GraphicalDisplay();

private:
	// two-dimensional array of pointers to Squares
	// directly compares the memory addresses to the ones in Board
	//   to see if a Square has changed and should be redrawn
	Square*** _lastDraw;

	//XWindow* _window;
};

#endif

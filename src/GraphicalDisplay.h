#ifndef GRAPHICAL_DISPLAY_H
#define GRAPHICAL_DISPLAY_H

#include "BoardDisplay.h"
#include "GraphicsRedirect.h"

class Square;

class GraphicalDisplay: public BoardDisplay {
public:
	// opens the window
	GraphicalDisplay(Board*);

	// draws squares that have been updated since the previous draw
	void draw();

	static const int WINDOW_WIDTH = 500;
	static const int WINDOW_HEIGHT = 500;

private:
	Xwindow _window;
};

#endif

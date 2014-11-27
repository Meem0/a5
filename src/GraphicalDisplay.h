#ifndef GRAPHICAL_DISPLAY_H
#define GRAPHICAL_DISPLAY_H

#include "BoardDisplay.h"
#include "GraphicsRedirect.h"

class Square;

class GraphicalDisplay: public BoardDisplay {
public:
	// opens the window
	GraphicalDisplay(Board*, Score*);

	// draws squares that have been updated since the previous draw
	void draw();

	static const int WINDOW_WIDTH = 500;
	static const int WINDOW_HEIGHT = 500;

	~GraphicalDisplay();

private:
	// two-dimensional array of pointers to Squares
	// directly compares the memory addresses to the ones in Board
	//   to see if a Square has changed and should be redrawn
	Square*** _lastDraw;
	
	Xwindow _window;
};

#endif

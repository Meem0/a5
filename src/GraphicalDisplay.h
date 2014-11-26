#ifndef GRAPHICAL_DISPLAY_H
#define GRAPHICAL_DISPLAY_H

#include "BoardDisplay.h"
#include "GraphicsRedirect.h"

class Square;

class GraphicalDisplay: public BoardDisplay {
public:
	GraphicalDisplay(Board*, Score*);

	// draws squares that have been updated since the previous draw
	void draw();

	~GraphicalDisplay();

private:
	// two-dimensional array of pointers to Squares
	// directly compares the memory addresses to the ones in Board
	//   to see if a Square has changed and should be redrawn
	Square*** _lastDraw;
	
	static const int WINDOW_WIDTH = 500;
	static const int WINDOW_HEIGHT = 500;
	Xwindow _window;
};

#endif

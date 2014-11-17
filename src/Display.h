#ifndef DISPLAY_H
#define DISPLAY_H

#include "Board.h"


class Display {
public:
	Display(Board*);

	// calls drawInfo(), then calls draw(Pos) on all the squares
	//   that are different from the last time board was drawn
	void draw();

	virtual ~Display() = 0;

protected:
	virtual void drawInfo() = 0;
	virtual void draw(Pos) = 0;

	// two-dimensional array of pointers to Squares
	// directly compares the memory addresses to the ones in Board
	//   to see if a Square has changed and should be redrawn
	Square*** _lastDraw;
	Board* _board;
};

#endif

#ifndef SQUARE_H
#define SQUARE_H

#include "Pos.h"

class Board;

class Square {
public:
	enum Colour { EMPTY = -1, WHITE = 0, RED, GREEN, BLUE };

	// construct a square with a position and colour
	Square(Pos, Colour);

	Pos getPos() const;
	Colour getColour() const;

	void setPos(Pos);
	void setBoard(Board*);

	// calls destroy on all squares affected by this square's ability
	// increases numDestroyed by 1
	// replaces this square with an empty square
	virtual void destroy(int& numDestroyed, int matchSize);

	virtual void textDraw() const;
	//virtual void graphicalDraw(XWindow*) const;

	virtual ~Square();

protected:
	Pos _pos;
	Colour _colour;

	Board* _board;
};

#endif

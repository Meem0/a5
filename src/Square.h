#ifndef SQUARE_H
#define SQUARE_H

#include "Pos.h"

class Board;
class Xwindow;

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

	void textDraw() const;
	void graphicalDraw(Xwindow*) const;

	virtual ~Square();

protected:
	Pos _pos;
	Colour _colour;

	Board* _board;

	bool _dontDestroy;

private:
	// draw the character representing the square's special attribute
	virtual void textDrawSpecial() const;

	// draw the decoration for the square's special attribute
	virtual void graphicalDrawSpecial(Xwindow*) const;
};

#endif

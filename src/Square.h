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
	bool getModified() const;

	void setPos(Pos);
	void setBoard(Board*);
	void setModified(bool);

	// calls destroy on all squares affected by this square's ability
	// increases numDestroyed by 1
	// replaces this square with an empty square
	virtual void destroy(int& numDestroyed, int matchSize);

	// prints an "l" if the cell containing this square is locked ("_" otherwise)
	// calls textDrawSpecial to print a character representing this square's
	//   special attribute
	// prints the number corresponding to this square's colour
	void textDraw() const;

	// draws a rectangle whose colour depends on the colour of this square
	// calls graphicalDrawSpecial to draw some graphics representing this
	//   square's special attribute
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

	// for GraphicalDisplay to know whether to redraw
	bool _modified;
};

#endif

#ifndef UPRIGHT_SQUARE_H
#define UPRIGHT_SQUARE_H

#include "Square.h"

class UprightSquare : public Square {
public:
	UprightSquare(Pos p, Colour c);

	// destroys all the squares on the same column as this square
	void destroy(int& numDestroyed, int matchSize);

private:
	// prints a "v"
	void textDrawSpecial() const;

	// draws a vertical magenta bar across the middle of this square
	void graphicalDrawSpecial(Xwindow*) const;
};

#endif

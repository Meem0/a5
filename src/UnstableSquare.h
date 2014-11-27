#ifndef UNSTABLE_SQUARE_H
#define UNSTABLE_SQUARE_H

#include "Square.h"

class UnstableSquare : public Square {
public:
	UnstableSquare(Pos p, Colour c);

	// destroys a box of radius 1 or 2 centred on this square
	// radius = 1 if matchSize = 3, radius = 2 otherwise
	void destroy(int& numDestroyed, int matchSize);

private:
	// prints a "b"
	void textDrawSpecial() const;

	// draws a magenta box in the middle of this square
	void graphicalDrawSpecial(Xwindow*) const;
};

#endif

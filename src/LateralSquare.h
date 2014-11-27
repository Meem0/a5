#ifndef LATERAL_SQUARE_H
#define LATERAL_SQUARE_H

#include "Square.h"

class LateralSquare : public Square {
public:
	LateralSquare(Pos p, Colour c);
	
	// destroys all the squares on the same row as this square
	void destroy(int& numDestroyed, int matchSize);

private:
	// prints an "h"
	void textDrawSpecial() const;

	// draws a horizontal magenta bar across the middle of this square
	void graphicalDrawSpecial(Xwindow*) const;
};

#endif

#ifndef LATERAL_SQUARE_H
#define LATERAL_SQUARE_H

#include "Square.h"

class LateralSquare : public Square {
public:
	LateralSquare(Pos p, Colour c);

	void destroy(int& numDestroyed, int matchSize);
	void textDrawSpecial() const;
	void graphicalDrawSpecial(Xwindow*) const;
};

#endif

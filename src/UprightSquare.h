#ifndef UPRIGHT_SQUARE_H
#define UPRIGHT_SQUARE_H

#include "Square.h"

class UprightSquare : public Square {
public:
	UprightSquare(Pos p, Colour c);

	void destroy(int& numDestroyed, int matchSize);

private:
	void textDrawSpecial() const;
	//void graphicalDraw(XWindow*) const;
};

#endif

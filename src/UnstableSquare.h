#ifndef UNSTABLE_SQUARE_H
#define UNSTABLE_SQUARE_H

#include "Square.h"

class UnstableSquare : public Square {
public:
	UnstableSquare(Pos p, Colour c);

	void destroy(int& numDestroyed, int matchSize);
	void textDraw() const;
	//void graphicalDraw(XWindow*) const;
};

#endif

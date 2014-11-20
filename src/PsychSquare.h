#ifndef PSYCH_SQUARE_H
#define PSYCH_SQUARE_H

#include "Square.h"

class PsychSquare : public Square {
public:
	PsychSquare(Pos p, Colour c);

	void destroy(int& numDestroyed, int matchSize);

private:
	void textDrawSpecial() const;
	//void graphicalDraw(XWindow*) const;
};

#endif

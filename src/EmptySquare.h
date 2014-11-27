#ifndef EMPTY_SQUARE_H
#define EMPTY_SQUARE_H

#include "Square.h"

class EmptySquare : public Square {
public:
	EmptySquare(Pos);

	// does nothing
	void destroy(int& numDestroyed, int matchSize);
};

#endif

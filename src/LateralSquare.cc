#include "LateralSquare.h"
#include "Board.h"
#include <iostream>

LateralSquare::LateralSquare(Pos p, Colour c)
	: Square(p, c) { }

void LateralSquare::destroy(int& numDestroyed, int matchSize) {
	if (_dontDestroy)
		return;

	// avoid infinite recursion between two special squares
	_dontDestroy = true;

	int cols = _board->getSize().col;

	// loop through all the squares on this row
	for(Pos current(_pos.row, 0); current.col < cols; current.col++) {
		// destroy each cell on the row
		_board->getSquare(current)->destroy(numDestroyed, matchSize);
	}

	_dontDestroy = false;

	// now that the special effect has been applied,
	//   destroy this square normally
	Square::destroy(numDestroyed, matchSize);
}

void LateralSquare::textDrawSpecial() const {
	std::cout << "h";
}

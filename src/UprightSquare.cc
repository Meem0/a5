#include "UprightSquare.h"
#include "Board.h"
#include <iostream>

UprightSquare::UprightSquare(Pos p, Colour c)
	: Square(p, c) { }

void UprightSquare::destroy(int& numDestroyed, int matchSize) {
	if (_dontDestroy)
		return;

	// avoid infinite recursion between two special squares
	_dontDestroy = true;

	int rows = _board->getSize().row;

	// loop through all the squares on this column
	for(Pos current(0, _pos.col); current.row < rows; current.row++) {
		// destroy each cell on the column
		_board->getSquare(current)->destroy(numDestroyed, matchSize);
	}

	_dontDestroy = false;

	// now that the special effect has been applied,
	//   destroy this square normally
	Square::destroy(numDestroyed, matchSize);
}

void UprightSquare::textDrawSpecial() const {
	std::cout << "v";
}

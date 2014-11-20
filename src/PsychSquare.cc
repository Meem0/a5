#include "PsychSquare.h"
#include "Board.h"
#include <iostream>

PsychSquare::PsychSquare(Pos p, Colour c)
	: Square(p, c) { }

void PsychSquare::destroy(int& numDestroyed, int matchSize) {
	if (_dontDestroy)
		return;

	// avoid infinite recursion between special squares
	_dontDestroy = true;

	Pos boardSize = _board->getSize();

	// iterate through the entire board
	for (Pos current; current.row < boardSize.row; current.row++) {
		for (current.col = 0; current.col < boardSize.col; current.col++) {
			Square* curSquare = _board->getSquare(current);
			// destroy squares with the same colour as this square
			if (_colour == curSquare->getColour())
				curSquare->destroy(numDestroyed, matchSize);
		}
	}

	_dontDestroy = false;

	// now that the effect has been applied, destroy this square normally
	Square::destroy(numDestroyed, matchSize);
}

void PsychSquare::textDrawSpecial() const {
	std::cout << "p";
}

#include "LateralSquare.h"
#include "Board.h"
#include "GraphicsRedirect.h"
#include "GraphicalDisplay.h"
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

void LateralSquare::graphicalDrawSpecial(Xwindow* window) const {
	Pos boardSize = _board->getSize();
	int width = boardSize.col != 0 ? GraphicalDisplay::WINDOW_WIDTH / boardSize.col : 0;
	int height = boardSize.row != 0 ? GraphicalDisplay::WINDOW_HEIGHT / boardSize.row : 0;
	int barHeight = height / 8;

	// draw a horizontal magenta bar along the middle of the square
	window->fillRectangle(width * _pos.col, height * _pos.row + height / 2 - barHeight / 2,
						  width, barHeight, Xwindow::Magenta);
}

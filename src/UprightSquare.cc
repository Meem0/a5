#include "UprightSquare.h"
#include "Board.h"
#include "GraphicsRedirect.h"
#include "GraphicalDisplay.h"
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

void UprightSquare::graphicalDrawSpecial(Xwindow* window) const {
	Pos boardSize = _board->getSize();
	int width = boardSize.col != 0 ? GraphicalDisplay::WINDOW_WIDTH / boardSize.col : 0;
	int height = boardSize.row != 0 ? GraphicalDisplay::WINDOW_HEIGHT / boardSize.row : 0;
	int barWidth = width / 8;

	// draw a vertical magenta bar along the middle of the square
	window->fillRectangle(width * _pos.col + width / 2 - barWidth / 2, height * _pos.row,
						  barWidth, height, Xwindow::Magenta);
}

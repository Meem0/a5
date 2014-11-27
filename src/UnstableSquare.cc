#include "UnstableSquare.h"
#include "Board.h"
#include "GraphicsRedirect.h"
#include "GraphicalDisplay.h"
#include <iostream>

UnstableSquare::UnstableSquare(Pos p, Colour c)
	: Square(p, c) { }

void UnstableSquare::destroy(int& numDestroyed, int matchSize) {
	if (_dontDestroy)
		return;

	// avoid infinite recursion between two special squares
	_dontDestroy = true;

	// decide whether to destroy a 3x3 or 5x5 box
	int blastRadius = matchSize == 3 ? 1 : 2;

	// iterate through the box of squares to destroy
	for (Pos current(_pos.row - blastRadius, _pos.col - blastRadius);
		 current.row <= _pos.row + blastRadius; current.row++) {
		for (current.col = _pos.col - blastRadius;
			 current.col <= _pos.col + blastRadius; current.col++) {
			if (_board->withinBounds(current))
				// destroy the current square
				_board->getSquare(current)->destroy(numDestroyed, matchSize);
		}
	}

	_dontDestroy = false;

	// now that the effect has been applied, destroy this square normally
	Square::destroy(numDestroyed, matchSize);
}

void UnstableSquare::textDrawSpecial() const {
	std::cout << "b";
}

void UnstableSquare::graphicalDrawSpecial(Xwindow* window) const {
	Pos boardSize = _board->getSize();
	int w = boardSize.col != 0 ? GraphicalDisplay::WINDOW_WIDTH / boardSize.col : 0;
	int h = boardSize.row != 0 ? GraphicalDisplay::WINDOW_HEIGHT / boardSize.row : 0;
	int x = w * _pos.col;
	int y = h * _pos.row;
	int boxPaddingX = w / 4;
	int boxPaddingY = h / 4;

	// draw a magenta box in the centre of the square
	window->fillRectangle(x + boxPaddingX, y + boxPaddingY,
						  w - 2 * boxPaddingX, h - 2 * boxPaddingY,
						  Xwindow::Magenta);
}

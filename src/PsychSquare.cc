#include "PsychSquare.h"
#include "Board.h"
#include "GraphicsRedirect.h"
#include "GraphicalDisplay.h"
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

void PsychSquare::graphicalDrawSpecial(Xwindow* window) const {
	Pos boardSize = _board->getSize();
	int width = boardSize.col != 0 ? GraphicalDisplay::WINDOW_WIDTH / boardSize.col : 0;
	int height = boardSize.row != 0 ? GraphicalDisplay::WINDOW_HEIGHT / boardSize.row : 0;
	int barHeight = height / 8;
	int barWidth = width / 8;

	// draw a horizontal magenta bar along the middle of the square
	window->fillRectangle(width * _pos.col, height * _pos.row + height / 2 - barHeight / 2,
						  width, barHeight, Xwindow::Magenta);
	// draw a vertical magenta bar along the middle of the square
	window->fillRectangle(width * _pos.col + width / 2 - barWidth / 2, height * _pos.row,
						  barWidth, height, Xwindow::Magenta);
}

#include "Square.h"
#include "Board.h"
#include "GraphicsRedirect.h"
#include "GraphicalDisplay.h"

#include <iostream>

Square::Square(Pos pos, Colour colour)
	: _pos(pos), _colour(colour), _dontDestroy(false) { }

Pos Square::getPos() const {
	return _pos;
}

Square::Colour Square::getColour() const {
	return _colour;
}

void Square::setPos(Pos pos) {
	_pos = pos;
}

void Square::setBoard(Board* board) {
	_board = board;
}

void Square::destroy(int& numDestroyed, int matchSize) {
	if (_dontDestroy)
		return;

	numDestroyed++;
	_board->removeSquare(_pos);
}

void Square::textDraw() const {
	if (_colour == EMPTY)
		std::cout << "nul";
	else {
		if (_board->isLocked(_pos))
			std::cout << "l";
		else
			std::cout << "_";

		textDrawSpecial();

		std::cout << _colour;
	}
}


void Square::graphicalDraw(Xwindow* window) const {
	Pos boardSize = _board->getSize();
	int w = boardSize.col != 0 ? GraphicalDisplay::WINDOW_WIDTH / boardSize.col : 0;
	int h = boardSize.row != 0 ? GraphicalDisplay::WINDOW_HEIGHT / boardSize.row : 0;
	int x = w * _pos.col;
	int y = h * _pos.row;

	// draw the base square
	window->fillRectangle(x, y, w, h, static_cast<int>(_colour));

	// draw the special attribute of the square
	graphicalDrawSpecial(window);

	// draw the lock around the cell
	if (_board->isLocked(_pos)) {
		int barWidth = w / 16;
		int barHeight = h / 16;

		window->fillRectangle(x, y, w, barHeight, Xwindow::Black);
		window->fillRectangle(x, y, barWidth, h, Xwindow::Black);
		window->fillRectangle(x + w - barWidth, y, barWidth, h, Xwindow::Black);
		window->fillRectangle(x, y + h - barHeight, w, barHeight, Xwindow::Black);
	}
}


void Square::textDrawSpecial() const {
	std::cout << "_";
}


void Square::graphicalDrawSpecial(Xwindow* window) const {
	// basic square has no special attribute to draw
}


Square::~Square() { }

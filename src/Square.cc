#include "Square.h"
#include "Board.h"

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


void Square::textDrawSpecial() const {
	std::cout << "_";
}

Square::~Square() { }

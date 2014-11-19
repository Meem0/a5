#include "Square.h"
#include "Board.h"

#include <iostream>

Square::Square(Pos pos, Colour colour) : _pos(pos), _colour(colour) { }

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
	numDestroyed++;
	_board->removeSquare(_pos);
}

void Square::textDraw() const {
	std::cout << "__";

	if (_colour == EMPTY)
		std::cout << "e";
	else
		std::cout << _colour;
}

Square::~Square() { }

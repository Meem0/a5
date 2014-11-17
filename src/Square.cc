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

void Square::fall(int amount) {
	std::cout << "Square::fall might be unnecessary; don't use it for now" << std::endl;
}

void Square::setBoard(Board* board) {
	_board = board;
}

void Square::destroy(int& numDestroyed, int matchSize) {
	numDestroyed++;
	_board->removeSquare(_pos);
}

void Square::textDraw() const {
	std::cout << "__" << _colour;
}

Square::~Square() { }

#include "Board.h"
#include "EmptySquare.h"

#include <iostream>

Board::Board(int rows, int cols) {
	create(rows, cols);
}


Board::~Board() {
	destroy();
}


Square* Board::getSquare(Pos pos) const {
	return _board[pos.row][pos.col].square;
}


bool Board::isLocked(Pos pos) const {
	return _board[pos.row][pos.col].isLocked;
}


Pos Board::getSize() const {
	return Pos(_rows, _cols);
}


bool Board::withinBounds(Pos pos) const {
	return pos.row >= 0 && pos.row < _rows &&
		   pos.col >= 0 && pos.col < _cols;
}


void Board::addSquare(Square* square) {
	Pos pos = square->getPos();

	delete _board[pos.row][pos.col].square;

	square->setBoard(this);
	_board[pos.row][pos.col].square = square;
}


void Board::swap(Pos pos1, Pos pos2) {
	if (!withinBounds(pos1) || !withinBounds(pos2)) {
		std::cout << "board: tried to swap with invalid pos" << std::endl;
	}

	Square* temp = _board[pos1.row][pos1.col].square;

	_board[pos1.row][pos1.col].square = _board[pos2.row][pos2.col].square;
	_board[pos1.row][pos1.col].square->setPos(pos1);

	_board[pos2.row][pos2.col].square = temp;
	_board[pos2.row][pos2.col].square->setPos(pos2);
}


void Board::setLock(Pos pos, bool isLocked) {
	_board[pos.row][pos.col].isLocked = isLocked;
}


void Board::removeSquare(Pos pos) {
	delete _board[pos.row][pos.col].square;
	_board[pos.row][pos.col].square = new EmptySquare(pos);
	setLock(pos,false);
}


void Board::resize(Pos newSize) {
	destroy();
	create(newSize.row, newSize.col);
}


void Board::create(int rows, int cols) {
	_rows = rows;
	_cols = cols;

	// create the column of rows
	_board = new Cell*[_rows];

	// create the rows
	for (int row = 0; row < _rows; row++) {
		_board[row] = new Cell[_cols];
		
		// create the squares
		for (int col = 0; col < _cols; col++) {
			_board[row][col].square = new EmptySquare(Pos(row, col));
			_board[row][col].isLocked = false;
		}
	}
}


void Board::destroy() {
	for (int row = 0; row < _rows; row++) {
		// delete the squares
		for (int col = 0; col < _cols; col++) {
			delete _board[row][col].square;
			_board[row][col].square = 0;
		}

		// delete the rows
		delete[] _board[row];
		_board[row] = 0;
	}

	// delete the column of rows
	delete[] _board;
	_board = 0;
}

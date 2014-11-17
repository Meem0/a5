#include "Board.h"
#include "EmptySquare.h"

Board::Board(int rows, int cols) : _rows(rows), _cols(cols) {
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

Board::~Board() {
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

Square* Board::getSquare(Pos pos) const {
	return _board[pos.row][pos.col].square;
}

bool Board::isLocked(Pos pos) const {
	return _board[pos.row][pos.col].isLocked;
}

Pos Board::getSize() const {
	return Pos(_rows, _cols);
}

void Board::addSquare(Square* square) {
	Pos pos = square->getPos();

	delete _board[pos.row][pos.col].square;

	_board[pos.row][pos.col].square = square;
}

void Board::lock(Pos pos) {
	_board[pos.row][pos.col].isLocked = true;
}

void Board::removeSquare(Pos pos) {
	delete _board[pos.row][pos.col].square;
	_board[pos.row][pos.col].square = new EmptySquare(pos);
}

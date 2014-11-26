#include "GraphicalDisplay.h"
#include "Board.h"

#include <iostream>

GraphicalDisplay::GraphicalDisplay(Board* board, Score* score)
	: BoardDisplay(board, score), _window(WINDOW_WIDTH, WINDOW_HEIGHT) {
	
	Pos boardSize = _board->getSize();

	// create the array of rows
	_lastDraw = new Square**[boardSize.row];

	// loop through the rows
	for (int row = 0; row < boardSize.row; row++) {

		// create each row
		_lastDraw[row] = new Square*[boardSize.col];

		// initialize each pointer in the row to null
		for (int col = 0; col < boardSize.col; col++)
			_lastDraw[row][col] = NULL;
	}
}


void GraphicalDisplay::draw() {
	Pos boardSize = _board->getSize();

	int squareWidth  = WINDOW_WIDTH  / boardSize.col;
	int squareHeight = WINDOW_HEIGHT / boardSize.row;

	// loop through the board
	for (Pos current; current.row < boardSize.row; current.row++) {
		for (current.col = 0; current.col < boardSize.col; current.col++) {
			// if the current square has been modified since last draw, draw it
			if (_lastDraw[current.row][current.col] != _board->getSquare(current)) {
				_lastDraw[current.row][current.col] = _board->getSquare(current);

				_window.fillRectangle(squareWidth * current.col, squareHeight * current.row,
									  squareWidth, squareHeight,
									  _lastDraw[current.row][current.col]->getColour());
			}
		}
	}
}


GraphicalDisplay::~GraphicalDisplay() {
	Pos boardSize = _board->getSize();

	// delete each row
	for (int row = 0; row < boardSize.row; row++) {
		delete[] _lastDraw[row];
		_lastDraw[row] = NULL;
	}

	// delete the array of rows
	delete[] _lastDraw;
	_lastDraw = NULL;
}

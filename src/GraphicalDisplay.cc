#include "GraphicalDisplay.h"
#include "Board.h"

#include <iostream>

GraphicalDisplay::GraphicalDisplay(Board* board)
	: BoardDisplay(board), _window(WINDOW_WIDTH, WINDOW_HEIGHT) { }


void GraphicalDisplay::draw() {
	Pos boardSize = _board->getSize();

	// loop through the board
	for (Pos current; current.row < boardSize.row; current.row++) {
		for (current.col = 0; current.col < boardSize.col; current.col++) {
			Square* currentSquare = _board->getSquare(current);
			// if the current square has been modified since last draw, draw it
			if (currentSquare->getColour() != Square::EMPTY &&
				currentSquare->getModified()) {
				currentSquare->graphicalDraw(&_window);
				currentSquare->setModified(false);
			}
		}
	}

	int boardWidth = (WINDOW_WIDTH / boardSize.col) * boardSize.col;
	int boardHeight = (WINDOW_HEIGHT / boardSize.row) * boardSize.row;

	// fill in unused edges of the window
	if (boardWidth != WINDOW_WIDTH) {
		_window.fillRectangle(boardWidth, 0, WINDOW_WIDTH - boardWidth, WINDOW_HEIGHT);
	}
	if (boardHeight != WINDOW_HEIGHT) {
		_window.fillRectangle(0, boardHeight, WINDOW_WIDTH, WINDOW_HEIGHT - boardHeight);
	}
}

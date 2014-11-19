#include "DebugDisplay.h"
#include <iostream>

void DebugDisplay::printBoard() {
	if (!_board)
		std::cout << "can't print; board not set" << std::endl;
	else {
		if (!_display)
			_display = new TextDisplay(_board);

		_display->draw();
	}
}

void DebugDisplay::setBoard(Board* board) {
	_board = board;
}

TextDisplay* DebugDisplay::_display = NULL;
Board* DebugDisplay::_board = NULL;

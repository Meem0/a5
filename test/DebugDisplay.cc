#include "DebugDisplay.h"
#include <iostream>

void DebugDisplay::printBoard() {
	if (!_board)
		std::cout << "can't print; board not set" << std::endl;
	else if (!_display) {
			_display = new TextDisplay(_board);
	}
	_display->draw();

	if (_gDisplay != NULL)
		_gDisplay->draw();
}

void DebugDisplay::setBoard(Board* board) {
	_board = board;
}

//temp dtor
void DebugDisplay::tempDtor() {
	delete _display;
}

TextDisplay* DebugDisplay::_display = NULL;
GraphicalDisplay* DebugDisplay::_gDisplay = NULL;
Board* DebugDisplay::_board = NULL;

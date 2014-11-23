#include "DebugDisplay.h"
#include <iostream>

void DebugDisplay::printBoard() {
	if (!_board)
		std::cout << "can't print; board not set or score not set" << std::endl;
	else if (!_display) {
			_display = new TextDisplay(_board,_score);
		}
		_display->draw();
}

void DebugDisplay::setBoard(Board* board) {
	_board = board;
}

void DebugDisplay::setScore(Score* score) {
	_score = score;
}

TextDisplay* DebugDisplay::_display = NULL;
Board* DebugDisplay::_board = NULL;
Score* DebugDisplay::_score = NULL;

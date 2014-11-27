#include "TextDisplay.h"
#include "Board.h"
#include "Score.h"

#include <iostream>

using namespace std;

TextDisplay::TextDisplay(Board* board, Score * score) : BoardDisplay(board,score) { }

void TextDisplay::draw() {
	Pos boardSize = _board->getSize();
	Pos cur(0, 0);
	std::cout << "Score:      " << _score->getScore() << std::endl;
	// loop through the board
	for (cur.row = 0; cur.row < boardSize.row; cur.row++) {
		for (cur.col = 0; cur.col < boardSize.col; cur.col++) {
			_board->getSquare(cur)->textDraw();
			std::cout << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
}

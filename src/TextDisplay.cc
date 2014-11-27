#include "TextDisplay.h"
#include "Board.h"

#include <iostream>

using namespace std;

TextDisplay::TextDisplay(Board* board) : BoardDisplay(board) { }

void TextDisplay::draw() {
	Pos boardSize = _board->getSize();

	// loop through the board
	for (Pos cur; cur.row < boardSize.row; cur.row++) {
		for (cur.col = 0; cur.col < boardSize.col; cur.col++) {
			_board->getSquare(cur)->textDraw();
			cout << " ";
		}
		cout << endl;
	}

	cout << endl;
}

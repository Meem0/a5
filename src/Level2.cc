#include "Level2.h"
#include "Score.h"
#include "Board.h"
#include <cstdlib>

using namespace std;

bool Level2::checkLevelUp() const {
	Pos boardSize = _board->getSize();

	// check score
	if (_score->getScore() < _startScore + 500)
		return false;

	// check locked squares
	for (Pos current; current.row < boardSize.row; current.row++) {
		for (current.col = 0; current.col < boardSize.col; current.col++) {
			if (_board->isLocked(current))
				return false;
		}
	}

	return true;
}


Square* Level2::generateSquare() {
	return new Square(Pos(0, 0), static_cast<Square::Colour>(rand() % 4));
}


void Level2::generateLocked() {
	Pos boardSize = _board->getSize();
	Pos current;
//	cout << "about to generate lockeds" << endl;
	for (current.row = 0; current.row < boardSize.row; current.row++) {
		for (current.col = 0; current.col < boardSize.col; current.col++) {
			// 1/5 chance of generating a locked square
			if (rand() % 5 == 0) {
				_lockedSquares.push_back(current);
			//	cout << "add to lock" << endl;
			}
		}
	}
}

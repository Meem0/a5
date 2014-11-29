#include "Level3.h"
#include "Score.h"
#include "Board.h"
#include <cstdlib>

using namespace std;

bool Level3::checkLevelUp() const {
	Pos boardSize = _board->getSize();

	// check score
	if (_score->getScore() < _startScore + 750)
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


bool Level3::requiresResize(Pos& newSize) const {
	if (_usingScriptFile)
		return false;
	else {
		newSize.row = 8;
		newSize.col = 6;

		return true;
	}
}


int Level3::moveLimit() const {
	return 10;
}


Square* Level3::generateSquare() {
	return new Square(Pos(0, 0), static_cast<Square::Colour>(rand() % 4));
}


void Level3::generateLocked() {
	Pos boardSize = _board->getSize();

	for (Pos current; current.row < boardSize.row; current.row++) {
		for (current.col = 0; current.col < boardSize.col; current.col++) {
			// 1/7 chance of generating a locked square
			if (rand() % 7 == 0) {
				_lockedSquares.push_back(current);
			}
		}
	}
}

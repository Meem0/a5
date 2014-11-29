#include "Level1.h"
#include "Score.h"
#include "LateralSquare.h"
#include "UprightSquare.h"
#include "UnstableSquare.h"
#include "PsychSquare.h"
#include <cstdlib>


bool Level1::checkLevelUp() const {
	return _score->getScore() >= _startScore + 300;
}


Square* Level1::generateSquare() {
	Square* result = NULL;
	Pos pos;
	Square::Colour colour;
	
	// choose colour
	switch (rand() % 6) {
	case 0: colour = Square::GREEN; break; // green: 1/6 chance
	case 1: colour = Square::BLUE;  break; // blue: 1/6 chance
	case 2:
	case 3: colour = Square::WHITE; break; // white: 1/3 chance
	case 4:
	case 5: colour = Square::RED;   break; // red: 1/3 chance
	}

	// 1/35 chance of generating a special square
	if (rand() % 35 == 0) {
		// equal chance for each special square, except PsychSquare
		int randNum = rand() % 13;
		if (randNum < 4)
			result = new LateralSquare(pos, colour);
		else if (randNum < 8)
			result = new UprightSquare(pos, colour);
		else if (randNum < 12)
			result = new UnstableSquare(pos, colour);
		else if (randNum == 12)
			result = new PsychSquare(pos, colour);
	}
	else {
		result = new Square(pos, colour);
	}

	return result;
}

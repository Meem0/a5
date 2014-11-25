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

	// roll a d6
	switch (rand() % 6) {
	case 0: colour = Square::GREEN; break; // green: 1/6 chance
	case 1: colour = Square::BLUE;  break; // blue: 1/6 chance
	case 2:
	case 3: colour = Square::WHITE; break; // white: 1/3 chance
	case 4:
	case 5: colour = Square::RED;   break; // red: 1/3 chance
	}
	//generate a special square
	//every 5th square (deterministic model ; not random)
	if (numSquareGenerated % 5 == 0 && numSquareGenerated != 0) {
		// equal chance for each special square
		// roll a d4
		switch (rand() % 4) {
		case 0: result = new LateralSquare(pos, colour);  break;
		case 1: result = new UprightSquare(pos, colour);  break;
		case 2: result = new UnstableSquare(pos, colour); break;
		case 3: result = new PsychSquare(pos, colour);    break;
		}
	}
	else {
		result = new Square(pos, colour);
	}
	numSquareGenerated++;
	return result;
}

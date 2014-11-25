#include "Level0.h"
#include "Score.h"
#include <cstdlib>

Level0::Level0() {
	// if a script file has not already been set
	//   (through the command-line argument),
	//   use sequence.txt
	/*if (!_usingScriptFile) {
		initializeWithScript("sequence.txt");
	}*/
}


bool Level0::checkLevelUp() const {
	return _score->getScore() >= _startScore + 200;
}


Square* Level0::generateSquare() {
	// this should only run if the script file has been exhausted
	// generate a randomly-coloured basic tile
	Square* result =
		new Square(Pos(0, 0), static_cast<Square::Colour>(rand() % 4));

	return result;
}

#include "LevelTest.h"
#include <stdlib.h>

Square* LevelTest::generateSquare() {
	return new Square(Pos(0, 0), static_cast<Square::Colour>(std::rand() % 4));
}

bool LevelTest::checkLevelUp() const {
	return false;
}

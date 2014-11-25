#include "LevelTest.h"
#include <cstdlib>

Square* LevelTest::generateSquare() {
	return new Square(Pos(0, 0), static_cast<Square::Colour>(rand() % 4));
}

bool LevelTest::checkLevelUp() const {
	return false;
}

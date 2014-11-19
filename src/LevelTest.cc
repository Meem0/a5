#include "LevelTest.h"
#include <stdlib.h>

LevelTest::LevelTest(Score* score) : Level(score) { }

Square* LevelTest::nextSquare() {
	return new Square(Pos(0, 0), (Square::Colour)(std::rand() % 4));
}

bool LevelTest::checkLevelUp() const {
	return false;
}

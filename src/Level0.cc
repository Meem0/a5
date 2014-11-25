#include "Level0.h"
#include "Score.h"


bool Level0::checkLevelUp() const {
	return _score->getScore() >= _startScore + 200;
}


Square* Level0::generateSquare() {
	std::cout << "error: Level0::generateSquare() was called" << std::endl;
	return NULL;
}

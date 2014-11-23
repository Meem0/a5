#ifndef LEVEL_TEST_H
#define LEVEL_TEST_H

#include "Level.h"

class LevelTest : public Level {
public:
	bool checkLevelUp() const;

private:
	Square* generateSquare();
};

#endif

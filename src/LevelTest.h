#ifndef LEVEL_TEST_H
#define LEVEL_TEST_H

#include "Level.h"

class LevelTest : public Level {
public:
	LevelTest(Score*);

	Square* nextSquare();

	bool checkLevelUp() const;
};

#endif
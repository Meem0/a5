#ifndef LEVEL_2_H
#define LEVEL_2_H

#include "Level.h"

class Level2 : public Level {
public:
	bool checkLevelUp() const;

private:
	Square* generateSquare();

	// 1/5 chance of adding a position within the
	//   bounds of _board to _lockedSquares
	void generateLocked();
};

#endif

#ifndef LEVEL_2_H
#define LEVEL_2_H

#include "Level.h"

class Level2 : public Level {
public:
	// returns true if the current score is 500 points higher than
	//   what it was when the level began, and there are no remaining
	//   locked squares
	bool checkLevelUp() const;

private:
	// returns a new basic square, with an equal chance of being each colour
	Square* generateSquare();

	// 1/5 chance of adding a position within the
	//   bounds of _board to _lockedSquares
	void generateLocked();
};

#endif

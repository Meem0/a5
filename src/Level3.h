#ifndef LEVEL_3_H
#define LEVEL_3_H

#include "Level.h"

class Level3 : public Level {
public:
	// returns true if the current score is 750 points higher than
	//   what it was when the level began, and there are no remaining
	//   locked squares
	bool checkLevelUp() const;

	// returns true, sets newSize to (8, 6)
	bool requiresResize(Pos& newSize) const;

	// returns 10
	int moveLimit() const;

private:
	// returns a new basic square, with an equal chance of being each colour
	Square* generateSquare();

	// 1/7 chance of adding a position within the
	//   bounds of _board to _lockedSquares
	void generateLocked();
};

#endif

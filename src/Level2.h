#ifndef LEVEL_2_H
#define LEVEL_2_H

#include "Level.h"

class Level2 {
public:
	Level2(Score*);

	bool checkLevelUp() const;

private:
	Square* generateSquare();

	// for every position between (0, 0) and the given pos,
	//   there's a 1/5 chance of adding that position to _lockedSquares
	void generateLocked(Pos);
};

#endif

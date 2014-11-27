#ifndef LEVEL_1_H
#define LEVEL_1_H

#include "Level.h"

class Level1 : public Level {
public:
	// returns true if the current score is 300 points higher than
	//   what it was when the level began
	bool checkLevelUp() const;

private:
	// returns a new square:
	// 1/6 chance of green/blue, 1/3 chance of white/red
	// 1/20 chance of being any special square
	Square* generateSquare();
};

#endif

#ifndef LEVEL_0_H
#define LEVEL_0_H

#include "Level.h"

class Level0 : public Level {
public:
	// returns true if the current score is 200 points higher than
	//   what it was when the level began
	bool checkLevelUp() const;

private:
	// should not be called, since Level0 is supposed to use a script file
	Square* generateSquare();
};

#endif

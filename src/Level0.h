#ifndef LEVEL_0_H
#define LEVEL_0_H

#include "Level.h"

class Level0 {
public:
	Level0(Score*);

	bool checkLevelUp() const;

private:
	Square* generateSquare();
};

#endif

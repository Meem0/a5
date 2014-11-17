#ifndef LEVEL_1_H
#define LEVEL_1_H

#include "Level.h"

class Level1 {
public:
	Level1(Score*);

	Square* nextSquare();

	bool checkLevelUp() const;
};

#endif

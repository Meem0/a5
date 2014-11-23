#ifndef LEVEL_0_H
#define LEVEL_0_H

#include "Level.h"

class Level0 : public Level {
public:
	Level0();

	bool checkLevelUp() const;

private:
	Square* generateSquare();
};

#endif

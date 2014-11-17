#ifndef LEVEL_0_H
#define LEVEL_0_H

#include "Level.h"
#include <fstream>

class Level0 {
public:
	Level0(Score*);

	Square* nextSquare();

	bool checkLevelUp() const;

private:
	std::ifstream _sequence;
};

#endif

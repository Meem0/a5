#ifndef LEVEL_2_H
#define LEVEL_2_H

#include "Level.h"

class Level2 {
public:
	Level2(Score*);

	Square* nextSquare();

	bool checkLevelUp() const;

	std::deque<Pos> getLockedSquares(Pos);
};

#endif

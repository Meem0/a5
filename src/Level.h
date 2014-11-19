#ifndef LEVEL_H
#define LEVEL_H

#include "Square.h"
#include "Score.h"
#include <deque>

class Level {
public:
	Level(Score*);

	// constructs a new square based on the specifications of the level
	virtual Square* nextSquare() = 0;

	// returns true if the conditions to advance to the next level
	//   have been satisfied
	virtual bool checkLevelUp() const = 0;

	// returns a list of positions between (0, 0) and Pos to lock
	virtual std::deque<Pos> getLockedSquares(Pos);

	virtual ~Level();

protected:
	Score* _score;
	int _startScore;
};

#endif

#include "Level.h"

Level::Level(Score* score) : _score(score) {
	//_startScore = _score == NULL ? 0 : _score->getScore();
	_startScore = 0;
}

std::deque<Pos> Level::getLockedSquares(Pos size) {
	std::deque<Pos> result;
	return result;
}

Level::~Level() { }

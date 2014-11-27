#ifndef BOARD_DISPLAY_H
#define BOARD_DISPLAY_H

class Board;
class Score;

class BoardDisplay {
public:
	BoardDisplay(Board*, Score *);

	// draws the board according to the implementation of the inheriting class
	virtual void draw() = 0;

	virtual ~BoardDisplay();

protected:
	Board* _board;
	Score* _score;
};

#endif

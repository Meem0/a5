#ifndef DISPLAY_H
#define DISPLAY_H

class Board;
class Score;

class Display {
public:
	Display(Board*, Score *);

	virtual void draw() = 0;

	virtual ~Display();

protected:
	Board* _board;
	Score* _score;
};

#endif

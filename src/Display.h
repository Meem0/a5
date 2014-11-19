#ifndef DISPLAY_H
#define DISPLAY_H

class Board;

class Display {
public:
	Display(Board*);

	virtual void draw() = 0;

	virtual ~Display();

protected:
	Board* _board;
};

#endif

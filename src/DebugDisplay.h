#ifndef DEBUG_DISPLAY_H
#define DEBUG_DISPLAY_H

#include "TextDisplay.h"

class DebugDisplay {
public:
	static void printBoard();

	static void setBoard(Board* board);
	static void setScore(Score* score);
	static void tempDtor();
private:
	static TextDisplay* _display;
	static Board* _board;
	static Score* _score;
};

#endif
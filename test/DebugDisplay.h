#ifndef DEBUG_DISPLAY_H
#define DEBUG_DISPLAY_H

#include "TextDisplay.h"
#include "GraphicalDisplay.h"

class DebugDisplay {
public:
	static void printBoard();

	static void setBoard(Board* board);
	static void tempDtor();

	static GraphicalDisplay* _gDisplay;
private:
	static TextDisplay* _display;
	static Board* _board;
};

#endif
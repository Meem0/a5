#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include "BoardDisplay.h"

class TextDisplay: public BoardDisplay {
public:
	TextDisplay(Board*);

	// prints the board to stdin
	void draw();

};

#endif

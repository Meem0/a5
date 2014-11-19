#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include "Display.h"

class TextDisplay: public Display {
public:
	TextDisplay(Board*);

	// writes information like level, score, etc.,
	//   then prints the whole board
	void draw();
};

#endif

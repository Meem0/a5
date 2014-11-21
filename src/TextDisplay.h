#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include "Display.h"
#include "Score.h"

class TextDisplay: public Display {
public:
	TextDisplay(Board*, Score *);

	// writes information like level, score, etc.,
	//   then prints the whole board
	void draw();
};

#endif

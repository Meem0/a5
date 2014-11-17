#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include "Display.h"

class TextDisplay: public Display {
public:
	TextDisplay(Board*);

private:
	// writes information like level, score, etc.
	void drawInfo();

	void draw(Pos);
};

#endif

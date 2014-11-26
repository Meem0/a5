#include "window_t.h"

Xwindow::Xwindow(int width, int height) : _width(width), _height(height) { }

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
	std::cout << "rect: pos: (" << x << ", " << y << "), size: ("
			  << width << ", " << height << "), colour: " << colour << std::endl;
}

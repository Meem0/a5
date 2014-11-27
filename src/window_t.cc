#include "window_t.h"

Xwindow::Xwindow(int width, int height) : _width(width), _height(height) { }

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
	std::cout << x << "x " << y << "y " << width << "w " << height << "h " << colour << "c" << std::endl;
}

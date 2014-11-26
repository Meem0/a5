#ifndef WINDOW_T_H
#define WINDOW_T_H
#include <iostream>
#include <string>

class Xwindow {
  int _width, _height;

 public:
  Xwindow(int width=500, int height=500);  // Constructor; displays the window

  enum {White=0, Black, Red, Green, Blue, Cyan, Yellow, Magenta, Orange, Brown}; // Available colours.

  // Draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour=Black);
};

#endif

#include <iostream>

#include "Board.h"

int main() {
	int rows = 5;
	int cols = 4;

	Board board(rows, cols);

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			board.addSquare(new Square(Pos(row, col), (Square::Colour)((row + col) % 4)));
		}
	}

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			board.getSquare(Pos(row, col))->textDraw();
			std::cout << " ";
		}
		std::cout << std::endl;
	}

	int end;
	std::cin >> end;
}

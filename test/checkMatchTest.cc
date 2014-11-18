#include <iostream>

#include "BoardManip.h"

void printBoard(const Board &board) {
	for (int row = 0; row < board.getSize().row; row++) {
		for (int col = 0; col < board.getSize().col; col++) {
			board.getSquare(Pos(row, col))->textDraw();
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}

int main() {
	int rows = 5;
	int cols = 4;

	Board board(rows, cols);
	BoardManip boardManip(&board, NULL);

	for (int row = 0; row < rows - 1; row++) {
		for (int col = 0; col < cols; col++) {
			board.addSquare(new Square(Pos(row, col), (Square::Colour)((row + col) % 4)));
		}
	}
	
	board.removeSquare(Pos(3, 3));

	board.addSquare(new Square(Pos(3, 3), Square::RED));
	board.addSquare(new Square(Pos(4, 0), Square::BLUE));
	board.addSquare(new Square(Pos(4, 1), Square::BLUE));
	board.addSquare(new Square(Pos(4, 2), Square::BLUE));
	board.addSquare(new Square(Pos(4, 3), Square::RED));

	printBoard(board);

	// this test requires BoardManip::checkMove to be public
	/*int posSize = 9;
	Pos positions[] = {
		Pos(0, 0),
		Pos(1, 1),
		Pos(4, 0),
		Pos(4, 1),
		Pos(4, 2),
		Pos(2, 3),
		Pos(3, 2),
		Pos(3, 3),
		Pos(4, 3)
	};

	Pos start(0, 0);
	Pos end(0, 0);
	Pos third(0, 0);
	
	for (int i = 0; i < posSize; i++) {
		int result = boardManip.checkMatch(positions[i], start, end, third);

		std::cout << "== checkMatch(" << positions[i].row << ", "
				  << positions[i].col << ") ==" << std::endl
				  << "result: " << result << std::endl;
		if (result != 0) {
			std::cout << "start: (" << start.row << ", " << start.col << ")" << std::endl
					  << "end: (" << end.row << ", " << end.col << ")" << std::endl;
		}
		std::cout << std::endl << std::endl;
	}*/

	int pause;
	std::cin >> pause;
}

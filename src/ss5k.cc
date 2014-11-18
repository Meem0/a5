#include <iostream>
#include <cstdlib>

#include "BoardManip.h"

void mainPrintBoard(const Board &board) {
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

	std::srand(1234);

	Board board(rows, cols);
	BoardManip boardManip(&board, NULL);

	mainPrintBoard(board);

	boardManip.resetBoard();

	mainPrintBoard(board);

	char choice = 'a';

	while (choice != 'x') {
		std::cin >> choice;

		switch (choice) {
		case 'r': boardManip.resetBoard(); break;
		case 's':
			int row, col, dir;
			std::cin >> row >> col >> dir;

			boardManip.swap(Pos(row, col), (BoardManip::Direction)dir);
			
			mainPrintBoard(board);

			break;
		}
	}
}

#include <iostream>
#include <cstdlib>

#include "BoardManip.h"
#include "LevelTest.h"

int main() {
	int rows = 5;
	int cols = 4;
	int seed;

	std::cout << "seed? ";
	std::cin >> seed;
	std::srand(seed);

	Board board(rows, cols);
	BoardManip boardManip(&board, NULL);
	LevelTest level(NULL);

	boardManip.setLevel(&level);
	DebugDisplay::setBoard(&board);

	DebugDisplay::printBoard();

	boardManip.resetBoard();

	DebugDisplay::printBoard();

	char choice = 'a';

	while (choice != 'x') {
		std::cin >> choice;

		switch (choice) {
		case 'r': boardManip.resetBoard(); break;
		case 's':
			int row, col, dir;
			std::cin >> row >> col >> dir;

			boardManip.swap(Pos(row, col), (BoardManip::Direction)dir);
			
			DebugDisplay::printBoard();

			break;
		}
	}
}

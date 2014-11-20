#include <iostream>
#include <cstdlib>

#include "BoardManip.h"
#include "LevelTest.h"

int main(int argc, char * argv[]) {
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

			break;
		case 'h': // hint
			Pos movePos;
			BoardManip::Direction moveDir;

			if (boardManip.findMove(movePos, moveDir)) {
				std::cout << "try the move: " << movePos.row << " " << movePos.col
						  << " " << moveDir << std::endl;
			}

			break;
		}
	}
}

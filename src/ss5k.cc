#include <iostream>
#include <cstdlib>

#include "BoardManip.h"
#include "LevelTest.h"

using namespace std;
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
		case 'h': {// hint			
			Pos movePos(0,0);
			BoardManip::Direction moveDir;

				if (boardManip.findMove(movePos, moveDir)) {
					std::cout << "try the move: " << movePos.row << " " << movePos.col
							  << " " << moveDir << std::endl;
				} else {
					 cout << "no moves, go scramble you noob" << endl;
				}
			}
			break;
		case 'b': {//scramble
				Pos fillerPos(0,0);
				BoardManip::Direction fillerDir;
				if (!boardManip.findMove(fillerPos,fillerDir)) {
					boardManip.scramble();
					DebugDisplay::printBoard();
				} else {
					cout << "there is a possible move" << endl;
				}
			}
			break;
		}
	}
}

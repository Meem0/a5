#include <iostream>
#include <cstdlib>
#include <string>

#include "BoardManip.h"
#include "LevelTest.h"

using namespace std;
int main(int argc, char * argv[]) {
	Pos boardSize(6, 5);
	int seed;
	string scriptFileName;
	Level* level = new LevelTest(NULL);

	cout << "script file? (blank line for none) ";
	getline(cin, scriptFileName);

	if (scriptFileName.length() > 0) {
		boardSize = level->initializeWithScript(scriptFileName);
	}

	std::cout << "seed? ";
	std::cin >> seed;
	std::srand(seed);

	Board board(boardSize.row, boardSize.col);
	BoardManip boardManip(&board, NULL);

	boardManip.setLevel(level);
	DebugDisplay::setBoard(&board);

	DebugDisplay::printBoard();

	boardManip.resetBoard();

	DebugDisplay::printBoard();

	char choice = 'a';

	while (choice != 'x') {
		std::cin >> choice;

		switch (choice) {
		case 'r': 
			//restart level, with the same starting grid if a scriptFile was given
			if (scriptFileName.length() > 0) {
				boardSize = level->initializeWithScript(scriptFileName);
			}
			boardManip.resetBoard(); break;
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

#include <iostream>
#include <cstdlib>
#include <string>

#include "BoardManip.h"
#include "LevelTest.h"
#include "Score.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"

using namespace std;

// create a new level corresponding to the given number, and reset any
//   necessary references
Level* setLevel(int levelNum, BoardManip& boardManip,Board &board);

int main(int argc, char * argv[]) {
	Pos boardSize(6, 5);
	int seed;
	int levelNum = 0;
	
	string scriptFileName;
	/*cout << "script file? (blank line for none) ";
	getline(cin, scriptFileName);

	if (scriptFileName.length() > 0) {
		boardSize = Level::initializeWithScript(scriptFileName);
	}*/

	std::cout << "seed? ";
	std::cin >> seed;
	std::srand(seed);

	Score score;
	Board board(boardSize.row, boardSize.col);
	BoardManip boardManip(&board, &score);
	Level* level = setLevel(levelNum, boardManip,board);

	DebugDisplay::setBoard(&board);
	DebugDisplay::setScore(&score);
	DebugDisplay::printBoard();

	boardManip.resetBoard();

	DebugDisplay::printBoard();

	char choice = 'a';

	while (choice != 'x') {
		std::cin >> choice;

		switch (choice) {
		case 'r': 
			//restart level, with the same starting grid if a scriptFile was given
			//if (scriptFileName.length() > 0) {//check if we changed level
				//level->initializeWithScript(scriptFileName);
			//}
			boardManip.resetBoard();
			break;
		case 's':
			int row, col, dir;
			std::cin >> row >> col >> dir;

			boardManip.swap(Pos(row, col), static_cast<BoardManip::Direction>(dir));

			if ((levelNum == 0 || levelNum == 1) && level->checkLevelUp()) {
				levelNum++;
				delete level;
				level = setLevel(levelNum, boardManip,board);
				boardManip.resetBoard();

				std::cout << "Level up!  Now at level " << levelNum << std::endl;
				DebugDisplay::printBoard();

			}

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
				BoardManip::Direction fillerDir;
				bool performedScramble = false;

				while (!boardManip.findMove(Pos(0, 0), fillerDir)) {
					boardManip.scramble();
					performedScramble = true;
					DebugDisplay::printBoard();
				}

				if (!performedScramble) {
					cout << "there is a possible move" << endl;
				}
			}
			break;
		case 'l': {//set level
			std::cin >> levelNum;
			delete level;
			level = setLevel(levelNum, boardManip,board);
			boardManip.resetBoard();
			break;
			}
		}
	}
}


Level* setLevel(int levelNum, BoardManip& boardManip,Board & board) {
	Level* result = NULL;
	Pos boardSize = board.getSize();
	Pos current;

	switch (levelNum) {
	case 0: result = new Level0; break;
	case 1: result = new Level1; break;
	case 2: result = new Level2; break;
	default: std::cout << "error: invalid level number in setLevel" << std::endl; break;
	}

	boardManip.setLevel(result);
	//unlock everything
	for (current.row = 0; current.row < boardSize.row; current.row++) {
		for (current.col = 0; current.col <boardSize.col; current.col++) {
			board.setLock(current,false);
		}
	}

	//set what to lock
	std::deque<Pos> toLock= result->getLockedSquares();
	for (std::deque<Pos>::iterator itr = toLock.begin(); itr != toLock.end(); itr++) {
		board.setLock(*itr,true);
	}
	return result;
}

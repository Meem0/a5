#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

#include "BoardManip.h"
#include "LevelTest.h"
#include "Score.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"

using namespace std;

// create a new level corresponding to the given number, and reset any
//   necessary references
Level* numToLevel(int levelNum);

int main(int argc, char * argv[]) {
	Pos boardSize(10,10);
	string scriptFileName("sequence.txt");
	bool useGraphicalDisplay = true;
	bool scriptFileCommand = false;
	int seed = 1;
	int levelNum = 0;

	for (int i = 1; i < argc; i++) {
		string cmd = argv[i];
		if (cmd == "-text") {
			useGraphicalDisplay = false;
		}
		else if (cmd == "-seed") {
			i++;
			if (argc > i) {
				istringstream ss(argv[i]);
				ss >> seed;
				std::srand(seed);
			}
			else
				std::cout << "invalid argument syntax: missing seed" << std::endl;
		} 
		else if (cmd == "-scriptfile") {
			i++;
			if (argc > i) {
				scriptFileName = argv[i];
				scriptFileCommand = true;
			}
			else
				std::cout << "inalid argument syntax: missing script file name" << std::endl;
		}
		else if (cmd == "-startlevel") {
			i++;
			if (argc > i) {
				istringstream ss(argv[i]);
				ss >> levelNum;
			}
			else
				std::cout << "invalid argument syntax: missing level" << std::endl;
		}
	}
	
	Level* level = numToLevel(levelNum);

	if (levelNum == 0 || scriptFileCommand)
		boardSize = level->initializeWithScript(scriptFileName);

	Score score;
	Board board(boardSize.row, boardSize.col);
	BoardManip boardManip(&board, &score);
	boardManip.setLevel(level);

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
				level = numToLevel(levelNum);
				boardManip.setLevel(level);

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
				Pos fillerPos;
				bool performedScramble = false;

				while (!boardManip.findMove(fillerPos, fillerDir)) {
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
			level = numToLevel(levelNum);
			boardManip.setLevel(level);

			boardManip.resetBoard();

			break;
			}
		}
	}

	delete level;
	DebugDisplay::tempDtor();
}


Level* numToLevel(int levelNum) {
	Level* result = NULL;

	switch (levelNum) {
	case 0: result = new Level0; break;
	case 1: result = new Level1; break;
	case 2: result = new Level2; break;
	default: std::cout << "error: invalid level number in setLevel" << std::endl; break;
	}

	return result;
}

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <sstream>
#include <deque>

#include "BoardManip.h"
#include "Score.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "TextDisplay.h"
#include "GraphicalDisplay.h"

using namespace std;

// create a new level corresponding to the given number, and reset any
//   necessary references
Level* numToLevel(int levelNum);

// print some game information, then draw with the TextDisplay
// if useGraphicalDisplay is true, draw with the GraphicalDisplay
void draw(deque<BoardDisplay*>& displays, Score& score, int levelNum, int moves);

int main(int argc, char * argv[]) {
	Pos boardSize(10,10);
	string scriptFileName("sequence.txt");
	deque<BoardDisplay*> displays;
	bool useGraphicalDisplay = true;
	bool scriptFileCommand = false;
	bool drawBreakdown = false;
	int seed = 1;
	int levelNum = 0;
	int maxLevel = 3;
	int movesRemaining = -1;

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

				if (levelNum < 0 || levelNum > maxLevel) {
					cout << "Invalid level number in command-line argument." << endl;
					levelNum = 0;
				}
			}
			else
				std::cout << "invalid argument syntax: missing level" << std::endl;
		}
	}
	
	Level* level = numToLevel(levelNum);
	
	// initialize the board size and level with the script file
	// includes error handling in case the file is not found
	if (scriptFileCommand || levelNum == 0) {
		bool resolved = false;

		while (!resolved) {
			try {
				boardSize = level->initializeWithScript(scriptFileName);
				resolved = true;
			}
			catch (int) {
				cout << "File \"" << scriptFileName << "\" was not found." << endl
					 << "Enter a new file name to try: ";
				getline(cin, scriptFileName);
			}
		}
	}

	// may or may not change boardSize, depending on the level
	level->requiresResize(boardSize);

	Score score;
	Board board(boardSize.row, boardSize.col);

	BoardManip boardManip(&board, &score);
	boardManip.setLevel(level);

	displays.push_back(new TextDisplay(&board));

	if (useGraphicalDisplay) {
		displays.push_back(new GraphicalDisplay(&board));
	}

	boardManip.resetBoard();
	
	// get the number of moves available to complete the level
	movesRemaining = level->moveLimit();

	draw(displays, score, levelNum, movesRemaining);

	while (cin) {
		bool changedLevel = false;
		string line, choice;

		// get the next command and arguments
		getline(cin, line);
		istringstream lineReader(line);

		// read the command
		lineReader >> choice;

		if (choice == "restart") {
			//creates a new board of the current level
			boardManip.resetBoard();

			// get the number of moves available to complete the level
			movesRemaining = level->moveLimit();

			draw(displays, score, levelNum, movesRemaining);
		}
		else if (choice == "swap") {
			// read in the swap details
			int row, col, dir;
			lineReader >> row >> col >> dir;

			if (!lineReader) {
				cout << "Invalid syntax.  Should be: swap x y dir" << endl;
			}
			else if (dir < 0 || dir > 3) {
				cout << "Invalid direction. 0: north, 1: south, 2: west, 3: east" << endl;
			}
			// try to perform the swap
			else if (boardManip.swap(Pos(row, col), static_cast<BoardManip::Direction>(dir))) {
				// consume a move, if this level uses the mechanic
				if (movesRemaining != -1)
					movesRemaining--;

				cout << endl;
				draw(displays, score, levelNum, movesRemaining);

				// check for level up
				if (level->checkLevelUp()) {

					changedLevel = true;

					if (levelNum == maxLevel) {
						cout << "Final level complete!  Restarting..." << endl;
					}
					else {
						levelNum++;
						cout << "Level up! ";
					}
				}
				else if (movesRemaining == 0) {
					cout << "Level failed.  Restarting..." << endl;
					changedLevel = true;
				}
			}
		}
		else if (choice == "hint") {
			Pos movePos(0,0);
			BoardManip::Direction moveDir;
			
			// check if there's a move
			if (boardManip.findMove(movePos, moveDir)) {
				std::cout << "Try the move: " << movePos.row << " "
					      << movePos.col << " " << moveDir << std::endl;
			}
			else
				std::cout << "No available moves.  Try the scramble command." << std::endl;
		}
		else if (choice == "scramble") {
			// call scramble, which returns true if it actually scrambled the board
			bool performedScramble = boardManip.scramble();

			// redraw the board if a scramble was performed
			if (performedScramble) {
				draw(displays, score, levelNum, movesRemaining);
			}
			else
				cout << "Cannot scramble when there is an availabe move." << endl;
		}
		else if (choice == "levelup") {
			if (levelNum < maxLevel) {
				levelNum++;
				changedLevel = true;
			}
			else
				cout << "Cannot level up past level " << maxLevel << "." << endl;
		}
		else if (choice == "leveldown") {
			if (levelNum > 0) {
				levelNum--;
				changedLevel = true;
			}
			else
				cout << "Cannot level down past level 0." << endl;
		}
		else if (choice == "drawsteps") {
			drawBreakdown = !drawBreakdown;
			boardManip.drawBreakdown(drawBreakdown, displays);

			if (drawBreakdown) {
				cout << "Now drawing intermediate steps." << endl;
				cout << "Press Enter to show the next move or type skip to skip to the end of the move" << endl;
			}else
				cout << "No longer drawing intermediate steps." << endl;
		}
		else if (cin) {
			cout << "Invalid command." << endl;
		}

		cout << endl;

		// swap, levelup, and leveldown can make this happen
		if (changedLevel) {
			cout << "Now at level " << levelNum << endl;

			delete level;
			level = numToLevel(levelNum);
			boardManip.setLevel(level);

			// get the number of moves available to complete the level
			movesRemaining = level->moveLimit();

			Pos newBoardSize;
			if (level->requiresResize(newBoardSize)) {
				board.resize(newBoardSize);
			}

			boardManip.resetBoard();

			cout << endl;
			draw(displays, score, levelNum, movesRemaining);
		}
	}

	for (deque<BoardDisplay*>::iterator itr = displays.begin();
		 itr != displays.end(); itr++) {
		delete (*itr);
	}
	delete level;
}


Level* numToLevel(int levelNum) {
	Level* result = NULL;

	switch (levelNum) {
	case 0: result = new Level0; break;
	case 1: result = new Level1; break;
	case 2: result = new Level2; break;
	case 3: result = new Level3; break;
	default: std::cout << "error: invalid level number in setLevel" << std::endl; break;
	}

	return result;
}

void draw(deque<BoardDisplay*>& displays, Score& score, int levelNum, int moves) {
	cout << "Level:" << setw(13) << levelNum << endl
		 << "Score:" << setw(13) << score.getScore() << endl
		 << "High score:" << setw(8) << score.getHighScore() << endl;

	if (moves != -1) {
		cout << "Moves remaining:" << setw(3) << moves << endl;
	}

	cout << "___________________" << endl << endl;

	for (deque<BoardDisplay*>::iterator itr = displays.begin();
		 itr != displays.end(); itr++) {
		(*itr)->draw();
	}
}

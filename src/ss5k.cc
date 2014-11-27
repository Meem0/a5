#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <sstream>

#include "BoardManip.h"
#include "Score.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "TextDisplay.h"
#include "GraphicalDisplay.h"

using namespace std;

// create a new level corresponding to the given number, and reset any
//   necessary references
Level* numToLevel(int levelNum);

// print some game information, then draw with the TextDisplay
// if useGraphicalDisplay is true, draw with the GraphicalDisplay
void draw(TextDisplay& textDisplay, GraphicalDisplay* graphicalDisplay,
		  Score& score, int levelNum, bool useGraphicalDisplay);

int main(int argc, char * argv[]) {
	Pos boardSize(10,10);
	string scriptFileName("sequence.txt");
	bool useGraphicalDisplay = true;
	bool scriptFileCommand = false;
	int seed = 1;
	int levelNum = 0;
	int maxLevel = 2;

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
	
	// initialize the board with the script file
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

	Score score;
	Board board(boardSize.row, boardSize.col);

	BoardManip boardManip(&board, &score);
	boardManip.setLevel(level);

	TextDisplay textDisplay(&board);

	// it's a pointer so that its constructor can be optional
	GraphicalDisplay* graphicalDisplay = NULL;
	if (useGraphicalDisplay) {
		graphicalDisplay = new GraphicalDisplay(&board);
#ifdef _DEBUG
		DebugDisplay::_gDisplay = graphicalDisplay;
#endif
	}

#ifdef _DEBUG
	DebugDisplay::setBoard(&board);
#endif

	boardManip.resetBoard();

	draw(textDisplay, graphicalDisplay, score, levelNum, useGraphicalDisplay);

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
			draw(textDisplay, graphicalDisplay, score, levelNum, useGraphicalDisplay);
		}
		else if (choice == "swap") {
			// read in the swap details
			int row, col, dir;
			lineReader >> row >> col >> dir;

			if (!lineReader) {
				cout << "Invalid syntax.  Should be: swap x y dir" << endl;
			}
			// try to perform the swap
			else if (boardManip.swap(Pos(row, col), static_cast<BoardManip::Direction>(dir))) {
				cout << endl;
				draw(textDisplay, graphicalDisplay, score, levelNum, useGraphicalDisplay);

				// check for level up
				if (levelNum < maxLevel && level->checkLevelUp()) {
					levelNum++;
					changedLevel = true;
					cout << "Level up! ";
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
			if (performedScramble)
				draw(textDisplay, graphicalDisplay, score, levelNum, useGraphicalDisplay);
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
		else {
			cout << "Invalid command." << endl;
		}

		// swap, levelup, and leveldown can make this happen
		if (changedLevel) {
			cout << "Now at level " << levelNum << endl;

			delete level;
			level = numToLevel(levelNum);
			boardManip.setLevel(level);

			boardManip.resetBoard();

			cout << endl;
			draw(textDisplay, graphicalDisplay, score, levelNum, useGraphicalDisplay);
		}
	}

	delete graphicalDisplay;
	delete level;

#ifdef _DEBUG
	DebugDisplay::tempDtor();
#endif
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

void draw(TextDisplay& textDisplay, GraphicalDisplay* graphicalDisplay,
		  Score& score, int levelNum, bool useGraphicalDisplay) {
	cout << "Level:" << setw(7) << levelNum << endl
		 << "Score:" << setw(7) << score.getScore() << endl
		 << "__________" << endl << endl;
	textDisplay.draw();

	if (useGraphicalDisplay)
		graphicalDisplay->draw();
}

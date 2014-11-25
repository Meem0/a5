#include "Level.h"
#include "Score.h"
#include "Board.h"
#include "LateralSquare.h"
#include "UprightSquare.h"
#include "UnstableSquare.h"
#include "PsychSquare.h"
#include <fstream>
#include <sstream>


void Level::setBoard(Board* board) {
	_board = board;
}


void Level::setScore(Score* score) {
	_score = score;
	_startScore = _score->getScore();
}


Pos Level::initializeWithScript(const std::string& fileName) {
	Pos result;
	std::ifstream in(fileName.c_str());
	std::string line;
	_usingScriptFile = true;

	// loop through the lines in the script file
	while (std::getline(in, line)) {
		_sequenceColoursIndex = 0;
		std::istringstream lineReader(line);

		// if line doesn't start with _ or l, it must be the last line with the colours
		if (line.length() > 0 && line[0] != '_' && line[0] != 'l') {
			char currentChar;

			while (lineReader >> currentChar) {
				if (currentChar < '0' || '3' < currentChar) {
					std::cout << "read an invalid character in the final line: "
							  << currentChar << std::endl;
				}
				else {
					_sequenceColours.push_back(static_cast<Square::Colour>(currentChar - '0'));
				}
			}
		}
		// otherwise, it's (supposed to be) a 3-character token representing a cell
		else {
			int colsThisLine = 0;
			std::string square;

			// loop through the tokens this line
			// expected to be of the form:
			// [_l][_hvbp][0123]
			while (lineReader >> square) {
				// error checking that might be removed in the final version
				if (square.length() != 3) {
					std::cout << "read a token whose length isn't 3 in the script file" << std::endl;
				}
				else if (square[1] != '_' && square[1] != 'h' && square[1] != 'v'
						 && square[1] != 'b' && square[1] != 'p') {
					std::cout << "read a token with an invalid special character" << std::endl;
				}
				else if (square[2] < '0' || square[2] > '3') {
					std::cout << "read a token with an invalid colour character" << std::endl;
				}
				// if everything's fine, build a cell
				else {
					ScriptCell cell;
					cell.isLocked = square[0] == '_' ? false : true;
					cell.special = square[1];
					cell.colour = static_cast<Square::Colour>(square[2] - '0');
					cell.pos = Pos(result.row, colsThisLine);

					_scriptCells.push_back(cell);

					colsThisLine++;
				}
			}

			// if at least one square was parsed, this is a valid row
			if (colsThisLine > 0) {
				result.row++;

				// theoretically, all rows should have the same number of columns
				//   so this should only happen once
				if (result.col == 0)
					result.col = colsThisLine;
				// sanity check for column number mismatches
				else if (result.col != colsThisLine) {
					std::cout << "inconsistent number of columns in script file row "
							  << result.row << std::endl;
				}
			}
		}
	}

	return result;
}


Square* Level::nextSquare(Pos pos) {
	Square* result;

	// if there are initialization cells from the script file
	if (!_scriptCells.empty()) {
		// create the square based on the information from the script file
		switch (_scriptCells.front().special) {
		case 'h': result = new LateralSquare  (_scriptCells.front().pos, _scriptCells.front().colour); break;
		case 'v': result = new UprightSquare  (_scriptCells.front().pos, _scriptCells.front().colour); break;
		case 'b': result = new UnstableSquare (_scriptCells.front().pos, _scriptCells.front().colour); break;
		case 'p': result = new PsychSquare    (_scriptCells.front().pos, _scriptCells.front().colour); break;
		default : result = new Square         (_scriptCells.front().pos, _scriptCells.front().colour); break;
		}

		// the script file determines locked cells as well,
		//   so add a locked cell to the list now if appropriate
		if (_scriptCells.front().isLocked) {
			_lockedSquares.push_back(result->getPos());
		}

		_scriptCells.pop_front();
	}
	// otherwise, check if there's a sequence of colours from the script file
	else if (!_sequenceColours.empty()) {
		result = new Square(pos, _sequenceColours[_sequenceColoursIndex]);
		
		_sequenceColoursIndex++;
		if (_sequenceColoursIndex >= _sequenceColours.size())
			_sequenceColoursIndex = 0;
	}
	// if a script file is not being used, generate a square based on the current level's implementation
	else {
		result = generateSquare();
		result->setPos(pos);
	}

	return result;
}


std::deque<Pos> Level::getLockedSquares() {
	// if a script file is not being used for initialization, generate locked squares
	//   based on the current level's implementation
	if (!_usingScriptFile)
		generateLocked();

	return _lockedSquares;
}


void Level::generateLocked() {
	// do nothing unless this is overridden
}


Level::~Level() { }

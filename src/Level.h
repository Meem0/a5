#ifndef LEVEL_H
#define LEVEL_H

#include "Square.h"
#include <deque>
#include <string>

class Score;
class Board;

class Level {
public:
	// reference setters
	// assigned by BoardManip when assigning a level to it
	void setScore(Score*);
	void setBoard(Board*);

	// parse a script file with a given filename
	// return the size of the board based on the file
	static Pos initializeWithScript(const std::string&);

	// get the next square from the list made from the script file
	// if there isn't one, generate a new square
	// if the resulting square is from the script file and its position
	//   was specified, use the script file position
	// otherwise, assign the given position to the square
	Square* nextSquare(Pos);

	// returns a list of positions on the board to lock
	std::deque<Pos> getLockedSquares();

	// returns true if the conditions to advance to the next level
	//   have been satisfied
	virtual bool checkLevelUp() const = 0;

	virtual ~Level();

protected:
	Score* _score;
	Board* _board;
	int _startScore;
	std::deque<Pos> _lockedSquares;
	
	static bool _usingScriptFile;

private:
	// constructs a new square based on the specifications of the level
	virtual Square* generateSquare() = 0;

	// fills _lockedSquares with squares to lock
	// note that this will do nothing unless an implementing class
	//   overrides this (ie level 2)
	virtual void generateLocked();

	struct ScriptCell {
		bool isLocked;
		char special;
		Square::Colour colour;
		Pos pos;
	};

	static std::deque<ScriptCell> _scriptCells;
};

#endif

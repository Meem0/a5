#ifndef LEVEL_H
#define LEVEL_H

#include "Square.h"
#include <deque>
#include <vector>
#include <string>

class Score;
class Board;

class Level {
public:
	Level();

	// reference setters
	// assigned by BoardManip when assigning a level to it
	void setScore(Score*);
	void setBoard(Board*);

	// parse a script file with a given filename
	// return the size of the board based on the file
	// throws a -1 if the file could not be opened
	Pos initializeWithScript(const std::string&);

	// get the next square from the list made from the script file
	// if there isn't one, generate a new square
	// if the resulting square is from the script file and its position
	//   was specified, use the script file position
	// otherwise, assign the given position to the square
	Square* nextSquare(Pos);

	// returns a list of positions on the board to lock
	std::deque<Pos> getLockedSquares();

	// returns true if this level specifies a new size for the board,
	//   and fills newSize with the specified size
	// otherwise (by default), returns false and doesn't touch newSize
	virtual bool requiresResize(Pos& newSize) const;

	// returns the number of moves in which this level must be completed
	// returns -1 if this level does not have a move limit
	virtual int moveLimit() const;

	// returns true if the conditions to advance to the next level
	//   have been satisfied
	virtual bool checkLevelUp() const = 0;

	virtual ~Level();

protected:
	Score* _score;
	Board* _board;
	int _startScore;
	std::deque<Pos> _lockedSquares;
	
	bool _usingScriptFile;
	std::vector<Square::Colour> _sequenceColours;
	unsigned int _sequenceColoursIndex;

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

	std::deque<ScriptCell> _scriptCells;
};

#endif

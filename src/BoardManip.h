#ifndef BOARD_MANIP_H
#define BOARD_MANIP_H

class Level;

#include "Board.h"
#include "Score.h"
#include <deque>

class BoardManip {
public:
	BoardManip(Board*, Score*);

	enum Direction { NORTH = 0, SOUTH, WEST, EAST };

	// if the given move would result in a match, moves the square
	//   at the given position in the given direction
	// adds the moved squares to updated, and updates the board
	void swap(Pos, Direction);

	// clears the board, then fills it with a new set of tiles and locks
	//   cells if the level requires it
	void resetBoard();

	// randomly shuffles the tiles on the board
	void scramble();

	// if the board currently contains a possible move, return true
	//   and fill start and dir the with corresponding information
	// otherwise return false and leave start and dir unchanged
	bool findMove(Pos& start, Direction& dir);

	void setLevel(Level*);

private:
	Board* _board;
	Level* _level;
	Score* _score;
	std::deque<Pos> _updated;

	bool _noScoringMode;

	// checks for a match for each square in updated
	// destroys any matched squares, scores points accordingly
	// plugs any holes, adding any moved squares to updated
	// repeats until no matches are made (updated is empty)
	void update();

	// shifts squares into any empty squares below them
	// generates new squares if needed
	void plug();

	// if the board currently contains any matches, return true
	// otherwise return false
	bool findMatch();

	// if there is no match involving square, returns 0 and leaves
	//   the other parameters unchanged
	// if a line match involving square was found, returns 1 and fills
	//   start and end with the endpoints of the match
	// if a L-match involving square was found, returns 2 and fills
	//   start, end, and third with the endpoints of the match
	int checkMatch(Pos square, Pos& start, Pos& end, Pos& third);
};

#endif

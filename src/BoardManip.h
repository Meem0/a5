#ifndef BOARD_MANIP_H
#define BOARD_MANIP_H

class Level;

#include "Board.h"
#include "Score.h"
#include <deque>
#include <vector>

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

	// returns a list of positions of all the squares involved in a match
	//   involving the square at the given position
	// (list is empty if there is no match)
	// if a special square is to be generated from the match,
	//   the first element of the list is the position where the special
	//   square should be generated
	std::vector<Pos> checkMatch(Pos square);
};

#endif

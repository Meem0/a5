#ifndef BOARD_H
#define BOARD_H

// TODO: remove this when we no longer need it
#include "DebugDisplay.h"

#include "Square.h"

class Board {
public:
	// creates a board of size rows x cols,
	//   and fills it with empty squares
	Board(int rows, int cols);

	~Board();

	Square* getSquare(Pos) const;
	Pos getSize() const;

	// returns true if the cell at the given position is locked
	bool isLocked(Pos) const;

	// returns true if the given position is within the bounds of the board
	// ie pos >= (0, 0), pos < (_rows, _cols)
	bool withinBounds(Pos) const;

	// adds the given square to the board, using the position specified by the square
	void addSquare(Square*);

	// swaps the two squares at the given positions
	void swap(Pos, Pos);

	// sets the locked state of the Cell at Pos to the given locked state
	void setLock(Pos, bool);

	// replaces the square at the given position with an empty square
	void removeSquare(Pos);

private:
	struct Cell {
		Square* square;
		bool isLocked;
	};

	int _rows;
	int _cols;

	Cell** _board;
};

#endif

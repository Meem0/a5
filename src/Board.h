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
	bool isLocked(Pos) const;
	Pos getSize() const;
	bool withinBounds(Pos) const;

	void addSquare(Square*);
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

#ifndef BOARD_H
#define BOARD_H

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

	void addSquare(Square*);

	// sets the cell at Pos to be locked
	void lock(Pos);

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

#include "BoardManip.h"
#include "Level.h"
#include "Score.h"
#include "BoardDisplay.h"
#include "LateralSquare.h"
#include "UprightSquare.h"
#include "PsychSquare.h"
#include "UnstableSquare.h"

#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

// returns a "unit vector" based on a Direction
// ex: NORTH = (-1, 0), EAST = (0, 1)
Pos dirToPos(BoardManip::Direction);

// returns the direction clockwise from the given direction
// NORTH -> EAST, EAST -> SOUTH, SOUTH -> WEST, WEST -> NORTH
BoardManip::Direction rotDir(BoardManip::Direction dir);

// returns the opposite direction of the given direction
// NORTH -> SOUTH, SOUTH -> NORTH, EAST -> WEST, WEST -> EAST
BoardManip::Direction opDir(BoardManip::Direction dir);

// returns true if moving the square at start in the direction dir would result in a match
bool doesMoveMakeMatch(const Pos& start, BoardManip::Direction dir, Board* board);

// swap the i1th and i2th elements of list
void posListSwap(std::vector<Pos>& list, int i1, int i2);

// returns the number of consecutive squares in the given direction from the given position
//   whose colour matches the given colour, which defaults to the colour of the square
//   at the given position if no colour is specified
int countMatches(const Pos& centre, BoardManip::Direction dir, Board* board, Square::Colour colour = Square::EMPTY);

// add all the positions from start to end to list
// start and end must be on a line (have a coordinate in common)
// option to omit a given position in the line
void addPosLineToList(const Pos& start, const Pos& end, std::vector<Pos>& list, const Pos& omit = Pos(-1, -1));

// adds the given position to updated, and sets the updated field of the corresponding Square
void addSquareToUpdated(Pos pos, deque<Pos>& updated, Board* board);

BoardManip::BoardManip(Board* board, Score* score)
	: _board(board), _score(score), _InitMode(true), _drawBreakdown(false) { }

bool BoardManip::swap(Pos pos, Direction dir){
	Pos moveDir = dirToPos(dir);
	Pos pos2(pos.row + moveDir.row, pos.col + moveDir.col);

	if (!_board->withinBounds(pos)) {
		cout << "The starting position of the swap: " << pos
			 << " is outside the bounds of the board." << endl;
	}
	else if (!_board->withinBounds(pos2)) {
		cout << "The end position of the swap: " << pos2
			 << " is outside the bounds of the board." << endl;
	}
	else if (_board->isLocked(pos) || _board->isLocked(pos2)) {
		cout << "Cannot swap locked squares." << endl;
	}
	else if (!doesMoveMakeMatch(pos, dir, _board) &&
			!doesMoveMakeMatch(pos + dirToPos(dir), opDir(dir), _board)) {
		cout << "Cannot make a move that does not result in a match." << endl;
	}
	else {
		_board->swap(pos, pos2);

		addSquareToUpdated(pos, _updated, _board);
		addSquareToUpdated(pos2, _updated, _board);

		update();

		return true;
	}

	return false;
}


void BoardManip::resetBoard(){
	Pos boardSize = _board->getSize();
	Pos current;
	for (current.row = 0; current.row < boardSize.row; current.row++) {
		for (current.col = 0; current.col < boardSize.col; current.col++) {
			_board->removeSquare(current);

			// unfortunately we can't just call plug() because plug iterates over columns (which shouldn't be changed),
			//   and the scriptfile is read along rows; once it gets to the second column, it starts skipping
			//   squares since they have already been added to the top rows
			// if we really want to use plug, it could create a list of Square pointers, and add them all at the end,
			//   after its done iterating
			Square * next = _level->nextSquare(current);

			_board->addSquare(next);

			// mark the newly generated square as updated
		addSquareToUpdated(current, _updated, _board);
		}
	}

	_InitMode = true;
	update();
	_InitMode = false;

	//unlock everything
	for (current.row = 0; current.row < boardSize.row; current.row++) {
		for (current.col = 0; current.col <boardSize.col; current.col++) {
			_board->setLock(current,false);
		}
	}

	//lock required cells
	std::deque<Pos> toLock= _level->getLockedSquares();
	for (std::deque<Pos>::iterator itr = toLock.begin(); itr != toLock.end(); itr++) {
		_board->setLock(*itr,true);
	}
}


bool BoardManip::scramble(){
	Pos dummyPos;
	Direction dir = {NORTH};
	Pos boardSize = _board->getSize();

	//if there is a match, do not scramble
	if (findMove(dummyPos,dir)) {return false;}

	//while there is no move or there is a match in the grid, scramble
	while (!findMove(dummyPos,dir) || findMatch()) {
		for (int row = 0; row < boardSize.row ; row++) {
			for (int col = 0; col < boardSize.col; col++){
				Pos randomPos(rand() % boardSize.row,rand() % boardSize.col);
				_board->swap(Pos(row,col),randomPos);
			}
		}
	}
	return true;
}


bool doesMoveMakeMatch(const Pos& start, BoardManip::Direction dir, Board* board) {
	Pos movedPos = start + dirToPos(dir); // position of the square after the move

	if (!board->withinBounds(start) || !board->withinBounds(movedPos)) {
		return false;
	}

	Square::Colour matchColour = board->getSquare(start)->getColour(); // colour of the square

	BoardManip::Direction perp1 = rotDir(dir); // clockwise direction of the move direction
	BoardManip::Direction perp2 = opDir(perp1);  // counter-clockwise direction of the move direction

	// if there's at least two more of the colour in the direction of the move
	if (countMatches(movedPos, dir, board, matchColour) >= 2 ||
		// or there's at least two more of the colour in the direction perpendicular
		//   to the direction of the move
		countMatches(movedPos, perp1, board, matchColour) +
		countMatches(movedPos, perp2, board, matchColour) >= 2)
		return true;
	else
		return false;
}


bool BoardManip::findMove(Pos& start, Direction& dir) {
	Pos boardSize = _board->getSize();

	// loop through the board
	for (Pos currentPos; currentPos.row < boardSize.row; currentPos.row++) {
		for (currentPos.col = 0; currentPos.col < boardSize.col; currentPos.col++) {
			// for all four directions, check if moving the current square in that
			//   direction would result in a match
			for (int i = 0; i < 4; i++) {
				Direction currentDir = static_cast<Direction>(i);
				if (doesMoveMakeMatch(currentPos, currentDir, _board)) {
					start = currentPos;
					dir = currentDir;
					return true;
				}
			}
		}
	}

	return false;
}


void BoardManip::setLevel(Level* level) {
	_level = level;

	_level->setBoard(_board);
	_level->setScore(_score);
}


void BoardManip::drawBreakdown(bool shouldDraw, const std::deque<BoardDisplay*>& displays) {
	_drawBreakdown = shouldDraw;

	if (_drawBreakdown) {
		_displays = displays;
	}
}


void BoardManip::update() {
	int chain = 0;
	// whether to skip the rest of the draws of intermediate steps
	// no effect if _drawBreakdown is not true
	bool skipBreakdown = false;

	while (!_updated.empty()) {
		// iterate through all the updated squares
		for (std::deque<Pos>::iterator itr = _updated.begin(); itr != _updated.end(); itr++) {
			// get a list of squares involved in a match with the current square
			// note that checkMatch puts the position where a special square should be
			//   generated as the first in the list
			std::vector<Pos> matches = checkMatch(*itr);

			// if a match involving the current square was found
			if (!matches.empty()) {

				Square* specialSquare = NULL;
				Square::Colour matchColour = _board->getSquare(matches.front())->getColour();

				if (_InitMode) {
					// don't make special squares in init mode
				}
				// if it's a line of 4
				else if (matches.size() == 4) {
					// if two squares are on the same row, it's a horizontal match
					if (matches[0].row == matches[1].row) {
						specialSquare = new LateralSquare(matches[0], matchColour);
					}
					// otherwise, it must be a vertical match
					else if (matches[0].col == matches[1].col) {
						specialSquare = new UprightSquare(matches[0], matchColour);
					}
#ifdef _DEBUG
					// sanity check
					else
						std::cout << "something went wrong: match of 4, but two squares aren't on the same line" << std::endl;
#endif
				}
				// more than 5 squares involved in the match
				else if (matches.size() >= 5) {
					bool straight = true;

					// get one of the positions in the set of matches
					std::vector<Pos>::iterator itr2 = matches.begin();
					Pos firstPos = *itr2;
					itr2++;

					// determine if the common coordinate is row or column
					bool isRowShared = firstPos.row == itr2->row;
					itr2++;

					// determine if it's an L- or a T- match
					while (straight && itr2 != matches.end()) {
						// if it's a line match, either the row or the col is shared among all positions
						// if this is not the case, it's an L- or T- match
						if ((isRowShared && itr2->row != firstPos.row) ||
							(!isRowShared && itr2->col != firstPos.col))
							straight = false;
						itr2++;
					}

					if (straight) {
						specialSquare = new PsychSquare(matches[0], matchColour);
					}
					else {
						specialSquare = new UnstableSquare(matches[0], matchColour);
					}
				}

				int numDestroyed = 0;

				for (std::vector<Pos>::iterator itr = matches.begin(); itr != matches.end(); itr++) {
					_board->getSquare(*itr)->destroy(numDestroyed, matches.size());
				}
				//increase score
				if (!_InitMode) _score->score(numDestroyed,chain);
				if (specialSquare != NULL) {
					specialSquare->setModified(true);
					_board->addSquare(specialSquare);
				}
			}
		}

		chain++;

		// draw the intermediate step, if required
		if (!skipBreakdown && _drawBreakdown) {
			for (deque<BoardDisplay*>::iterator itr = _displays.begin();
				 itr != _displays.end(); itr++) {
				(*itr)->draw();
			}
		}

		_updated.clear();

		plug();

		// draw the intermediate step, if required
		if (!skipBreakdown && _drawBreakdown && !_updated.empty()) {
			for (deque<BoardDisplay*>::iterator itr = _displays.begin();
				 itr != _displays.end(); itr++) {
				(*itr)->draw();
			}

			string input;
			getline(cin, input);
			if (input == "skip")
				skipBreakdown = true;
		}
	}
}


void BoardManip::plug() {
	Pos boardSize = _board->getSize();
	Pos current(0, 0);

	//make squares fall down column by column
	for (current.col = 0; current.col < boardSize.col; current.col++) {
		int emptySquares = 0;
		//start at the bottom of column, iterate until you reach top of column
		for (current.row = boardSize.row - 1; current.row >= 0; current.row--) {
			//if square is empty, increment emptySquares
			if (_board->getSquare(current)->getColour() == Square::EMPTY) {
				emptySquares++;
			}
			//shift square down by emptySquares if there are empty squares below
			else if (emptySquares != 0) {
				Pos swapPos(current.row + emptySquares, current.col);
				_board->swap(current, swapPos);

				// mark a square as updated if it fell
				addSquareToUpdated(swapPos, _updated, _board);
			}
		}

		//value of emptySquares now tells us how many empty squares are at the top of the column
		//fill the empty squares
		for (current.row = emptySquares - 1; current.row >= 0; current.row--) {
			Square * next = _level->nextSquare(current);

			//std::cout << "plug: generated a " << next->getColour() << " at " << next->getPos() << std::endl;

			_board->addSquare(next);

			// mark the newly generated square as updated
			addSquareToUpdated(current, _updated, _board);
		}
	}
}


bool BoardManip::findMatch() {
	Pos boardSize = _board->getSize();

	//check for matches in every square by checking for a line of 3 same-coloured square
	for (int row = 0; row < boardSize.row; row++) {
		for (int col = 0; col < boardSize.col; col++) {
			if (!checkMatch(Pos(row,col)).empty()) {return true;}	
		}
	}

	return false;
}


int countMatches(const Pos& centre, BoardManip::Direction dir, Board* board, Square::Colour colour) {
	if (!board->withinBounds(centre)) {
		std::cout << "countMatches: centre is out of bounds" << std::endl;
		return 0;
	}

	Pos moveDir = dirToPos(dir);
	Pos current = centre + moveDir;
	int matches = 0;

	// if no colour was provided, use the colour of the square at centre
	if (colour == Square::EMPTY)
		colour = board->getSquare(centre)->getColour();

	// increment matches until a colour doesn't match or the edge of the board is reached
	while (board->withinBounds(current) && board->getSquare(current)->getColour() == colour) {
		matches++;
		current = current + moveDir;
	}

	return matches;
}


void addPosLineToList(const Pos& start, const Pos& end, std::vector<Pos>& list, const Pos& omit) {

	if (start.row != end.row && start.col != end.col)
		std::cout << "invalid positions in addPosLineToList" << std::endl;

	// determine the "unit vector" used for iterating across the line
	Pos incr;
	if (start.row == end.row)
		incr.row = 0;
	else
		incr.row = start.row < end.row ? 1 : -1;
	if (start.col == end.col)
		incr.col = 0;
	else
		incr.col = start.col < end.col ? 1 : -1;

	// add all the positions (except omit) to the list
	for (Pos current = start; current != end + incr; current = current + incr) {
		if (current != omit)
			list.push_back(current);
	}
}


std::vector<Pos> BoardManip::checkMatch(Pos square) {
	std::vector<Pos> result;

	if (_board->getSquare(square)->getColour() != Square::EMPTY) {
		// get a list of the number of matching squares in each direction
		int matches[4];
		for (int i = 0; i < 4; i++) {
			matches[i] = countMatches(square, (BoardManip::Direction)i, _board);
		}

		bool isMatch = true;
		BoardManip::Direction matchDir;

		// determine if there is a match, and its direction (if applicable)
		if (matches[NORTH] + matches[SOUTH] >= 2)
			matchDir = NORTH;
		else if (matches[WEST] + matches[EAST] >= 2)
			matchDir = WEST;
		else
			isMatch = false;

		if (isMatch) {
			Direction opMatchDir = opDir(matchDir); // the other direction of the matched line

			// add the matched squares to the list
			// start at square offset by the number of matches in the direction of matchDir
			addPosLineToList(square + (matches[matchDir] * dirToPos(matchDir)),
				// end at square offset by the number of matches in the direction of opMatchDir
				square + (matches[opMatchDir] * dirToPos(opMatchDir)),
				result);

			//TODO: clarify if the Lateral/Horizontal Square's position must be randomnly chosen between the two possible choices
			// I posted a question on Piazza: search "ss5k Newly generated Lateral and Horizontal Squares"	
			// if the match is longer than 3 squares, move the third element to the front
			// this means that in lines of 4 or 5, the third element will become a special square
			if (result.size() > 3)
				posListSwap(result, 0, 2);

			// search for a L- or T-match

			// these are the directions perpendicular to the matched line
			Direction altDir1 = rotDir(matchDir);
			Direction altDir2 = opDir(altDir1);

			// stuff for iteration
			Pos intersect(-1, -1);
			int intersectIndex = -1;
			std::vector<Pos>::iterator itr = result.begin();
			int altDir1Matches;
			int altDir2Matches;

			// iterate through the matched line
			while (intersectIndex == -1 && itr != result.end()) {
				// count the number of colour matches in the 
				//   perpendicular directions of the matched line
				altDir1Matches = countMatches(*itr, altDir1, _board);
				altDir2Matches = countMatches(*itr, altDir2, _board);

				// if there is an intersecting match
				if (altDir1Matches + altDir2Matches >= 2) {
					intersect = (*itr);
					intersectIndex = itr - result.begin();
				}

				itr++;
			}

			// if an intersecting match was found
			if (intersectIndex != -1) {
				// add the perpendicular line to the list of matched squares
				// start at the position of the current square offset
				//   by the number of matches in the first direction
				addPosLineToList(intersect + (altDir1Matches * dirToPos(altDir1)),
					// end at the position of the current square offset
					//   by the number of matches in the second direction
					intersect + (altDir2Matches * dirToPos(altDir2)),
					result, intersect);

#ifdef _DEBUG
				// sanity check
				if (result.size() < 5) {
					std::cout << "something went wrong: L- or T- match involving fewer than 5 squares" << std::endl;
				}
#endif

				// move the point of intersection to the front of the list
				//   to mark it as the location of the special square
				posListSwap(result, 0, intersectIndex);
			}
		}
	}

	return result;
}


Pos dirToPos(BoardManip::Direction dir) {
	Pos result(0, 0);

	switch (dir) {
	case BoardManip::NORTH: result.row = -1; break;
	case BoardManip::SOUTH: result.row = 1; break;
	case BoardManip::WEST:  result.col = -1; break;
	case BoardManip::EAST:  result.col = 1; break;
	default:
#ifdef _DEBUG
		std::cout << "invalid direction in dirToPos" << std::endl;
#endif
		break;
	}

	return result;
}


BoardManip::Direction rotDir(BoardManip::Direction dir) {
	BoardManip::Direction result = BoardManip::NORTH;

	switch (dir) {
	case BoardManip::NORTH: result = BoardManip::EAST; break;
	case BoardManip::SOUTH: result =  BoardManip::WEST; break;
	case BoardManip::WEST:  result =  BoardManip::NORTH; break;
	case BoardManip::EAST:  result =  BoardManip::SOUTH; break;
	default:
#ifdef _DEBUG
		std::cout << "invalid direction in rotateDirection" << std::endl;  
#endif
		break;
	}

	return result;
}


BoardManip::Direction opDir(BoardManip::Direction dir) {
	BoardManip::Direction result = BoardManip::NORTH;

	switch (dir) {
	case BoardManip::NORTH: result = BoardManip::SOUTH; break;
	case BoardManip::SOUTH: result =  BoardManip::NORTH; break;
	case BoardManip::WEST:  result =  BoardManip::EAST; break;
	case BoardManip::EAST:  result =  BoardManip::WEST; break;
	default:
#ifdef _DEBUG
		std::cout << "invalid direction in oppositeDirection" << std::endl;
#endif
		break;
	}

	return result;
}


void posListSwap(std::vector<Pos>& list, int i1, int i2) {
	Pos temp = list[i1];
	list[i1] = list[i2];
	list[i2] = temp;
}


void addSquareToUpdated(Pos pos, deque<Pos>& updated, Board* board) {
	updated.push_back(pos);
	board->getSquare(pos)->setModified(true);
}

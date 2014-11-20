#include "BoardManip.h"
#include "Level.h"
#include "DebugDisplay.h"

#include <iostream>

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

BoardManip::BoardManip(Board* board, Score* score): _board(board), _score(score), _noScoringMode(true) { }

void BoardManip::swap(Pos pos, Direction dir){
	Pos moveDir = dirToPos(dir);
	Pos pos2(pos.row + moveDir.row, pos.col + moveDir.col);

	if (!_board->withinBounds(pos) || !_board->withinBounds(pos2))
		std::cout << "swap coordinates out of bounds" << std::endl;
	else if (_board->isLocked(pos) || _board->isLocked(pos2))
		std::cout << "trying to swap locked squares" << std::endl;
	else {
		_board->swap(pos, pos2);

		_updated.push_back(pos);
		_updated.push_back(pos2);

		std::cout << "Post-swap:" << std::endl;
		DebugDisplay::printBoard();
		std::cout << std::endl;

		update();
	}
}


void BoardManip::resetBoard(){
	Pos boardSize = _board->getSize();

	for (int row = 0; row < boardSize.row; row++) {
		for (int col = 0; col < boardSize.col; col++) {
			_board->removeSquare(Pos(row, col));
		}
	}

	plug();
	update();
}


void BoardManip::scramble(){
	Direction dir = {NORTH};

	while (!findMove(Pos(0,0),dir)) {
		resetBoard();
	}
}

//for a given square, checks how many of its neighbour have the same colour as the colour we're searching
bool findMoveAtSquare(Square::Colour searchColour, Pos search, Board* board, int alreadyMatched) {
	const int directionsToSearch = 4;
	Pos orientation[directionsToSearch] = {Pos(search.row-1,search.col),Pos(search.row+1,search.col),
		Pos(search.row+1,search.col),Pos(search.row,search.col+1)};
	int moves = 0;
	for (int i = 0; i < directionsToSearch; i++) {
		if (searchColour == board->getSquare(orientation[i])->getColour()) moves++;
	}
	if (moves > (directionsToSearch - alreadyMatched)) {return true;} else {return false;}
}

//TODO: document this method properly; currently hard to understand / rewrite as a big for loop?
//findMove actually has a few more cases then I thought
//its better to explain in person
bool BoardManip::findMove(Pos& start, Direction& dir) {
	Pos boardSize = _board->getSize();
	Pos current(0,0);
	//check for moves row by row
	for (current.row = 0; current.row < boardSize.row; current.row++) {
		for (current.col = 0; current.col < boardSize.col-2; current.col++) { 
			//get the current square's colour
			Square::Colour currentColour = (_board->getSquare(current))->getColour();
			//get the next square in the row and the next of next
			Square * next = _board->getSquare(Pos(current.row,current.col+1));
			Square * nextOfNext = _board->getSquare(Pos(current.row,current.col+2));
			//if two adjacent squares have the same colour, check there is a move involving the square before current or nextOfnext
			if (next->getColour() == currentColour) {
				if (findMoveAtSquare(currentColour,Pos(current.row, current.col-1),_board,1)|| findMoveAtSquare(currentColour,nextOfNext->getPos(),_board,1)) {
					return true;
				}
			}
			//if two square of the same colour are separated by a square
			else if (nextOfNext->getColour() == currentColour) {
				if (findMoveAtSquare(currentColour,next->getPos(),_board,2)) {
					return true;
				}
			}
		}
	}
	
	//check for moves col by col
	for (current.col = 0; current.col < boardSize.col; current.col++) {
		for (current.row = 0; current.row < boardSize.row-2; current.row++) { 
			//get the current square's colour
			Square::Colour currentColour = (_board->getSquare(current))->getColour();
			//get the next square in the col and the next of next
			Square * next = _board->getSquare(Pos(current.row+1,current.col));
			Square * nextOfNext = _board->getSquare(Pos(current.row+2,current.col));
			//if two adjacent squares have the same colour, check there is a move involving the square before current or nextOfnext
			if (next->getColour() == currentColour) {
				if (findMoveAtSquare(currentColour, Pos(current.row-1, current.col), _board, 1) ||
					findMoveAtSquare(currentColour, nextOfNext->getPos(), _board, 1)) {
					return true;
				}
			}
			//if two square of the same colour are separated by a square
			else if (nextOfNext->getColour() == currentColour) {
				if (findMoveAtSquare(currentColour,next->getPos(),_board,2)) {
					return true;
				}
			}
		}
	}

	//no move in the board
	return false;
}


void BoardManip::setLevel(Level* level) {
	_level = level;
}


void BoardManip::update() {
	Pos start(0, 0);
	Pos end(0, 0);
	Pos third(0, 0);

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

				// if it's a line of 4
				if (matches.size() == 4) {
					// if two squares are on the same row, it's a horizontal match
					if (matches[0].row == matches[1].row) {
						std::cout << "(pretend I generated a LateralSquare)" << std::endl;
						// specialSquare = new LateralSquare(matches[0], matchColour);
					}
					// otherwise, it must be a vertical match
					else if (matches[0].col == matches[1].col) {
						std::cout << "(pretend I generated an UprightSquare)" << std::endl;
						// specialSquare = new UprightSquare(matches[0], matchColour);
					}
					// sanity check
					else
						std::cout << "something went wrong: match of 4, but two squares aren't on the same line" << std::endl;
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
						std::cout << "(pretend I generated a PsychSquare)" << std::endl;
						// specialSquare = new PsychSquare(matches[0], matchColour);
					}
					else {
						std::cout << "(pretend I generated an UnstableSquare)" << std::endl;
						// specialSquare = new UnstableSquare(matches[0], matchColour);
					}
				}

				int numDestroyed = 0;

				for (std::vector<Pos>::iterator itr = matches.begin(); itr != matches.end(); itr++) {
					std::cout << "update: destroyed at " << (*itr) << std::endl;
					_board->getSquare(*itr)->destroy(numDestroyed, matches.size());
				}
			}
		}

		_updated.clear();

		std::cout << "update: after checking updated squares:" << std::endl;
		DebugDisplay::printBoard();

		plug();
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
				_updated.push_back(swapPos);
			}
		}

		//value of emptySquares now tells us how many empty squares are at the top of the column
		//fill the empty squares
		for (current.row = emptySquares - 1; current.row >= 0; current.row--) {
			Square * next = _level->nextSquare();
			next->setPos(current);

			std::cout << "plug: generated a " << next->getColour() << " at " << current << std::endl;

			_board->addSquare(next);

			// mark the newly generated square as updated
			_updated.push_back(current);
		}
	}

	std::cout << "end of plug:" << std::endl;
	DebugDisplay::printBoard();
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
	Pos moveDir = dirToPos(dir);
	Pos current(centre.row + moveDir.row, centre.col + moveDir.col);
	int matches = 0;

	// if no colour was provided, use the colour of the square at centre
	if (colour == Square::EMPTY)
		colour = board->getSquare(centre)->getColour();

	// increment matches until a colour doesn't match or the edge of the board is reached
	while (board->withinBounds(current) && board->getSquare(current)->getColour() == colour) {
		matches++;
		current.row += moveDir.row;
		current.col += moveDir.col;
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

				// sanity check
				if (result.size() < 5) {
					std::cout << "something went wrong: L- or T- match involving fewer than 5 squares" << std::endl;
				}

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
	default: std::cout << "invalid direction in dirToPos" << std::endl;
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
	default: std::cout << "invalid direction in rotateDirection" << std::endl;
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
	default: std::cout << "invalid direction in oppositeDirection" << std::endl;
	}

	return result;
}


void posListSwap(std::vector<Pos>& list, int i1, int i2) {
	Pos temp = list[i1];
	list[i1] = list[i2];
	list[i2] = temp;
}


/*
//checks for colour matches
int colourCheck(Square::Colour one,Square::Colour two, Square::Colour three) {
	if (one == two && one == three) return 3;
	if (one == two) return 2;
	return 0;
}

//for a match of 3, do we leave the third Pos untouched?
//For L matches, end is always the horizontal endpoint, third is the vertical endpoint
int BoardManip::checkMatch(Pos square, Pos& start, Pos& end, Pos& third){
	int result = 0;
	//how many squares are matched in each direction
	int left, right, up , down  = 0;
	Square::Colour currentColour = (_board->getSquare(square))->getColour();
	int row = square.row;
	int col = square.col;
	Pos BoardSize = _board->getSize();
	int rows = BoardSize.row;
	int cols = BoardSize.col;
	//check for matches in all direction
           //check left
			if (col > 1) {
				Square::Colour tempColour1 = (_board->getSquare(Pos(row,col-1)))->getColour();
				Square::Colour tempColour2 = (_board->getSquare(Pos(row,col-2)))->getColour();
				left = colourCheck(currentColour, tempColour1, tempColour2);

			}
			//check right
			if (col < cols -2) {
				Square::Colour tempColour1 = (_board->getSquare(Pos(row,col+1)))->getColour();
				Square::Colour tempColour2 = (_board->getSquare(Pos(row,col+2)))->getColour();
				right = colourCheck(currentColour, tempColour1, tempColour2);
			}
			//check above
			if (row > 1) {
				Square::Colour tempColour1 = (_board->getSquare(Pos(row,row-1)))->getColour();
				Square::Colour tempColour2 = (_board->getSquare(Pos(row,row-2)))->getColour();
				up = colourCheck(currentColour, tempColour1, tempColour2);
			}
			//check below
			if (row< rows - 2) {
				Square::Colour tempColour1 = (_board->getSquare(Pos(row,row+1)))->getColour();
				Square::Colour tempColour2 = (_board->getSquare(Pos(row,row+2)))->getColour();
				down = colourCheck(currentColour, tempColour1, tempColour2);
			}
	//check for L
			if ((left == 2 && up == 2) || (left == 2 && down == 2) ||
			    (right == 2 && up == 2) || (right == 2 && down == 2)) {
					start = square;
					//set horizaton end point
					end.row = row;
					if (left == 2) { 
						end.col = row -2;
					} else {
						end.col = row +2;
					}
					//set vertical endpoint
					third.col = col;
					if (up == 2) {
						third.row = row -2;
					} else {
						third.row = row + 2;
					}
			}
    //check for 5 in a row
			if ((left == 2 && right ==2) || (up == 2 && down == 2)) {
				
				//horizontal line
				if (left == 2) {
					end.row = row;
					third.row = row;
					end.col = col - 2;
					third.col = col + 2;
				}
				//vertical line
				else {
					end.col = col;
					third.col = col;
					end.row = row - 2;
					third.row = row + 2;
				}
			}
    //check for 4 in a row
		//horizontal line
			//left side shorter
			if (left == 1 && right== 2) {
				   start = square;
					end.row = row;
					third.row = row;
					end.col = col - 1;
					third.col = col + 2;

			} 
			//right side shorter
			else if (left == 2 && right== 1) {
					start = square;	
					end.row = row;
					third.row = row;
					end.col = col - 2;
					third.col = col + 1;
			}
		//vertical line
			//up shorter
			else if (up == 1 && down ==2) {
					start = square;
					end.col = col;
					third.col = col;
					end.row = row - 1;
					third.row = row + 2;
			}
			//down shorter
			else if ( up == 2 && down == 1) {
					start = square;
					end.col = col;
					third.col = col;
					end.row = row - 2;
					third.row = row + 1;
			}
    //check for regular match
			//match to the left
			if (left == 2) {
				start = square;
				end.row = row;
				end.col = col -2;
			}
			else if (right == 2) {
				start = square;
				end.row = row;
				end.col = col + 2;
			}
			else if (up == 2) {
				start = square;
				end.row = row - 2;
				end.col = col;
			}
			else if (down == 2) {
				start = square;
				end.row = row + 2;
				end.col = col;
			}
	return result;
}
*/

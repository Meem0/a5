#include "BoardManip.h"
#include <iostream>

using namespace std;

// returns a "unit vector" based on a Direction
// ex: NORTH = (-1, 0), EAST = (0, 1)
Pos dirToPos(BoardManip::Direction);

// for the temporary checkMatch solution
// returns the number of squares (capped at 2) whose colour matches
//   that of the square at centre in the given direction
int countMatches(Pos centre, BoardManip::Direction dir, Board* board);

void printBoard(const Board &board) {
	for (int row = 0; row < board.getSize().row; row++) {
		for (int col = 0; col < board.getSize().col; col++) {
			board.getSquare(Pos(row, col))->textDraw();
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}

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
		printBoard(*_board);
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
		if (searchColour == board->getSquare(orientation[i])->getColour) moves++;
	}
	if (moves > (directionsToSearch - alreadyMatched)) {return true;} else {return false;}
}

//TODO: document this method properly; currently hard to understand / rewrite as a big for loop?
//findMove actually has a few more cases then I thought
//its better to explain in person
bool BoardManip::findMove(Pos& start, Direction& dir){
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
			//if two adjacent squares have the same colour, check if a move is possible with the square before current
			//or with the square after next
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
	
	//check for moves col by col
	for (current.col = 0; current.col < boardSize.col; current.col++) {
		for (current.row = 0; current.row < boardSize.row-2; current.row++) { 
			//get the current square's colour
			Square::Colour currentColour = (_board->getSquare(current))->getColour();
			//get the next square in the col and the next of next
			Square * next = _board->getSquare(Pos(current.row+1,current.col));
			Square * nextOfNext = _board->getSquare(Pos(current.row+2,current.col));
			//if two adjacent squares have the same colour, check if a move is possible with nextOfNext
			if (next->getColour() == currentColour) {
				if (findMoveAtSquare(currentColour,Pos(current.row-1, current.col),_board,1)||findMoveAtSquare(currentColour,nextOfNext->getPos(),_board,1){
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


void BoardManip::setLevel(Level* level){
	_level = level;
}


// TODO: make this work for L-matches
void BoardManip::update() {
	Pos start(0, 0);
	Pos end(0, 0);
	Pos third(0, 0);

	plug();

	while (!_updated.empty()) {
		// iterate through all the updated squares
		for (auto itr = _updated.begin(); itr != _updated.end(); itr++) {
			// if a match involving the current square was found
			if (checkMatch(*itr, start, end, third)) {
				// figure out whether to iterate forward or backwards
				// (this allows the order of start/end to not matter)
				int incr = (end.row - start.row < 0 || end.col - start.col < 0) ?
					-1 : 1;

				int destroyed = 0;

				// only one of these loops will run more than once,
				//   which is why there's only one incr
				for (int row = start.row; row <= end.row; row += incr) {
					for (int col = start.col; col <= end.col; col += incr) {
						// second parameter will be used for L-matches
						_board->getSquare(Pos(row, col))->destroy(destroyed, -1);
						std::cout << "update: destroyed at ("
								  << row << ", " << col << ")" << std::endl;
					}
				}
			}
		}

		_updated.clear();

		plug();
	}
}

void BoardManip::plug() {
	Pos boardSize = _board->getSize();
	Pos current(0, 0);

	//make squares fall down column by column
	for (current.col = 0; current.col < boardSize.col; current.col++) {
		int shiftIndex = 0;
		//start at the bottom of column, iterate until you reach top of column
		for (current.row = boardSize.row - 1; current.row >= 0; current.row--) {
			//if square is empty, increment shiftIndex
			if (_board->getSquare(current)->getColour() == Square::EMPTY) {
				shiftIndex++;
			}
			//shift square down by shiftIndex
			else {
				_board->removeSquare(current);
				_board->swap(current,Pos(current.row- shiftIndex, current.col));
			}
		}
		//value of shiftIndex now tells us how many empty squares are at the top of the column
		//fill the empty squares
		for (int i = 0; i < shiftIndex; i++) {
		//TODO: Requires a implementation for Level class
		Square * next; //remove once line below is valid
		//Square * next = _level->nextSquare(); //poitner to imcomplete class type error
		next->setPos(Pos(i,current.col));
		_board->addSquare(next);
		}
	}
}


bool BoardManip::findMatch() {
	Pos boardSize = _board->getSize();
	
	//check for matches in every square by checking for a line of 3 same-coloured square
	for (int row = 0; row < boardSize.row; row++) {
		for (int col = 0; col < boardSize.col; col++) {
			if (checkMatch(Pos(row,col), Pos(0,0),Pos(0,0),Pos(0,0))) {return true;}	
		}
	}

	return false;
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


int countMatches(Pos centre, BoardManip::Direction dir, Board* board) {
	Pos moveDir = dirToPos(dir);
	Pos current(centre.row, centre.col);
	Square::Colour centreColour = board->getSquare(centre)->getColour();
	int matches = 0;
	
	for (int i = 0; i < 2; i++) {
		current.row += moveDir.row;
		current.col += moveDir.col;
		
		if (board->withinBounds(current) && board->getSquare(current)->getColour() == centreColour)
			matches++;
		else
			break;
	}
	
	return matches;
}


// temporary working solution for testing
int BoardManip::checkMatch(Pos square, Pos& start, Pos& end, Pos& third) {
	int result = 0;
	int matches[4];
	for (int i = 0; i < 4; i++) {
		matches[i] = countMatches(square, (BoardManip::Direction)i, _board);
	}
	
	if (matches[NORTH] + matches[SOUTH] >= 2) {
		start.row = square.row - matches[NORTH];
		start.col = square.col;
		
		end.row = square.row + matches[SOUTH];
		end.col = square.col;
		
		result = 1;
	}
	else if (matches[WEST] + matches[EAST] >= 2) {
		start.row = square.row;
		start.col = square.col - matches[WEST];
		
		end.row = square.row;
		end.col = square.col + matches[EAST];

		result = 1;
	}
	
	return result;
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

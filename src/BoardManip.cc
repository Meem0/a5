#include "BoardManip.h"


using namespace std;

BoardManip::BoardManip(Board* board, Score* score): _board(board),_score(score), _noScoringMode(true){
}

void BoardManip::swap(Pos start, Direction dir){
	// is move validation done at this step?
	//check for locked cells
	//swap by add/remove squares or by swapping ptrs?	
}


void BoardManip::resetBoard(){
	Pos BoardSize = _board->getSize();
	delete _board;
	_board = new Board(BoardSize.row,BoardSize.col);
}


void BoardManip::scramble(){
	Direction dir = {NORTH};
	//how to
	while (!findMove(Pos(0,0),dir)) {
		resetBoard();
	}
}

//lots of repeated code; can probably be simplifed later on :(
bool BoardManip::findMove(Pos& start, Direction& dir){
	Pos BoardSize = _board->getSize();
	int rows = BoardSize.row;
	int cols = BoardSize.col;
	//check for moves row by row
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols-2; col++) { 
			//get the current square's colour
			Square::Colour currentColour = (_board->getSquare(Pos(row,col)))->getColour();
			//get the next square in the row and the next of next
			Square * next = _board->getSquare(Pos(row,col+1));
			Square * nextOfNext = _board->getSquare(Pos(row,col+2));

			//if two adjacent squares have the same colour, check if a move is possible with nextOfNext
			if (next->getColour() == currentColour) {
				//check above nextOfNext, making sure there is a square there
				if (row > 0) {
					Square::Colour tempColour = (_board->getSquare(Pos(row-1,col)))->getColour();
					if (tempColour == currentColour) {
						return true;
					}
				}
				//check below nextOfNext, making sure there is a square below
				else if (row < rows-1) {
					Square::Colour tempColour = (_board->getSquare(Pos(row+1,col)))->getColour();
					if (tempColour == currentColour) {
						return true;
					}
				} 
			}
			//if two square of the same colour are separated by a square
			else if (nextOfNext->getColour == currentColour) {
				//check above next; make sure a square exist there
				if (row > 0) {
					Square::Colour tempColour = (_board->getSquare(Pos(row-1,col)))->getColour();
					if (tempColour == currentColour) {
						return true;
					}
				}
				//check below next
				else if (row < rows-1) {
					Square::Colour tempColour = (_board->getSquare(Pos(row+1,col)))->getColour();
					if (tempColour == currentColour) {
						return true;
					}
				}
			}
		}
	}
	//check for moves col by col
	for (int col = 0; col < cols; col++) {
		for (int row = 0; row < rows-2; row++) { 
			//get the current square's colour
			Square::Colour currentColour = (_board->getSquare(Pos(row,col)))->getColour();
			//get the next square in the col and the next of next
			Square * next = _board->getSquare(Pos(row+1,col));
			Square * nextOfNext = _board->getSquare(Pos(row+2,col));

			//if two adjacent squares have the same colour, check if a move is possible with nextOfNext
			if (next->getColour() == currentColour) {
				//check left of nextOfNext, making sure there is a square there
				if (col > 0) {
					Square::Colour tempColour = (_board->getSquare(Pos(row,col-1)))->getColour();
					if (tempColour == currentColour) {
						return true;
					}
				}
				//check right of nextOfNext, making sure there is a square below
				else if (col < cols-1) {
					Square::Colour tempColour = (_board->getSquare(Pos(row,col+1)))->getColour();
					if (tempColour == currentColour) {
						return true;
					}
				} 
			}
			//if two square of the same colour are separated by a square
			else if (nextOfNext->getColour == currentColour) {
				//check left of next; make sure a square exist there
				if (col > 0) {
					Square::Colour tempColour = (_board->getSquare(Pos(row,col-1)))->getColour();
					if (tempColour == currentColour) {
						return true;
					}
				}
				//check right of next
				else if (col < cols-1) {
					Square::Colour tempColour = (_board->getSquare(Pos(row,col+1)))->getColour();
					if (tempColour == currentColour) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

void BoardManip::setLevel(Level* level){
	_level = level;
}


void BoardManip::update(){}


void BoardManip::plug(){}


bool BoardManip::findMatch() {
	Pos BoardSize = _board->getSize();
	int rows = BoardSize.row;
	int cols = BoardSize.col;
	//check for matches in every square by checking for a line of 3 same-coloured square
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols-2; col++) {
			if (checkMatch(Pos(row,col), Pos(0,0),Pos(0,0),Pos(0,0))) {return true;}	
		}
	}
	return false;
}

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

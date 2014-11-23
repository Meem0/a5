#include "Score.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

Score::Score():_score(0),_highScore(0){}

int Score::getScore() const {
	return _score;
}

int Score::getHighScore() const {
	return _highScore;
}


//apparently there is no built-in exponential function that returns int
void Score::score(int tilesCleared, int chain) {
	int minMatchSize = 3;
	if (tilesCleared < 3) {
		cout << "error: trying to add score on no match" << endl;
	} else {
		/*multipliers : tilesCleared == 3 => 1
						tilesCleared == 4 => 2
						tilesCleared == 5 => 3
						tilesCleared == 6+ => 4
		*/
		// chain multiplier == 2^ chain
		_score =_score + (min(4,tilesCleared -minMatchSize+1) * tilesCleared) * static_cast<int>(pow(2,chain)); 
	}
}
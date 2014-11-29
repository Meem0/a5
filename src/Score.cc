#include "Score.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

Score::Score() : _score(0), _highScore(0) {
	ifstream hsIn(HIGH_SCORE_FILE_NAME);

	// read in previous high score, if possible
	if (hsIn)
		hsIn >> _highScore;
}

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

		// set new high score
		if (_score > _highScore) {
			_highScore = _score;

			ofstream hsOut(HIGH_SCORE_FILE_NAME);
			hsOut << _highScore;
		}
	}
}

const char* Score::HIGH_SCORE_FILE_NAME = "highscore.txt";

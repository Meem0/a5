#ifndef SCORE_H
#define SCORE_H

class Score {
public:
	Score();

	int getScore() const;
	int getHighScore() const;
	// scores points based on the number of tiles cleared
	// multiplies the amount scored by 2^chain
	void score(int tilesCleared, int chain);

private:
	int _score;
	int _highScore;

	static const char* HIGH_SCORE_FILE_NAME;
};

#endif

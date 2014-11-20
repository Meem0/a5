#ifndef POS_H
#define POS_H

#include <iostream>

class Pos {
public:
	Pos();
	Pos(int r, int c);

	bool operator==(const Pos&) const;
	bool operator!=(const Pos&) const;
	Pos operator+(const Pos&) const;
	Pos operator-(const Pos&) const;
//	Pos operator*(const int&) const;

	int row;
	int col;
};

Pos operator*(const int&, const Pos&);
Pos operator*(const Pos&,const int&);
std::ostream& operator<<(std::ostream&, const Pos&);

#endif

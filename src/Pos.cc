#include "Pos.h"

Pos::Pos() : row(0), col(0) { }

Pos::Pos(int r, int c) : row(r), col(c) { }

bool Pos::operator==(const Pos& other) const {
	return row == other.row && col == other.col;
}

bool Pos::operator!=(const Pos& other) const {
	return row != other.row || col != other.col;
}

Pos Pos::operator+(const Pos& other) const {
	return Pos(row + other.row, col + other.col);
}

Pos Pos::operator-(const Pos& other) const {
	return Pos(row - other.row, col - other.col);
}
/*
Pos Pos::operator*(const int& scalar) const {
	return Pos(row * scalar, col * scalar);
}
*/

Pos operator*(const int& scalar, const Pos& pos) {
	return Pos(pos.row * scalar, pos.col * scalar);
}

Pos operator*(const Pos& pos,const int& scalar) {
	return scalar * pos;
}

std::ostream& operator<<(std::ostream& out, const Pos& pos) {
	out << "(" << pos.row << ", " << pos.col << ")";
	return out;
}

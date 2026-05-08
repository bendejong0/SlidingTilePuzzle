#include "PuzzleState.h"

bool PuzzleState::operator==(const PuzzleState& rhs) const{
	for (size_t i{ 0 }; i < tiles.size(); i++) {
		if (this->tiles[i] != rhs.tiles[i])
			return false;
	}
	return true;
}

bool PuzzleState::operator!=(const PuzzleState& rhs) const {
	return !(*this == rhs);
}

const PuzzleState& PuzzleState::operator=(const PuzzleState& rhs){
	rows = rhs.rows;
	cols = rhs.cols;
	tiles = rhs.tiles;
	blank_position_row = rhs.blank_position_row;
	blank_position_col = rhs.blank_position_col;
	return *this;
}

bool PuzzleState::canMoveUp(){
	return blank_position_row > 0;
}

bool PuzzleState::canMoveDown(){
	return blank_position_row < rows - 1;
}

bool PuzzleState::canMoveLeft(){
	return blank_position_col > 0;
}

bool PuzzleState::canMoveRight(){
	return blank_position_col < cols - 1;
}

PuzzleState PuzzleState::moveBlankUp(){
	PuzzleState temp(*this);
	auto blankAlias = &temp.tiles[blank_position_row * cols + temp.blank_position_col];
	auto moveBlankHere = &temp.tiles[(blank_position_row - 1) * cols + temp.blank_position_col];

	swap(*blankAlias, *moveBlankHere);

	temp.blank_position_row--;
	return temp;
}

PuzzleState PuzzleState::moveBlankDown(){
	PuzzleState temp(*this);
	auto blankAlias = &temp.tiles[temp.blank_position_row * cols + temp.blank_position_col];
	auto moveBlankHere = &temp.tiles[(temp.blank_position_row + 1) * cols + temp.blank_position_col];

	swap(*blankAlias, *moveBlankHere);

	temp.blank_position_row++;
	return temp;
}

PuzzleState PuzzleState::moveBlankLeft(){
	PuzzleState temp(*this);
	auto blankAlias = &temp.tiles[temp.blank_position_row * cols + temp.blank_position_col];
	auto moveBlankHere = &temp.tiles[(temp.blank_position_row) * cols + temp.blank_position_col-1];

	swap(*blankAlias, *moveBlankHere);

	temp.blank_position_col--;
	return temp;
}

PuzzleState PuzzleState::moveBlankRight(){
	PuzzleState temp(*this);
	auto blankAlias = &temp.tiles[temp.blank_position_row * cols + temp.blank_position_col];
	auto moveBlankHere = &temp.tiles[(temp.blank_position_row) * cols + temp.blank_position_col+1];

	swap(*blankAlias, *moveBlankHere);

	temp.blank_position_col++;
	return temp;
}

vector<int> PuzzleState::getTiles() const{
	return this->tiles;
}

void PuzzleState::read(istream& in) {
	for (size_t row{ 0 }; row < rows; row++)
		for (size_t col{ 0 }; col < cols; col++)
			in >> tiles[row*cols+col];
	
	for (size_t i = 0; i < tiles.size(); i++) {
		if (tiles[i] == 0) {
			blank_position_row = i / cols;
			blank_position_col = i % cols;
		}
	}
}

void PuzzleState::print(ostream& out) {
	for (size_t row{ 0 }; row < rows; row++) {
		for (size_t col{ 0 }; col < cols; col++)
			out << tiles[row * cols + col] << " ";
		out << endl;
	}
}

istream& operator>>(istream& in, PuzzleState& rhs) {
	rhs.read(in);
	return in;
}
ostream& operator<<(ostream& out, PuzzleState& rhs) {
	rhs.print(out);
	return out;
}

PuzzleState PuzzleState::NullState = PuzzleState();
#include "Heuristic.h"
#include "PuzzleMove.h"
#include <cmath>

// Sets up the heuristic to compute the Manhattan distance.
Heuristic::Heuristic(PuzzleState g) : goal(g) {

    // set N
    N = g.getTiles().size();

	vector<int> table(g.getTiles().size());
    // build an inverse lookup table so we can quickly 
    // find distances in tiles.
	for (int i = 0; i < table.size(); i++) {
		table[g.getTiles()[i]] = i;
	}
    inverseTable = table;
}

// priority_queue REQUIRES the operator() to compare two PuzzleMoves.
bool Heuristic::operator()(const PuzzleMove& s1, const PuzzleMove& s2){
	return s1.getFValue() > s2.getFValue();
}

// This function computes the heuristic for a given state.
int Heuristic::operator()(const PuzzleState& s1) const {

    vector<int> tiles = s1.getTiles();
    int sum = 0;

    int N = sqrt(tiles.size());

    for (int i = 0; i < tiles.size(); i++) {

        int tile = tiles[i];
        if (tile == 0) continue;

		// use the previously defined inverse table to find the index of the tile in the goal state.
        int goalIndex = inverseTable[tile];

        // Find the row by performing integer division
		//      if a tile is at index 5 in a 3x3, then it's in the second row, which is 5 / 3 = 1 using integer division
        int startRow = i / N;
        // Find the column by performing modulus
		//      if a tile is at index 5 in a 3x3, then its in the 3rd column, which is 5 % 3 = 2.
        int startCol = i % N;

        // Compute goalRow and goalCol using the same method.
        int goalRow = goalIndex / N;
        int goalCol = goalIndex % N;

        // the manhattan distance is the absolute value of the distance 
        // between the X coordinates and the Y coordinates. (RowDist + ColDist)
        sum += abs(startRow - goalRow) + abs(startCol - goalCol);
    }

    return sum;
}
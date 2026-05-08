// Implements the Manhattan distance heuristic.

#pragma once
#include "PuzzleState.h"
#include "PuzzleMove.h"

using namespace std;
class Heuristic {
public:
	
	Heuristic(PuzzleState g);
	Heuristic() {}

	bool operator()(const PuzzleMove& s1, const PuzzleMove& s2);
	int operator()(const PuzzleState& s1) const;
private:
	PuzzleState goal;
	vector<int> inverseTable; // An inverse lookup table which maps a tile to its index in the goal state.
							  // This allows us to compute the heuristic in O(N) time.
	int N; // The number of tiles in the puzzle.
};
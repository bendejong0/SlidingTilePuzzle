#pragma once
#include <vector>
#include "PuzzleMove.h"
using namespace std;
class Hash {
public:
	// This is all the same function but C++ demands different return types and signatures.
	size_t operator()(const PuzzleMove& p) const;
	size_t operator()(const PuzzleState& p) const;
	size_t operator()(PuzzleMove& p) const;
};
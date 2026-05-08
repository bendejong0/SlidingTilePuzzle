// Contains definitions for hashing functions.
// We can do hashes using a polynomial rolling hash.
// Here's a link that I learned from:
// https://www.geeksforgeeks.org/dsa/string-hashing-using-polynomial-rolling-hash-function/

#include "Hash.h"

// Hashes a PuzzleMove
// input: const PuzzleMove reference
// output: size_t because I'm assuming this will get huge.
size_t Hash::operator()(const PuzzleMove& p) const{
    vector<int> v = p.getState().getTiles();
    size_t result = 0;
    for (const auto& i : v) {
        result = result * 17 + i;  // You could pick any prime here. I just chose 17
                                   // I also tried other primes but I didn't see any difference.
    }
    return result;
}

// Hashes a PuzzleState
// input: const PuzzleState reference
// output: size_t because I'm assuming this will get huge.
size_t Hash::operator()(const PuzzleState& p) const {
    vector<int> v = p.getTiles();
    size_t result = 0;
    for (const auto& i : v) {
        result = result * 17 + i;
    }
    return result;
}

// Hashes a PuzzleState
// input: PuzzleState reference
// output: size_t because I'm assuming this will get huge.
size_t Hash::operator()(PuzzleMove& p) const {
    vector<int> v = p.getState().getTiles();
    size_t result = 0;
    for (const auto& i : v) {
        result = result * 17 + i;
    }
    return result;
}
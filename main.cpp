// Here's a list of changes which are weird or unexplained.
// Code also has documentation.
// 
// + Added unordered_set to store the closed list.
//  	unordered_set requires a hashing function, so I implimented that.
// 
// + Used a polynomial rolling hash for the hashing function.
// 
// + Added a cost, heuristic, and F variable to PuzzleMove to avoid recomputation.
// 
// + Utilized an inverse lookup table in the heuristic to compute much faster (O(N) vs O(N^2)).
// 

#include "PuzzleMove.h"
#include "Heuristic.h"
#include "PuzzleState.h"
#include <algorithm>
#include <queue>		 // for std::priority_queue
#include <utility>  	 // for std::pair
#include "Hash.h"
#include <unordered_set> // allows us to check if we've visited a state in O(1) time.

bool findSolution(const PuzzleState& start, const PuzzleState& goal);
void printSolution(unordered_set<PuzzleMove, Hash>& closed, PuzzleMove& goal);
void expandState(PuzzleMove& current, priority_queue<PuzzleMove, vector<PuzzleMove>, Heuristic>& open, unordered_set<PuzzleMove, Hash>& closed, Heuristic& h);
vector<PuzzleMove> buildSolutionList(unordered_set<PuzzleMove, Hash>& closed, PuzzleMove& goal);

int main() {

	int runCount{ 1 };
	do
	{
		cout << "****** SAMPLE RUN #" << runCount << " ********************************" << endl << endl;
		cout << "Enter number of rows and columns: ";
		size_t N;
		cin >> N;

		cout << "\nEnter Start State row by row:" << endl;
		PuzzleState start = PuzzleState(N, N);
		cin >> start;
		cout << "\nEnter Goal State row by row: " << endl;
		PuzzleState goal = PuzzleState(N, N);
		cin >> goal;
		cout << endl << endl;

		if (start == goal) {
			cout << "*** Solution found immediately: start state is the goal state. ***\n\n";
		}
		else {
			findSolution(start, goal);
		}
		runCount++;
	} while (true);
}

// takes in the start state and goal state.
bool findSolution(const PuzzleState& start, const PuzzleState& goal) {
	Heuristic h(goal);
	priority_queue<PuzzleMove, vector<PuzzleMove>, Heuristic> open(h);
	unordered_set<PuzzleMove, Hash> closed;
	open.push(PuzzleMove(start, PuzzleState::NullState, MoveType::nullMove));

	while (!open.empty()) {
		PuzzleMove current = open.top();
		open.pop();
		if (current.getState() == goal) {
			closed.insert(current);
			printSolution(closed, current);
			return true;
		}
		else {
			closed.insert(current);
			expandState(current, open, closed, h);
		}
	}
	// something really bad happened
	return false;
}

// takes in the current state, a priority queue, an unordered set, and a heuristic function which contains the goal state.
void expandState(PuzzleMove& current, priority_queue<PuzzleMove, vector<PuzzleMove>, Heuristic>& open, unordered_set<PuzzleMove, Hash>& closed, Heuristic& h) {
	// Down, Left, Up, Right
	PuzzleState& currentState = current.getState();
	if (currentState.canMoveDown()) {
		PuzzleState temp = currentState.moveBlankDown();
		// if it's not in the closed list,
		if (closed.find(PuzzleMove(temp, PuzzleState(), nullMove)) == closed.end()) {
			// add it to the open list
			open.push(PuzzleMove(temp, currentState, MoveType::down, current.getCost()+1, h(temp)));
		}
	}
	if (currentState.canMoveLeft()) {
		PuzzleState temp = currentState.moveBlankLeft();
		// if it's not in the closed list,
		if (closed.find(PuzzleMove(temp, PuzzleState(), nullMove)) == closed.end()) {
			// add it to the open list
			open.push(PuzzleMove(temp, currentState, MoveType::left, current.getCost() + 1, h(temp)));
		}
	}
	if (currentState.canMoveUp()) {
		PuzzleState temp = currentState.moveBlankUp();
		// if it's not in the closed list,
		if (closed.find(PuzzleMove(temp, PuzzleState(), nullMove)) == closed.end()) {
			// add it to the open list
			open.push(PuzzleMove(temp, currentState, MoveType::up, current.getCost() + 1, h(temp)));
		}
	}
	if (currentState.canMoveRight()) {
		PuzzleState temp = currentState.moveBlankRight();
		// if it's not in the closed list,
		if (closed.find(PuzzleMove(temp, PuzzleState(), nullMove)) == closed.end()) {
			// compute the heuristic value of the state
			// add it to the open list
			open.push(PuzzleMove(temp, currentState, MoveType::right, current.getCost() + 1, h(temp)));
		}
	}
}

// takes in an unordered_set and goal state
// takes in the goal state because otherwise we dont know where to start
// because unordered_set does not have a "first" or "last" element.
vector<PuzzleMove> buildSolutionList(unordered_set<PuzzleMove, Hash>& closed, PuzzleMove& goal) {
	vector<PuzzleMove> solutionList;

	PuzzleMove current = goal;
	
	// We have to do it like this because unordered_set does not have "first" or "last".
	// Additionally, it's unordered, as the name implies.
	// Changing this to an ordered set is possible but defeats the purpose 
	// of using an unordered set in the first place, which is to have O(1) lookups.
	
	// Find the previous node by hashing the parent
	while (current.getMoveName() != nullMove) {
		solutionList.push_back(current);

		PuzzleState parentState = current.getParent();

		// we must use PuzzleMove because of the type of the unordered_set.
		// changing the unordered_set type to store PuzzleState instead of PuzzleMove
		// is possible, but then we would not be able to store the parent move, 
		// which makes it impossible to reconstruct the path.

		// Dummy key, only state matters
		PuzzleMove key(parentState, PuzzleState(), nullMove);

		auto it = closed.find(key); // returns an iterator.
		if (it == closed.end()) break; // safety

		current = *it;
	}
	solutionList.push_back(current); // add the start state


	return solutionList;
}

// takes in an unordered_set, goal state
// needs the goal state because it has to pass it to buildSolutionList
void printSolution(unordered_set<PuzzleMove, Hash>& closed, PuzzleMove& goal) {
	vector<PuzzleMove> solution;

	solution = buildSolutionList(closed, goal);
	int pathLength = solution.size()-1;

	cout << "*** Solution Found ***\n\n";

	for (auto it = solution.rbegin(); it != solution.rend(); it++)
	{
		switch (it->getMoveName())
		{
		case MoveType::down:
			cout << "down\n\n";
			break;
		case MoveType::left:
			cout << "left\n\n";
			break;
		case MoveType::right:
			cout << "right\n\n";
			break;
		case MoveType::up:
			cout << "up\n\n";
			break;
		}

		it->getState().print(cout);
		cout << "\n";

	}

	cout << "\nPath length to Goal: " << pathLength;
	cout << "\n\n";
}
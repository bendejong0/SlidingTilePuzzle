"""
Sliding Tile Puzzle Generator

Usage:
    python sliding_tile_puzzle.py <n>

    n  -- number of tiles (must be a perfect square, e.g. 4, 9, 16)
         Tile labels run from 0 to n-1; 0 represents the blank tile.

Example:
    python sliding_tile_puzzle.py 9
"""

import sys
import math
import random


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def is_perfect_square(n: int) -> bool:
    root = int(math.isqrt(n))
    return root * root == n


def grid_side(n: int) -> int:
    return int(math.isqrt(n))


def format_grid(tiles: list[int], side: int) -> str:
    """Pretty-print a flat tile list as a square grid."""
    cell_width = len(str(max(tiles))) + 1
    rows = []
    for row in range(side):
        chunk = tiles[row * side:(row + 1) * side]
        rows.append(" ".join(f"{t:>{cell_width}}" for t in chunk))
    return "\n".join(rows)


# ---------------------------------------------------------------------------
# Solvability check (based on inversions + blank row from bottom)
# ---------------------------------------------------------------------------

def count_inversions(tiles: list[int]) -> int:
    """Count pairs (i, j) where i < j but tiles[i] > tiles[j], ignoring 0."""
    flat = [t for t in tiles if t != 0]
    inversions = 0
    for i in range(len(flat)):
        for j in range(i + 1, len(flat)):
            if flat[i] > flat[j]:
                inversions += 1
    return inversions


def is_solvable(tiles: list[int], side: int) -> bool:
    """
    A sliding-puzzle configuration is solvable iff:
      - odd grid width  : number of inversions is even
      - even grid width : (inversions + blank's row from bottom) is odd
    """
    inversions = count_inversions(tiles)
    if side % 2 == 1:
        return inversions % 2 == 0
    else:
        blank_row_from_bottom = side - (tiles.index(0) // side)
        return (inversions + blank_row_from_bottom) % 2 == 1


# ---------------------------------------------------------------------------
# Scrambler: random walk from solved state (always solvable)
# ---------------------------------------------------------------------------

def scramble(n: int, moves: int | None = None) -> list[int]:
    """
    Produce a scrambled puzzle by executing random valid moves from the
    solved state. This guarantees the result is always solvable.
    """
    side = grid_side(n)
    if moves is None:
        # Scale the walk length with puzzle size for a thorough shuffle
        moves = max(200, n * 50)

    state = list(range(n))          # solved state: [0, 1, 2, ..., n-1]
    blank = 0                       # 0 is the blank tile; starts at index 0

    directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]  # up, down, left, right

    for _ in range(moves):
        row, col = divmod(blank, side)
        neighbors = []
        for dr, dc in directions:
            nr, nc = row + dr, col + dc
            if 0 <= nr < side and 0 <= nc < side:
                neighbors.append(nr * side + nc)
        target = random.choice(neighbors)
        state[blank], state[target] = state[target], state[blank]
        blank = target

    return state


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    if len(sys.argv) != 2:
        print(f"Usage: python {sys.argv[0]} <n>")
        sys.exit(1)

    try:
        n = int(sys.argv[1])
    except ValueError:
        print(f"Error: '{sys.argv[1]}' is not a valid integer.")
        sys.exit(1)

    if n < 4:
        print("Error: n must be at least 4 (smallest meaningful puzzle is 2×2).")
        sys.exit(1)

    if not is_perfect_square(n):
        print(f"Error: n={n} is not a perfect square. "
              f"Try 4 (2×2), 9 (3×3), 16 (4×4), 25 (5×5), …")
        sys.exit(1)

    side = grid_side(n)
    solved = list(range(n))
    scrambled = scramble(n)

    print(f"Sliding Tile Puzzle  ({side}×{side}, tiles 0–{n-1},  0 = blank)\n")
    print("── Scrambled (Input) " + "─" * 20)
    print(format_grid(scrambled, side))
    print()
    print("── Finished  (Goal)  " + "─" * 20)
    print(format_grid(solved, side))


if __name__ == "__main__":
    main()
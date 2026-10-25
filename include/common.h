#ifndef COMMON_H
#define COMMON_H

#include <vector>

// Board dimensions
const int BOARD_SIZE = 8;

// Piece types on the checkers board
enum class Piece {
    EMPTY,
    RED,
    BLACK,
    RED_KING,
    BLACK_KING
};

// Player turn state
enum class Player {
    RED,
    BLACK
};

// Board coordinate representation
struct Position {
    int row;
    int col;

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

// A full turn: either a single step, or a (possibly chained) sequence of jumps.
struct Move {
    Position start;
    Position end;                // final landing square
    bool isJump;
    std::vector<Position> path;  // every square landed on after `start`, in order

    Move(Position s, Position e, bool jump = false)
        : start(s), end(e), isJump(jump), path{e} {}
};

#endif // COMMON_H

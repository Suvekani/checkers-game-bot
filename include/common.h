#ifndef COMMON_H
#define COMMON_H

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

// Movement structure representing a single move or jump
struct Move {
    Position start;
    Position end;
    bool isJump;

    Move(Position s, Position e, bool jump = false) 
        : start(s), end(e), isJump(jump) {}
};

#endif // COMMON_H
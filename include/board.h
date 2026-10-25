#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include <vector>

class Board {
private:
    Piece grid[BOARD_SIZE][BOARD_SIZE];

    // Recursively builds jump chains for one piece. `state` already has the
    // moving piece lifted off the board and previously captured pieces removed.
    void collectJumps(const Board& state, Position cur, Piece piece, Player player,
                    const Position& start, const std::vector<Position>& path,
                    std::vector<Move>& out) const;

public:
    Board();

    void initializeBoard();
    void clear();
    void setPiece(int row, int col, Piece piece);
    void displayBoard() const;
    Piece getPiece(int row, int col) const;
    bool isValidPosition(int row, int col) const;

    std::vector<Move> getValidMoves(Player player) const;
    bool makeMove(const Move& move, Player player);
};

#endif // BOARD_H

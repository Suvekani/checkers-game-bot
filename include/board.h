#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include <vector>

class Board {
private:
    Piece grid[BOARD_SIZE][BOARD_SIZE];

public:
    Board();
    
    void initializeBoard();
    void displayBoard() const;
    Piece getPiece(int row, int col) const;
    bool isValidPosition(int row, int col) const;
    
    std::vector<Move> getValidMoves(Player player) const;
    bool makeMove(const Move& move, Player player);
};

#endif // BOARD_H
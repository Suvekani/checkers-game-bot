#include "board.h"
#include <iostream>

Board::Board() {
    initializeBoard();
}

void Board::initializeBoard() {
    // Clear board
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            grid[r][c] = Piece::EMPTY;
        }
    }

    // Place Black pieces (rows 0 to 2)
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if ((r + c) % 2 == 1) {
                grid[r][c] = Piece::BLACK;
            }
        }
    }

    // Place Red pieces (rows 5 to 7)
    for (int r = 5; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if ((r + c) % 2 == 1) {
                grid[r][c] = Piece::RED;
            }
        }
    }
}

Piece Board::getPiece(int row, int col) const {
    if (isValidPosition(row, col)) {
        return grid[row][col];
    }
    return Piece::EMPTY;
}

bool Board::isValidPosition(int row, int col) const {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

void Board::displayBoard() const {
    std::cout << "\n  0 1 2 3 4 5 6 7\n";
    for (int r = 0; r < BOARD_SIZE; ++r) {
        std::cout << r << " ";
        for (int c = 0; c < BOARD_SIZE; ++c) {
            char symbol = '.';
            switch (grid[r][c]) {
                case Piece::RED:        symbol = 'r'; break;
                case Piece::BLACK:      symbol = 'b'; break;
                case Piece::RED_KING:   symbol = 'R'; break;
                case Piece::BLACK_KING: symbol = 'B'; break;
                default:                symbol = '.'; break;
            }
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
std::vector<Move> Board::getValidMoves(Player player) const {
    std::vector<Move> moves;
    std::vector<Move> jumps;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            Piece p = grid[r][c];

            // Filter pieces belonging to current player
            if ((player == Player::RED && (p == Piece::RED || p == Piece::RED_KING)) ||
                (player == Player::BLACK && (p == Piece::BLACK || p == Piece::BLACK_KING))) {

                // Determine direction based on color/king status
                std::vector<int> rowDirs;
                if (p == Piece::RED || p == Piece::RED_KING) rowDirs.push_back(-1);   // Moving up
                if (p == Piece::BLACK || p == Piece::BLACK_KING) rowDirs.push_back(1); // Moving down

                int colDirs[] = {-1, 1};

                for (int dr : rowDirs) {
                    for (int dc : colDirs) {
                        int nr = r + dr;
                        int nc = c + dc;

                        // Check simple single step
                        if (isValidPosition(nr, nc) && grid[nr][nc] == Piece::EMPTY) {
                            moves.push_back(Move({r, c}, {nr, nc}, false));
                        }

                        // Check capture jump over opponent
                        int jr = r + 2 * dr;
                        int jc = c + 2 * dc;
                        if (isValidPosition(jr, jc) && grid[jr][jc] == Piece::EMPTY) {
                            Piece midPiece = grid[nr][nc];
                            bool isOpponent = false;
                            if (player == Player::RED && (midPiece == Piece::BLACK || midPiece == Piece::BLACK_KING)) isOpponent = true;
                            if (player == Player::BLACK && (midPiece == Piece::RED || midPiece == Piece::RED_KING)) isOpponent = true;

                            if (isOpponent) {
                                jumps.push_back(Move({r, c}, {jr, jc}, true));
                            }
                        }
                    }
                }
            }
        }
    }

    // In checkers rules, jumps are mandatory if available
    return !jumps.empty() ? jumps : moves;
}

bool Board::makeMove(const Move& move, Player player) {
    Piece p = grid[move.start.row][move.start.col];

    // Basic move execution
    grid[move.start.row][move.start.col] = Piece::EMPTY;
    grid[move.end.row][move.end.col] = p;

    // Handle jump capture
    if (move.isJump) {
        int midRow = (move.start.row + move.end.row) / 2;
        int midCol = (move.start.col + move.end.col) / 2;
        grid[midRow][midCol] = Piece::EMPTY;
    }

    // King promotion
    if (player == Player::RED && move.end.row == 0) {
        grid[move.end.row][move.end.col] = Piece::RED_KING;
    } else if (player == Player::BLACK && move.end.row == BOARD_SIZE - 1) {
        grid[move.end.row][move.end.col] = Piece::BLACK_KING;
    }

    return true;
}
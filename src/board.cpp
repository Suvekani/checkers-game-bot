#include "board.h"
#include <iostream>

namespace {

bool isRed(Piece p)   { return p == Piece::RED || p == Piece::RED_KING; }
bool isBlack(Piece p) { return p == Piece::BLACK || p == Piece::BLACK_KING; }
bool isKing(Piece p)  { return p == Piece::RED_KING || p == Piece::BLACK_KING; }

bool belongsTo(Piece p, Player player) {
    return player == Player::RED ? isRed(p) : isBlack(p);
}

bool isOpponentOf(Piece p, Player player) {
    return player == Player::RED ? isBlack(p) : isRed(p);
}

// A man (non-king) reaching the far row becomes a king.
bool isPromotionRow(Piece p, int row) {
    if (p == Piece::RED)   return row == 0;
    if (p == Piece::BLACK) return row == BOARD_SIZE - 1;
    return false;
}

// Kings move in both row directions; men only move forward.
std::vector<int> rowDirections(Piece p) {
    if (isKing(p)) return {-1, 1};
    if (p == Piece::RED) return {-1};   // RED moves up the board
    return {1};                          // BLACK moves down the board
}

} // namespace

Board::Board() {
    initializeBoard();
}

void Board::initializeBoard() {
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
            if (!belongsTo(p, player)) continue;

            // Simple steps
            for (int dr : rowDirections(p)) {
                for (int dc : {-1, 1}) {
                    int nr = r + dr;
                    int nc = c + dc;
                    if (isValidPosition(nr, nc) && grid[nr][nc] == Piece::EMPTY) {
                        moves.push_back(Move({r, c}, {nr, nc}, false));
                    }
                }
            }

            // This Move representation stores a single start/end pair, so each
            // capture is represented as one jump.
            for (int dr : rowDirections(p)) {
                for (int dc : {-1, 1}) {
                    int mr = r + dr;
                    int mc = c + dc;
                    int lr = r + 2 * dr;
                    int lc = c + 2 * dc;
                    if (isValidPosition(lr, lc) &&
                        isOpponentOf(grid[mr][mc], player) &&
                        grid[lr][lc] == Piece::EMPTY) {
                        jumps.push_back(Move({r, c}, {lr, lc}, true));
                    }
                }
            }
        }
    }

    // Jumps are mandatory when available.
    return !jumps.empty() ? jumps : moves;
}

bool Board::makeMove(const Move& move, Player player) {
    if (!isValidPosition(move.start.row, move.start.col) ||
        !isValidPosition(move.end.row, move.end.col)) {
        return false;
    }

    Piece p = grid[move.start.row][move.start.col];
    if (!belongsTo(p, player)) return false;
    if (grid[move.end.row][move.end.col] != Piece::EMPTY) return false;

    if (move.isJump) {
        int rowDiff = move.end.row - move.start.row;
        int colDiff = move.end.col - move.start.col;
        if (std::abs(rowDiff) != 2 || std::abs(colDiff) != 2) return false;
        int capturedRow = (move.start.row + move.end.row) / 2;
        int capturedCol = (move.start.col + move.end.col) / 2;
        if (!isOpponentOf(grid[capturedRow][capturedCol], player)) return false;
    } else if (std::abs(move.end.row - move.start.row) != 1 ||
               std::abs(move.end.col - move.start.col) != 1 ||
               (p != Piece::RED_KING && p != Piece::BLACK_KING &&
                ((p == Piece::RED && move.end.row >= move.start.row) ||
                 (p == Piece::BLACK && move.end.row <= move.start.row)))) {
        return false;
    }

    grid[move.start.row][move.start.col] = Piece::EMPTY;

    // Remove the piece jumped over.
    if (move.isJump) {
        grid[(move.start.row + move.end.row) / 2]
            [(move.start.col + move.end.col) / 2] = Piece::EMPTY;
    }

    grid[move.end.row][move.end.col] = p;

    // King promotion (men only)
    if (isPromotionRow(p, move.end.row)) {
        grid[move.end.row][move.end.col] = (p == Piece::RED) ? Piece::RED_KING
                                                              : Piece::BLACK_KING;
    }

    return true;
}

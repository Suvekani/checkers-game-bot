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

void Board::clear() {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            grid[r][c] = Piece::EMPTY;
        }
    }
}

void Board::setPiece(int row, int col, Piece piece) {
    if (isValidPosition(row, col)) {
        grid[row][col] = piece;
    }
}

void Board::initializeBoard() {
    clear();

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

void Board::collectJumps(const Board& state, Position cur, Piece piece, Player player,
                         const Position& start, const std::vector<Position>& path,
                         std::vector<Move>& out) const {
    bool foundJump = false;

    for (int dr : rowDirections(piece)) {
        for (int dc : {-1, 1}) {
            int mr = cur.row + dr,     mc = cur.col + dc;      // square being jumped
            int lr = cur.row + 2 * dr, lc = cur.col + 2 * dc;  // landing square

            if (!state.isValidPosition(lr, lc)) continue;
            if (!isOpponentOf(state.grid[mr][mc], player)) continue;
            if (state.grid[lr][lc] != Piece::EMPTY) continue;

            foundJump = true;

            Board next = state;
            next.grid[mr][mc] = Piece::EMPTY;   // captured pieces can't be jumped twice

            std::vector<Position> nextPath = path;
            nextPath.push_back({lr, lc});

            if (isPromotionRow(piece, lr)) {
                // Becoming a king ends the turn.
                Move m(start, {lr, lc}, true);
                m.path = nextPath;
                out.push_back(m);
            } else {
                collectJumps(next, {lr, lc}, piece, player, start, nextPath, out);
            }
        }
    }

    // No further jump from here: the chain (if any) ends on this square.
    if (!foundJump && !path.empty()) {
        Move m(start, cur, true);
        m.path = path;
        out.push_back(m);
    }
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

            // Jumps (including multi-jump chains). Lift the piece off its start
            // square so a king can legally loop back through it.
            Board state = *this;
            state.grid[r][c] = Piece::EMPTY;
            collectJumps(state, {r, c}, p, player, {r, c}, {}, jumps);
        }
    }

    // Jumps are mandatory when available.
    return !jumps.empty() ? jumps : moves;
}

bool Board::makeMove(const Move& move, Player player) {
    Piece p = grid[move.start.row][move.start.col];
    if (!belongsTo(p, player)) return false;

    grid[move.start.row][move.start.col] = Piece::EMPTY;

    // Remove every piece jumped over along the path.
    if (move.isJump) {
        Position cur = move.start;
        for (const Position& next : move.path) {
            grid[(cur.row + next.row) / 2][(cur.col + next.col) / 2] = Piece::EMPTY;
            cur = next;
        }
    }

    grid[move.end.row][move.end.col] = p;

    // King promotion (men only)
    if (isPromotionRow(p, move.end.row)) {
        grid[move.end.row][move.end.col] = (p == Piece::RED) ? Piece::RED_KING
                                                              : Piece::BLACK_KING;
    }

    return true;
}

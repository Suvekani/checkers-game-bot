#include "board.h"
#include "bot.h"
#include <cassert>
#include <iostream>

static int passed = 0;
#define CHECK(cond) do { assert(cond); ++passed; } while (0)

static void testInitialMoves() {
    Board b;
    CHECK(b.getValidMoves(Player::RED).size() == 7);
    CHECK(b.getValidMoves(Player::BLACK).size() == 7);
}

static void testMandatoryJump() {
    Board b; b.clear();
    b.setPiece(4, 3, Piece::RED);
    b.setPiece(3, 2, Piece::BLACK);
    b.setPiece(7, 0, Piece::RED);          // has a plain step available, must be ignored
    auto moves = b.getValidMoves(Player::RED);
    CHECK(moves.size() == 1);
    CHECK(moves[0].isJump);
    CHECK(moves[0].end == (Position{2, 1}));
}

static void testMultiJump() {
    Board b; b.clear();
    b.setPiece(6, 1, Piece::RED);
    b.setPiece(5, 2, Piece::BLACK);
    b.setPiece(3, 4, Piece::BLACK);
    auto moves = b.getValidMoves(Player::RED);
    CHECK(moves.size() == 1);
    CHECK(moves[0].path.size() == 2);
    CHECK(moves[0].end == (Position{2, 5}));

    b.makeMove(moves[0], Player::RED);
    CHECK(b.getPiece(5, 2) == Piece::EMPTY);
    CHECK(b.getPiece(3, 4) == Piece::EMPTY);
    CHECK(b.getPiece(6, 1) == Piece::EMPTY);
    CHECK(b.getPiece(2, 5) == Piece::RED);
}

static void testKingMovesBackward() {
    Board man; man.clear();
    man.setPiece(4, 3, Piece::RED);
    CHECK(man.getValidMoves(Player::RED).size() == 2);

    Board king; king.clear();
    king.setPiece(4, 3, Piece::RED_KING);
    CHECK(king.getValidMoves(Player::RED).size() == 4);

    Board bk; bk.clear();
    bk.setPiece(4, 3, Piece::BLACK_KING);
    CHECK(bk.getValidMoves(Player::BLACK).size() == 4);
}

static void testKingJumpsBackward() {
    Board b; b.clear();
    b.setPiece(2, 3, Piece::RED_KING);
    b.setPiece(3, 4, Piece::BLACK);        // behind the king (downward)
    auto moves = b.getValidMoves(Player::RED);
    CHECK(moves.size() == 1);
    CHECK(moves[0].isJump);
    CHECK(moves[0].end == (Position{4, 5}));
}

static void testPromotionEndsChain() {
    Board b; b.clear();
    b.setPiece(2, 1, Piece::RED);
    b.setPiece(1, 2, Piece::BLACK);
    b.setPiece(1, 4, Piece::BLACK);        // a king could continue, a new king may not
    auto moves = b.getValidMoves(Player::RED);
    CHECK(moves.size() == 1);
    CHECK(moves[0].path.size() == 1);
    b.makeMove(moves[0], Player::RED);
    CHECK(b.getPiece(0, 3) == Piece::RED_KING);
    CHECK(b.getPiece(1, 4) == Piece::BLACK);
}

// Regression test: the BLACK bot used to maximize RED's score and walk into captures.
static void testBotAvoidsBlunders() {
    Board b; b.clear();
    b.setPiece(4, 3, Piece::RED);
    b.setPiece(2, 3, Piece::BLACK);
    b.setPiece(2, 5, Piece::BLACK);
    MinimaxBot bot(2);
    Move m = bot.getBestMove(b, Player::BLACK);

    bool blunder1 = m.start == (Position{2, 3}) && m.end == (Position{3, 2});
    bool blunder2 = m.start == (Position{2, 5}) && m.end == (Position{3, 4});
    CHECK(!blunder1);
    CHECK(!blunder2);
}

int main() {
    testInitialMoves();
    testMandatoryJump();
    testMultiJump();
    testKingMovesBackward();
    testKingJumpsBackward();
    testPromotionEndsChain();
    testBotAvoidsBlunders();
    std::cout << "All tests passed (" << passed << " checks).\n";
    return 0;
}

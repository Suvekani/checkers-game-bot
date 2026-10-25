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

static void testBotReturnsLegalMove() {
    Board b;
    const auto legalMoves = b.getValidMoves(Player::BLACK);
    MinimaxBot bot(2);
    Move m = bot.getBestMove(b, Player::BLACK);

    bool isLegal = false;
    for (const auto& move : legalMoves) {
        if (move.start == m.start && move.end == m.end) {
            isLegal = true;
            break;
        }
    }
    CHECK(isLegal);
}

int main() {
    testInitialMoves();
    testBotReturnsLegalMove();
    std::cout << "All tests passed (" << passed << " checks).\n";
    return 0;
}

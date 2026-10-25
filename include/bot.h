#ifndef BOT_H
#define BOT_H

#include "board.h"
#include <limits>
#include <algorithm>
#include <stdexcept>

class MinimaxBot {
private:
    int maxDepth;

    // Score from `aiPlayer`'s point of view: higher is always better for the AI.
    int evaluateBoard(const Board& board, Player aiPlayer) const {
        int score = 0;
        for (int r = 0; r < BOARD_SIZE; ++r) {
            for (int c = 0; c < BOARD_SIZE; ++c) {
                Piece p = board.getPiece(r, c);
                if (p == Piece::EMPTY) continue;

                bool isKing = (p == Piece::RED_KING || p == Piece::BLACK_KING);
                bool isRed  = (p == Piece::RED || p == Piece::RED_KING);
                int value = isKing ? 15 : 10;

                bool belongsToAI = (isRed == (aiPlayer == Player::RED));
                score += belongsToAI ? value : -value;
            }
        }
        return score;
    }

public:
    MinimaxBot(int depth = 4) : maxDepth(depth) {}

    int minimax(Board board, int depth, bool isMaximizing, int alpha, int beta, Player aiPlayer) {
        if (depth == 0) {
            return evaluateBoard(board, aiPlayer);
        }

        Player opponent = (aiPlayer == Player::RED ? Player::BLACK : Player::RED);
        Player currentTurn = isMaximizing ? aiPlayer : opponent;
        std::vector<Move> moves = board.getValidMoves(currentTurn);

        // No moves = the side to move loses. Adding `depth` prefers faster wins
        // and slower losses.
        if (moves.empty()) {
            return isMaximizing ? -1000 - depth : 1000 + depth;
        }

        if (isMaximizing) {
            int maxEval = std::numeric_limits<int>::min();
            for (const auto& move : moves) {
                Board tempBoard = board;
                tempBoard.makeMove(move, currentTurn);
                int eval = minimax(tempBoard, depth - 1, false, alpha, beta, aiPlayer);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) break; // Alpha-beta cutoff
            }
            return maxEval;
        } else {
            int minEval = std::numeric_limits<int>::max();
            for (const auto& move : moves) {
                Board tempBoard = board;
                tempBoard.makeMove(move, currentTurn);
                int eval = minimax(tempBoard, depth - 1, true, alpha, beta, aiPlayer);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) break; // Alpha-beta cutoff
            }
            return minEval;
        }
    }

    // Precondition: aiPlayer has at least one legal move.
    Move getBestMove(Board board, Player aiPlayer) {
        std::vector<Move> moves = board.getValidMoves(aiPlayer);
        if (moves.empty()) {
            throw std::logic_error("getBestMove called with no legal moves");
        }

        Move bestMove = moves[0];
        int bestValue = std::numeric_limits<int>::min();

        for (const auto& move : moves) {
            Board tempBoard = board;
            tempBoard.makeMove(move, aiPlayer);
            int moveValue = minimax(tempBoard, maxDepth - 1, false,
                                    std::numeric_limits<int>::min(),
                                    std::numeric_limits<int>::max(),
                                    aiPlayer);

            if (moveValue > bestValue) {
                bestValue = moveValue;
                bestMove = move;
            }
        }
        return bestMove;
    }
};

#endif // BOT_H

#ifndef BOT_H
#define BOT_H

#include "board.h"
#include <limits>
#include <algorithm>

class MinimaxBot {
private:
    int maxDepth;

    // Evaluates board score: higher is better for RED, lower for BLACK
    int evaluateBoard(const Board& board) const {
        int score = 0;
        for (int r = 0; r < BOARD_SIZE; ++r) {
            for (int c = 0; c < BOARD_SIZE; ++c) {
                Piece p = board.getPiece(r, c);
                if (p == Piece::RED) score += 10;
                else if (p == Piece::RED_KING) score += 15;
                else if (p == Piece::BLACK) score -= 10;
                else if (p == Piece::BLACK_KING) score -= 15;
            }
        }
        return score;
    }

public:
    MinimaxBot(int depth = 4) : maxDepth(depth) {}

    int minimax(Board board, int depth, bool isMaximizing, int alpha, int beta, Player aiPlayer) {
        if (depth == 0) {
            return evaluateBoard(board);
        }

        Player currentTurn = isMaximizing ? aiPlayer : (aiPlayer == Player::RED ? Player::BLACK : Player::RED);
        std::vector<Move> moves = board.getValidMoves(currentTurn);

        if (moves.empty()) {
            return isMaximizing ? -1000 : 1000;
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

    Move getBestMove(Board board, Player aiPlayer) {
        std::vector<Move> moves = board.getValidMoves(aiPlayer);
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
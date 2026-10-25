#include "board.h"
#include "stack.h"
#include "queue.h"
#include <iostream>
#include <utility>
#include "bot.h"

struct MoveRecord {
    Board previousState;
    Move move;
    bool hadCapture = false;
    decltype(std::declval<Move&>().start) capturedPos{};
    Piece capturedPiece = Piece::EMPTY;
};

MoveRecord makeMoveRecord(const Board& previousState, const Move& move) {
    MoveRecord record{previousState, move};
    record.hadCapture = move.isJump;

    if (move.isJump) {
        record.capturedPos = {
            (move.start.row + move.end.row) / 2,
            (move.start.col + move.end.col) / 2
        };
        record.capturedPiece = previousState.getPiece(record.capturedPos.row, record.capturedPos.col);
    }

    return record;
}

int main() {
    Board board;
    Player currentTurn = Player::RED;

    // Stack to track move history for Undo feature
    Stack<MoveRecord> history;
    
    // Queue to hold suggested move evaluation sequence
    Queue<Move> moveQueue;

    bool gameRunning = true;

    // Initialize AI Bot (search depth 4)
    MinimaxBot bot(4);

    while (gameRunning) {
        board.displayBoard();

        std::vector<Move> validMoves = board.getValidMoves(currentTurn);

        if (validMoves.empty()) {
            std::cout << "Game Over! " 
                    << (currentTurn == Player::RED ? "BLACK (AI)" : "RED (Player)") 
                    << " wins!\n";
            break;
        }

        // --- PLAYER TURN (RED) ---
        if (currentTurn == Player::RED) {
            std::cout << "\n[Your Turn - RED]\n";
            std::cout << "Available moves:\n";
            for (size_t i = 0; i < validMoves.size(); ++i) {
                std::cout << "  " << i + 1 << ". From (" << validMoves[i].start.row << "," << validMoves[i].start.col << ")"
                        << " to (" << validMoves[i].end.row << "," << validMoves[i].end.col << ")"
                        << (validMoves[i].isJump ? " [CAPTURE]" : "") << "\n";
            }

            std::cout << "\nOptions:\n";
            std::cout << "  - Enter move number (1-" << validMoves.size() << ")\n";
            std::cout << "  - Enter '99' to Undo last move\n";
            std::cout << "  - Enter '0' to Exit\n";
            std::cout << "Your choice: ";

            int choice;
            std::cin >> choice;

            if (choice == 0) {
                gameRunning = false;
                std::cout << "Exiting game...\n";
                break;
            }

            // Undo logic
            if (choice == 99) {
                if (history.isEmpty()) {
                    std::cout << "\n[!] No moves to undo!\n";
                } else {
                    for (int i = 0; i < 2 && !history.isEmpty(); ++i) {
                        MoveRecord lastRecord = history.top();
                        history.pop();
                        board = lastRecord.previousState;
                    }
                    currentTurn = Player::RED;
                    std::cout << "\n[✓] Undid last move sequence!\n";
                }
                continue;
            }

            if (choice < 1 || choice > static_cast<int>(validMoves.size())) {
                std::cout << "\n[!] Invalid selection! Try again.\n";
                continue;
            }

            Move selectedMove = validMoves[choice - 1];

            MoveRecord rec = makeMoveRecord(board, selectedMove);

            board.makeMove(selectedMove, currentTurn);
            history.push(rec);

            currentTurn = Player::BLACK; // Hand turn to AI
        } 
        // --- AI TURN (BLACK) ---
        else {
            std::cout << "\n[AI Turn - BLACK Thinking...]\n";
            Move aiMove = bot.getBestMove(board, Player::BLACK);

            MoveRecord rec = makeMoveRecord(board, aiMove);

            board.makeMove(aiMove, Player::BLACK);
            history.push(rec);

            std::cout << "AI played from (" << aiMove.start.row << "," << aiMove.start.col 
                    << ") to (" << aiMove.end.row << "," << aiMove.end.col << ")\n\n";

            currentTurn = Player::RED; // Hand turn back to Player
        }
    }
}
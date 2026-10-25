#include "board.h"
#include "stack.h"
#include "queue.h"
#include "bot.h"
#include <iostream>
#include <limits>

// One entry per move, so Undo can roll the board back.
struct MoveRecord {
    Board previousState;
    Player mover;
};

// Prints a move as (r,c) -> (r,c) [-> (r,c) ...]. The hop squares go through
// a Queue so multi-jumps are printed in the order they are played.
void printMove(const Move& move) {
    Queue<Position> hops;
    for (const Position& p : move.path) {
        hops.enqueue(p);
    }

    std::cout << "(" << move.start.row << "," << move.start.col << ")";
    while (!hops.isEmpty()) {
        Position p = hops.front();
        hops.dequeue();
        std::cout << " -> (" << p.row << "," << p.col << ")";
    }
}

int main() {
    Board board;
    Player currentTurn = Player::RED;

    // Stack to track move history for Undo feature
    Stack<MoveRecord> history;

    // Initialize AI Bot (search depth 4)
    MinimaxBot bot(4);

    while (true) {
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
                std::cout << "  " << i + 1 << ". From ";
                printMove(validMoves[i]);
                std::cout << (validMoves[i].isJump ? " [CAPTURE]" : "") << "\n";
            }

            std::cout << "\nOptions:\n";
            std::cout << "  - Enter move number (1-" << validMoves.size() << ")\n";
            std::cout << "  - Enter '99' to Undo last move\n";
            std::cout << "  - Enter '0' to Exit\n";
            std::cout << "Your choice: ";

            int choice;
            if (!(std::cin >> choice)) {
                if (std::cin.eof()) {
                    std::cout << "\nInput closed. Exiting game...\n";
                    break;
                }
                // Non-numeric input: clear the error and discard the bad line.
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\n[!] Please enter a number.\n";
                continue;
            }

            if (choice == 0) {
                std::cout << "Exiting game...\n";
                break;
            }

            // Undo: roll back to just before the player's last move
            // (this also takes back the AI's reply).
            if (choice == 99) {
                if (history.isEmpty()) {
                    std::cout << "\n[!] No moves to undo!\n";
                } else {
                    while (!history.isEmpty()) {
                        MoveRecord last = history.top();
                        history.pop();
                        board = last.previousState;
                        if (last.mover == Player::RED) break;
                    }
                    currentTurn = Player::RED;
                    std::cout << "\n[OK] Undid last move sequence!\n";
                }
                continue;
            }

            if (choice < 1 || choice > static_cast<int>(validMoves.size())) {
                std::cout << "\n[!] Invalid selection! Try again.\n";
                continue;
            }

            const Move& selectedMove = validMoves[choice - 1];

            history.push(MoveRecord{board, Player::RED});
            board.makeMove(selectedMove, Player::RED);

            currentTurn = Player::BLACK; // Hand turn to AI
        }
        // --- AI TURN (BLACK) ---
        else {
            std::cout << "\n[AI Turn - BLACK Thinking...]\n";
            Move aiMove = bot.getBestMove(board, Player::BLACK);

            history.push(MoveRecord{board, Player::BLACK});
            board.makeMove(aiMove, Player::BLACK);

            std::cout << "AI played ";
            printMove(aiMove);
            std::cout << "\n\n";

            currentTurn = Player::RED; // Hand turn back to Player
        }
    }

    return 0;
}

# Checkers Game Bot

A console checkers game (C++17) where you play RED against a minimax AI playing BLACK.

## Build and run

```bash
make          # builds ./checkers  (checkers.exe on Windows/MinGW)
./checkers
make test     # builds and runs the unit tests
make clean
```

Requires a C++17 compiler (g++ or clang++) and `make`. Without make:

```bash
g++ -std=c++17 -Iinclude src/main.cpp src/board.cpp -o checkers
```

## How to play

- The board is printed with row/column numbers. `r` / `R` = your RED man / king, `b` / `B` = the AI's BLACK man / king.
- Each turn the game lists your legal moves as numbered options, e.g. `(5,0) -> (4,1)`. A multi-jump is shown as `(6,1) -> (4,3) -> (2,5)`. Type the number of the move you want.
- `99` undoes your last move (and the AI's reply), `0` exits.

## Rules implemented

- Men move diagonally forward; kings move diagonally in both directions.
- Captures are mandatory.
- Multi-jumps: a piece keeps jumping while another capture is available.
- A man that reaches the far row becomes a king, and that ends its turn.
- You lose when you have no legal moves (no pieces, or all blocked).

## Project layout

```
include/common.h   Piece, Player, Position, Move
include/board.h    Board class
include/bot.h      MinimaxBot (minimax + alpha-beta pruning, depth 4)
include/stack.h    Linked-list stack (undo history)
include/queue.h    Linked-list queue (prints multi-jump paths in order)
src/board.cpp      Move generation, move execution, display
src/main.cpp       Game loop and UI
tests/test_board.cpp  Unit tests for moves, jumps, promotion and the bot
```

## Known limitations / ideas

- No draw detection (move limit or repetition), so a king-vs-king endgame can go on forever.
- Evaluation is material only (man = 10, king = 15). Adding advancement, center control and back-row defense would make the AI stronger.
- No move ordering (trying captures first would speed up alpha-beta pruning).
- Search depth and player side are fixed.

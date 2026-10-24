#include "../include/board.h"
#include <iostream>

int main() {
    Board board;
    std::cout << "Initial Checkers Board Setup:\n";
    board.displayBoard();
    return 0;
}
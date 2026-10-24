#include "../include/board.h"
#include <iostream>

Board::Board() {
    initializeBoard();
}

void Board::initializeBoard() {
    // Clear board
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
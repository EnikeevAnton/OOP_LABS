// Scanner.cpp
#include "../include/Scanner.h"
#include "../include/GameBoard.h"

bool Scanner::use() {
    const int directions[4][2] = {
        {0, 0},
        {1, 0},
        {0, -1},
        {1, -1}, 
    };

    for (size_t i = 0; i < 4; ++i) {
        int cellX = coordinates.first + directions[i][0];
        int cellY = coordinates.second + directions[i][1];
        if (board.isShipSegment(cellX, cellY)) {
            return true;
        }
    }
    return false;
}


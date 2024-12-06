// Scanner.h
#ifndef SCANNER_H
#define SCANNER_H

#include "Ability.h"

class Scanner : public Ability {
public:
    Scanner(GameBoard& board, int x, int y) 
        : board(board), coordinates(std::make_pair(x, y)) 
    {}
    bool use() override;
private:
    GameBoard& board;
    std::pair <int, int> coordinates;
};

#endif // SCANNER_H
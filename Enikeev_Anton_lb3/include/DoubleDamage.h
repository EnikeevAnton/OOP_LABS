// DoubleDamage.h
#ifndef DOUBLEDAMAGE_H
#define DOUBLEDAMAGE_H

#include "Ability.h"

class DoubleDamage : public Ability {
public:
    DoubleDamage(GameBoard& board) : board(board) {}
    bool use() override;
private:
    GameBoard& board;
};

#endif // DOUBLEDAMAGE_H
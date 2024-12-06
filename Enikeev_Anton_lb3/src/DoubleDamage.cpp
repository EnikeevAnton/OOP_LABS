// DoubleDamage.cpp
#include "../include/DoubleDamage.h"
#include "../include/GameBoard.h"

bool DoubleDamage::use() {
    board.activateDoubleDamage();
    return true;
}
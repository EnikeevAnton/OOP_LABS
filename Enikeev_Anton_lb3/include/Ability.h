// Ability.h
#ifndef ABILITY_H
#define ABILITY_H

#include "Exceptions.h"
#include "RandomNumberGenerator.h"

class GameBoard;
class ShipManager;

enum class AbilityType {
    Scanner,
    DoubleDamage,
    Bombardment,
};

class Ability {
public:
    virtual bool use() = 0;
    virtual ~Ability() = default;
};

#endif // ABILITY_H
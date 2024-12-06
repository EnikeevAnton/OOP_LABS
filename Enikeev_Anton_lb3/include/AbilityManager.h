// AbilityManager.h
#ifndef ABILITYMANAGER_H
#define ABILITYMANAGER_H

#include <queue>
#include <memory>
#include <unordered_map>
#include "Ability.h"

class AbilityManager {
public:
    AbilityManager();
    AbilityManager(std::queue<AbilityType> abilities);
    bool useAbility(GameBoard& board, ShipManager& shipManager, int x = 0, int y = 0);
    void addRandomAbility();
    AbilityType showAvailableAbility() const;
    bool canUseAbility() const;
    std::queue<AbilityType> getAbilities() const;
private:
    std::queue<AbilityType> abilities;
};

#endif // ABILITYMANAGER_H
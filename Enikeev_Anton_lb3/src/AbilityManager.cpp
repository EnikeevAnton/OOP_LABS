// AbilityManager.cpp
#include "../include/AbilityManager.h"
#include "../include/DoubleDamage.h"
#include "../include/Scanner.h"
#include "../include/Bombardment.h"

AbilityManager::AbilityManager() {
    addRandomAbility();
}

AbilityManager::AbilityManager(std::queue<AbilityType> abilities){
    this->abilities = abilities;
}

bool AbilityManager::useAbility(GameBoard& board, ShipManager& shipManager, int x, int y) {
    if (abilities.empty()) {
        throw NoAbilitiesException();
    }

    std::unique_ptr<Ability> currentAbility;
    AbilityType ability = abilities.front();

    if (ability == AbilityType::DoubleDamage) {
        currentAbility = std::make_unique<DoubleDamage>(board);
    } 
    else if (ability == AbilityType::Bombardment) {
        currentAbility = std::make_unique<Bombardment>(shipManager);
    } 
    else {
        currentAbility = std::make_unique<Scanner>(board, x, y);
    }

    bool success = currentAbility->use();
    abilities.pop();
    return success;
}

void AbilityManager::addRandomAbility() {
    RandomNumberGenerator rng;

    switch (rng.getRandomNumber(2)) {
        case 0:
            abilities.push(AbilityType::DoubleDamage);
            break;
        case 1:
            abilities.push(AbilityType::Scanner);
            break;
        case 2:
            abilities.push(AbilityType::Bombardment);
            break;
    }
}

AbilityType AbilityManager::showAvailableAbility() const {
    if (abilities.empty()) {
        throw NoAbilitiesException();
    }
    return abilities.front();
}

bool AbilityManager::canUseAbility() const {
    return !abilities.empty();
}

std::queue<AbilityType> AbilityManager::getAbilities() const{
    return abilities;
}
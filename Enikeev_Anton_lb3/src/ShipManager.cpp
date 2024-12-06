#include "../include/ShipManager.h"

ShipManager::ShipManager(const std::vector<int>& shipSizes) {
    for (size_t i = 0; i < shipSizes.size(); ++i) {
        int size = shipSizes[i];
        ships.emplace_back(size, i);
    }
}

const std::vector<Ship>& ShipManager::getShips() const {
    return ships;
}

Ship& ShipManager::getShip(int shipIndex) {
    if (shipIndex < 0 || shipIndex >= static_cast<int>(ships.size())) {
        std::cerr << "Invalid ship index" << std::endl;
        return ships[0];
    }
    return ships[shipIndex];
}

void ShipManager::updateShipStatus(int shipIndex, int segmentIndex) {
    if (shipIndex < 0 || shipIndex >= static_cast<int>(ships.size())) {
        std::cerr << "Invalid ship index" << std::endl;
        return;
    }
    ships[shipIndex].hit(segmentIndex);
}

std::vector<int> ShipManager::getShipSizes(){
    std::vector<int> shipSizes;
    for (size_t i = 0; i < ships.size(); ++i) {
        int length = ships[i].getLength();
        shipSizes.emplace_back(length);
    }
    return shipSizes;
}


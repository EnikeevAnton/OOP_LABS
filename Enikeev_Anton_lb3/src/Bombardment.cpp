// Bombardment.cpp
#include "../include/Bombardment.h"
#include "../include/GameBoard.h"

bool Bombardment::use() {

    const auto& ships = shipManager.getShips();
    std::vector<int> availableShips;

    for (size_t i = 0; i < ships.size(); ++i) {
        if (!ships[i].isDestroyed()) {
            availableShips.push_back(i);
        }
    }

    if (!availableShips.empty()) {
        RandomNumberGenerator rng;
        int randomShipIndex = rng.getRandomNumber(availableShips.size() - 1);
        Ship& ship = shipManager.getShip(randomShipIndex);
        std::vector<int> availableSegments;

        for (int i = 0; i < ship.getLength(); ++i) {
            if (ship.getSegmentStatus(i) != SegmentStatus::Destroyed) {
                availableSegments.push_back(i);
            }
        }

        int randomSegmentIndex = rng.getRandomNumber(availableSegments.size() - 1);
        ship.hit(randomSegmentIndex);
        return true;
    }
    return false;
}

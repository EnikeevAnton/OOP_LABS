#ifndef SHIPMANAGER_H
#define SHIPMANAGER_H

#include <tuple>
#include "Ship.h"

class ShipManager {
public:
    ShipManager(const std::vector<int>& shipSizes);
    const std::vector<Ship>& getShips() const;
    Ship& getShip(int shipIndex);
    void updateShipStatus(int shipIndex, int segmentIndex);
    std::vector<int> getShipSizes();
private:
    std::vector<Ship> ships;
};

#endif // SHIPMANAGER_H
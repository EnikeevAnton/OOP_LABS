// Bombardment.h
#ifndef BOMBARDMENT_H
#define BOMBARDMENT_H

#include "Ability.h"

class Bombardment : public Ability {
public:
    Bombardment(ShipManager& shipManager) : shipManager(shipManager) {}
    bool use() override;
private:
    ShipManager& shipManager;
};

#endif // BOMBARDMENT_H
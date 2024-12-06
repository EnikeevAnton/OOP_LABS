#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <iostream>

enum class SegmentStatus {
    Intact,
    Damaged,
    Destroyed
};

enum class Orientation {
    Horizontal,
    Vertical
};

class Ship {
public:
    Ship(int length,int id);
    void hit(int segmentIndex);
    SegmentStatus getSegmentStatus(int segmentIndex) const;
    bool isDestroyed() const;
    int getLength() const;
    int getID() const;
    void setSegmentStatus(int index, SegmentStatus status);
    void printShip() const;
private:
    int length;
    int shipID;
    std::vector<SegmentStatus> segments;
};

#endif // SHIP_H
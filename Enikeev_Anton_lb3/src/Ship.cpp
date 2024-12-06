#include "../include/Ship.h"

Ship::Ship(int length, int id){
    this->length = length;
    this->shipID = id;
    segments.resize(length, SegmentStatus::Intact);
}

void Ship::hit(int segmentIndex) {
    if (segmentIndex < 0 || segmentIndex >= length) {
        std::cerr << "Invalid segment index" << std::endl;
        return;
    }
    if (segments[segmentIndex] == SegmentStatus::Intact) {
        segments[segmentIndex] = SegmentStatus::Damaged;
    } else if (segments[segmentIndex] == SegmentStatus::Damaged) {
        segments[segmentIndex] = SegmentStatus::Destroyed;
    }
}

SegmentStatus Ship::getSegmentStatus(int segmentIndex) const {
    if (segmentIndex < 0 || segmentIndex >= length) {
        std::cerr << "Invalid segment index" << std::endl;
        return SegmentStatus::Intact;
    }
    return segments[segmentIndex];
}

bool Ship::isDestroyed() const{
    for (const auto& status : segments){
        if (status != SegmentStatus::Destroyed) {return false;}
    }
    return true;
}
int Ship::getLength() const {
    return length;
}

int Ship::getID()const{
    return shipID;
}

void Ship::setSegmentStatus(int index, SegmentStatus status){
    segments[index] = status;
}

void Ship::printShip() const{
    std::cout << "[length="<< getLength() << " ID=" << getID() << " ";
    for (int i = 0; i < getLength(); ++i){
        SegmentStatus status = getSegmentStatus(i);
        switch (status)
        {
        case SegmentStatus::Intact:
            std::cout << "Intact ";
            break;
        case SegmentStatus::Damaged:
            std::cout << "Damaged ";
            break;
        case SegmentStatus::Destroyed:
            std::cout << "Destroyed ";
            break;
        default:
            break;
        }
    }
    std::cout << "]"<< std::endl;
}
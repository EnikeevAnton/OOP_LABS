// Exceptions.h
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class NoAbilitiesException : public std::exception {
public:
    const char* what() const noexcept override {
        return "No abilities available to use.";
    }
};

class InvalidShipPlacementException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid ship placement: ships are too close or overlapping.";
    }
};

class AttackOutOfBoundsException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Attack coordinates are out of bounds.";
    }
};


#endif // EXCEPTIONS_H
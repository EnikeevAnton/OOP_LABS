#include "../include/RandomNumberGenerator.h"
#include <stdexcept>

RandomNumberGenerator::RandomNumberGenerator() : rng(std::random_device{}()) {}

int RandomNumberGenerator::getRandomNumber(int maxValue) {
    if (maxValue < 0) {
        throw std::invalid_argument("maxIndex must be non-negative");
    }
    std::uniform_int_distribution<int> dist(0, maxValue);
    return dist(rng);
}

bool RandomNumberGenerator::getBool() {
    std::uniform_int_distribution<int> dist(0, 1);
    return dist(rng) == 1;
}

double RandomNumberGenerator::getDouble(double minValue, double maxValue) {
    std::uniform_real_distribution<double> dist(minValue, maxValue);
    return dist(rng);
}

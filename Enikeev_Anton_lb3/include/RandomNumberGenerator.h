#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include <random>

class RandomNumberGenerator {
public:
    RandomNumberGenerator();

    // Возвращает случайное число от 0 до maxValue (включительно)
    int getRandomNumber(int maxValue);
    
    // Возвращает случайное булево значение (true/false)
    bool getBool();

    // Возвращает случайное число с плавающей точкой от minValue до maxValue
    double getDouble(double minValue, double maxValue);

private:
    std::mt19937 rng; // Генератор случайных чисел
};

#endif // RANDOM_NUMBER_GENERATOR_H

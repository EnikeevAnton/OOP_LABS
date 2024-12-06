#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <optional>
#include <iomanip>
#include "ShipManager.h"
#include "AbilityManager.h"

class AbilityManager;

enum class CellStatus {
    Unknown,
    Empty,
    Ship
};

struct Cell {
    CellStatus status;
    std::optional<std::pair<Ship*, int>> shipSegment;

    Cell() : status(CellStatus::Unknown), shipSegment(std::nullopt) {}
};

class GameBoard {
public:
    GameBoard(int width, int height);
    GameBoard(const GameBoard& other);
    GameBoard(const GameBoard& other, ShipManager& shipManager);
    GameBoard(GameBoard&& other) noexcept;
    GameBoard& operator=(const GameBoard& other);
    GameBoard& operator=(GameBoard&& other) noexcept;
    void placeShip(ShipManager& shipManager, int shipIndex, int x, int y, Orientation orientation);
    bool attackCell(int x, int y);
    bool isShipSegment(int x, int y);
    void printBoard() const;
    void activateDoubleDamage();
    int getWidth() const;
    int getHeight() const;
    Cell& getCell(int x, int y);
private:
    int width;
    int height;
    bool isDoubleDamageUsed;
    std::vector<std::vector<Cell>> board;

    bool areNeighborsValid(int nx, int ny) const;
    bool canPlaceShip(const Ship& ship, int x, int y, Orientation orientation) const;
};

#endif // GAMEBOARD_H
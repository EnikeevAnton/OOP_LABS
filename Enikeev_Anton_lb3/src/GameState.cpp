#include "../include/GameState.h"

GameState::GameState(const GameBoard& playerBoard, const GameBoard& computerBoard, 
                     const ShipManager& playerShipManager, const ShipManager& computerShipManager, 
                     const AbilityManager& playerAbilityManager, int roundNumber, bool playerTurn)
    : playerBoard(playerBoard), computerBoard(computerBoard),
      playerShipManager(playerShipManager), computerShipManager(computerShipManager),
      playerAbilityManager(playerAbilityManager), roundNumber(roundNumber), playerTurn(playerTurn) {}

GameState::GameState()
    : roundNumber(0), playerTurn(true), 
      playerBoard(1, 1), computerBoard(1, 1), 
      playerShipManager({}), computerShipManager({}), 
      playerAbilityManager() {}

void GameState::saveToFile(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Could not open file for saving game state.");
    }

    outFile << *this;
}

void GameState::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        throw std::runtime_error("Could not open file for loading game state.");
    }

    inFile >> *this;
}

void GameState::saveToStream(std::ostream& out) {
    safeWrite(out, reinterpret_cast<const char*>(&roundNumber), sizeof(roundNumber));
    safeWrite(out, reinterpret_cast<const char*>(&playerTurn), sizeof(playerTurn));

    saveShipManager(out, playerShipManager);
    saveShipManager(out, computerShipManager);

    saveGameBoard(out, playerBoard);
    saveGameBoard(out, computerBoard);

    saveAbilityManager(out, playerAbilityManager);
}

void GameState::loadFromStream(std::istream& in) {
    safeRead(in, reinterpret_cast<char*>(&roundNumber), sizeof(roundNumber));
    safeRead(in, reinterpret_cast<char*>(&playerTurn), sizeof(playerTurn));

    playerShipManager = loadShipManager(in);
    computerShipManager = loadShipManager(in);

    loadUserGameBoard(in, playerShipManager);
    loadCompGameBoard(in, computerShipManager);

    playerAbilityManager = loadAbilityManager(in);
}


void GameState::saveGameBoard(std::ostream& out, GameBoard& board) {
    int width = board.getWidth();
    int height = board.getHeight();

    safeWrite(out, reinterpret_cast<const char*>(&width), sizeof(width));
    safeWrite(out, reinterpret_cast<const char*>(&height), sizeof(height));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Cell& cell = board.getCell(x, y);
            CellStatus status = cell.status;
            safeWrite(out, reinterpret_cast<const char*>(&status), sizeof(status));

            if (status == CellStatus::Ship) {
                int shipID = cell.shipSegment->first->getID();
                int segmentIndex = cell.shipSegment->second;
                safeWrite(out, reinterpret_cast<const char*>(&shipID), sizeof(shipID));
                safeWrite(out, reinterpret_cast<const char*>(&segmentIndex), sizeof(segmentIndex));
            }
        }
    }
}


void GameState::loadUserGameBoard(std::istream& in, ShipManager& manager) {
    int width, height;
    safeRead(in, reinterpret_cast<char*>(&width), sizeof(width));
    safeRead(in, reinterpret_cast<char*>(&height), sizeof(height));

    GameBoard board(width, height);
    playerBoard = board;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Cell& cell = playerBoard.getCell(x, y);
            CellStatus status;
            safeRead(in, reinterpret_cast<char*>(&status), sizeof(status));
            cell.status = status;

            if (status == CellStatus::Ship) {
                int shipID, segmentIndex;
                safeRead(in, reinterpret_cast<char*>(&shipID), sizeof(shipID));
                safeRead(in, reinterpret_cast<char*>(&segmentIndex), sizeof(segmentIndex));
                Ship& ship = manager.getShip(shipID);
                cell.shipSegment = {&ship, segmentIndex};
            }
        }
    }
}

void GameState::loadCompGameBoard(std::istream& in, ShipManager& manager) {
    int width, height;
    safeRead(in, reinterpret_cast<char*>(&width), sizeof(width));
    safeRead(in, reinterpret_cast<char*>(&height), sizeof(height));


    GameBoard board(width, height);
    computerBoard = board;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Cell& cell = computerBoard.getCell(x, y);
            CellStatus status;
            safeRead(in, reinterpret_cast<char*>(&status), sizeof(status));
            cell.status = status;

            if (status == CellStatus::Ship) {
                int shipID, segmentIndex;
                safeRead(in, reinterpret_cast<char*>(&shipID), sizeof(shipID));
                safeRead(in, reinterpret_cast<char*>(&segmentIndex), sizeof(segmentIndex));
                Ship& ship = manager.getShip(shipID);
                cell.shipSegment = {&ship, segmentIndex};
            }
        }
    }
}

void GameState::saveShipManager(std::ostream& out, const ShipManager& manager) {
    const auto& ships = manager.getShips();
    size_t shipCount = ships.size();
    safeWrite(out, reinterpret_cast<const char*>(&shipCount), sizeof(shipCount));

    for (const auto& ship : ships) {
        int size = ship.getLength();
        int shipID = ship.getID();
        safeWrite(out, reinterpret_cast<const char*>(&size), sizeof(size));
        safeWrite(out, reinterpret_cast<const char*>(&shipID), sizeof(shipID));

        for (int i = 0; i < size; ++i) {
            SegmentStatus status = ship.getSegmentStatus(i);
            safeWrite(out, reinterpret_cast<const char*>(&status), sizeof(status));
        }
    }
}

ShipManager GameState::loadShipManager(std::istream& in) {
    size_t shipCount;
    safeRead(in, reinterpret_cast<char*>(&shipCount), sizeof(shipCount));

    std::vector<int> shipSizes(shipCount);
    std::vector<Ship> ships;

    for (size_t i = 0; i < shipCount; ++i) {
        int size, shipID;
        safeRead(in, reinterpret_cast<char*>(&size), sizeof(size));
        safeRead(in, reinterpret_cast<char*>(&shipID), sizeof(shipID));

        Ship ship(size, shipID);
        for (int j = 0; j < size; ++j) {
            SegmentStatus status;
            safeRead(in, reinterpret_cast<char*>(&status), sizeof(status));
            ship.setSegmentStatus(j, status);
        }
        ships.push_back(std::move(ship));
    }

    ShipManager manager(shipSizes);
    for (size_t i = 0; i < ships.size(); ++i) {
        manager.getShip(i) = std::move(ships[i]);
    }

    return manager;
}

void GameState::saveAbilityManager(std::ostream& out, const AbilityManager& manager) {
    std::queue<AbilityType> abilities = manager.getAbilities();
    size_t abilityCount = abilities.size();
    safeWrite(out, reinterpret_cast<const char*>(&abilityCount), sizeof(abilityCount));

    while (!abilities.empty()) {
        AbilityType ability = abilities.front();
        safeWrite(out, reinterpret_cast<const char*>(&ability), sizeof(ability));
        abilities.pop();
    }
}

AbilityManager GameState::loadAbilityManager(std::istream& in) {
    size_t abilityCount;
    safeRead(in, reinterpret_cast<char*>(&abilityCount), sizeof(abilityCount));

    std::queue<AbilityType> abilities;
    for (size_t i = 0; i < abilityCount; ++i) {
        AbilityType ability;
        safeRead(in, reinterpret_cast<char*>(&ability), sizeof(ability));
        abilities.push(ability);
    }

    return AbilityManager(abilities);
}

GameBoard& GameState::getPlayerBoard() { return playerBoard; }
GameBoard& GameState::getComputerBoard() { return computerBoard; }
ShipManager& GameState::getPlayerShipManager() { return playerShipManager; }
ShipManager& GameState::getComputerShipManager() { return computerShipManager; }
AbilityManager& GameState::getPlayerAbilityManager() { return playerAbilityManager; }
int GameState::getRoundNumber() const { return roundNumber; }
bool GameState::isPlayerTurn() const { return playerTurn; }

void GameState::safeRead(std::istream& stream, char* buffer, size_t size) {
    stream.read(buffer, size);
    if (!stream) throw std::runtime_error("Error reading from stream or data is corrupted.");
}

void GameState::safeWrite(std::ostream& stream, const char* buffer, size_t size) {
    stream.write(buffer, size);
    if (!stream) throw std::runtime_error("Error writing to stream.");
}

std::ostream& operator<<(std::ostream& out, GameState& state) {
    state.saveToStream(out); // Используем универсальный метод
    return out;
}

std::istream& operator>>(std::istream& in, GameState& state) {
    state.loadFromStream(in); // Используем универсальный метод
    return in;
}
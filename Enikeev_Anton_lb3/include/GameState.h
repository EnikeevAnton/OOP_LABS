#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <fstream>
#include "GameBoard.h"
#include "ShipManager.h"
#include "AbilityManager.h"

class GameState {
public:
    GameState();
    GameState(const GameBoard& playerBoard, const GameBoard& computerBoard, 
              const ShipManager& playerShipManager, const ShipManager& computerShipManager, 
              const AbilityManager& playerAbilityManager, int roundNumber, bool playerTurn);

    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);

    GameBoard& getPlayerBoard();
    GameBoard& getComputerBoard();
    ShipManager& getPlayerShipManager();
    ShipManager& getComputerShipManager();
    AbilityManager& getPlayerAbilityManager();
    int getRoundNumber() const;
    bool isPlayerTurn() const;


    void saveToStream(std::ostream& out);
    void loadFromStream(std::istream& in);

    void saveGameBoard(std::ostream& out, GameBoard& board);
    void loadUserGameBoard(std::istream& in, ShipManager& manager);
    void loadCompGameBoard(std::istream& in, ShipManager& manager);

    void saveShipManager(std::ostream& out, const ShipManager& manager);
    ShipManager loadShipManager(std::istream& in);

    void saveAbilityManager(std::ostream& out, const AbilityManager& manager);
    AbilityManager loadAbilityManager(std::istream& in);

    void safeRead(std::istream& stream, char* buffer, size_t size);
    void safeWrite(std::ostream& stream, const char* buffer, size_t size);

    friend std::ostream& operator<<(std::ostream& out, GameState& state);
    friend std::istream& operator>>(std::istream& in, GameState& state);

    private:
    GameBoard playerBoard;
    GameBoard computerBoard;
    ShipManager playerShipManager;
    ShipManager computerShipManager;
    AbilityManager playerAbilityManager;
    int roundNumber;
    bool playerTurn;
};

#endif // GAME_STATE_H

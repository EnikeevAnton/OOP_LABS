// Game.h
#ifndef GAME_H
#define GAME_H

#include <memory>
#include <string>
#include "GameBoard.h"
#include "ShipManager.h"
#include "AbilityManager.h"
#include "RandomNumberGenerator.h"
#include "GameState.h"

enum class CommandType {
    UseAbilityAndAttack,
    Attack
};

struct Command {
    CommandType type; // Тип команды
    int x = -1;       // Координата X (для атаки)
    int y = -1;       // Координата Y (для атаки)
    int abilityX = -1; // Индекс способности (для применения)
    int abilityY = -1;
};

struct ShipPlacement {
    int shipID;             // Идентификатор корабля
    int x;                  // Координата X размещения
    int y;                  // Координата Y размещения
    Orientation orientation; // Ориентация корабля (горизонтальная или вертикальная)

    ShipPlacement(int id, int xCoord, int yCoord, Orientation orient)
        : shipID(id), x(xCoord), y(yCoord), orientation(orient) {}
};

class Game {
public:
    Game(int boardWidth, int boardHeight, const std::vector<int>& shipSizes);
    void resetGame(const std::vector<ShipPlacement>& userPlacements, const std::vector<ShipPlacement>& compPlacements);
    bool playRound(const Command& command);
    bool isComputerLost();
    bool isPlayerLost();
    bool hasAbility() const;
    AbilityType getAvailableAbility() const;
    int getRoundNumber();
    void placeShips(const std::vector<ShipPlacement>& placements, bool isPlayerBoard);
    void saveGame(const std::string& filename) const;
    void loadGame(const std::string& filename, GameState& state);
    GameBoard& getPlayerBoard();
    GameBoard& getComputerBoard();
    void computerBoardRecovery(const std::vector<ShipPlacement>& placements);

private:
    std::unique_ptr<GameBoard> playerBoard;
    std::unique_ptr<GameBoard> computerBoard;
    std::unique_ptr<ShipManager> playerShipManager;
    std::unique_ptr<ShipManager> computerShipManager;
    AbilityManager playerAbilityManager;
    RandomNumberGenerator rng;

    bool playerTurnFlag;
    int roundNumber;
};

#endif

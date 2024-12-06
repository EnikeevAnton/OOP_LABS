#include "../include/Game.h"
#include <iostream>
#include <stdexcept>

// Конструктор игры
Game::Game(int boardWidth, int boardHeight, const std::vector<int>& shipSizes)
    : playerBoard(std::make_unique<GameBoard>(boardWidth, boardHeight)),
      computerBoard(std::make_unique<GameBoard>(boardWidth, boardHeight)),
      playerShipManager(std::make_unique<ShipManager>(shipSizes)),
      computerShipManager(std::make_unique<ShipManager>(shipSizes)),
      playerAbilityManager(),
      rng(),
      playerTurnFlag(true),
      roundNumber(1) {}


bool Game::playRound(const Command& command) {
    bool ability_result = false;

    if (playerTurnFlag == false){
        playerBoard->attackCell(command.x, command.y);
        playerTurnFlag = true; // Передаем ход игроку

    }else{

        if (command.type ==  CommandType::UseAbilityAndAttack){
            ability_result = playerAbilityManager.useAbility(*computerBoard, *computerShipManager, command.abilityX, command.abilityY);
        }
        int destroyed = computerBoard->attackCell(command.x, command.y);

        if (destroyed){
            playerAbilityManager.addRandomAbility();
        }
        
        playerTurnFlag = false; // Передаем ход компьютеру

    }
    ++roundNumber;
    return ability_result;
}

/*
Метод восстанавливает игровое поле компьюетра (и менеджер), требует новой расстановки кораблей
Используется в случае проигрыша компьюетра в раунде для восстановления 
*/
void Game::computerBoardRecovery(const std::vector<ShipPlacement>& placements){
    const auto& shipsComp = computerShipManager->getShips();
    std::vector<int> shipSizesComp;

    for (size_t i = 0; i < shipsComp.size(); ++i) {
        shipSizesComp.push_back(shipsComp[i].getLength());
    }
    // Создаётся новый менеджер компьютера
    computerShipManager = std::make_unique<ShipManager>(shipSizesComp);
    // Создается новое поле
    computerBoard = std::make_unique<GameBoard>(computerBoard->getWidth(), computerBoard->getHeight());
    // Расставляютя корабли
    placeShips(placements, false);
}

void Game::placeShips(const std::vector<ShipPlacement>& placements, bool isPlayerBoard) {
    GameBoard& board = isPlayerBoard ? *playerBoard : *computerBoard;
    ShipManager& manager = isPlayerBoard ? *playerShipManager : *computerShipManager;

    for (const auto& placement : placements) {
        board.placeShip(manager, placement.shipID, placement.x, placement.y, placement.orientation);
    }
}

bool Game::isComputerLost(){
    const auto& ships = computerShipManager->getShips();

    for (size_t i = 0; i < ships.size(); ++i) {
        if (!ships[i].isDestroyed()) {

            return false;
        }
    }

    return true;
}

bool Game::isPlayerLost(){
    const auto& ships = playerShipManager->getShips();

    for (size_t i = 0; i < ships.size(); ++i) {
        if (!ships[i].isDestroyed()) {
            return false;
        }
    }
    return true;
}

int Game::getRoundNumber(){
    return roundNumber;
}

bool Game::hasAbility() const {
    return playerAbilityManager.canUseAbility();
}

AbilityType Game::getAvailableAbility() const {
    return playerAbilityManager.showAvailableAbility();
}

// Сохранение состояния игры
void Game::saveGame(const std::string& filename) const {
    GameState state(*playerBoard, *computerBoard, *playerShipManager, *computerShipManager,
                    playerAbilityManager, roundNumber, playerTurnFlag);
    state.saveToFile(filename);
    std::cout << "Игра сохранена в файл: " << filename << std::endl; // потом уберем 
}

// Загрузка состояния игры
void Game::loadGame(const std::string& filename, GameState& state) {
    state.loadFromFile(filename);
    playerShipManager = std::make_unique<ShipManager>(state.getPlayerShipManager());
    computerShipManager = std::make_unique<ShipManager>(state.getComputerShipManager());
    playerBoard = std::make_unique<GameBoard>(state.getPlayerBoard(), *playerShipManager);
    computerBoard = std::make_unique<GameBoard>(state.getComputerBoard(), *computerShipManager);
    playerAbilityManager = state.getPlayerAbilityManager();
    roundNumber = state.getRoundNumber();
    playerTurnFlag = state.isPlayerTurn();
    std::cout << "Игра загружена из файла: " << filename << std::endl; // потом уберем 
}

/* 
    Можно сбросить состоние поля и менеджера игровка и компьютера,
    требуется новая расстановка кораблей 
*/
void Game::resetGame(const std::vector<ShipPlacement>& userPlacements, const std::vector<ShipPlacement>& compPlacements){
    // Очищаем текущие доски
    playerBoard = std::make_unique<GameBoard>(playerBoard->getWidth(), playerBoard->getHeight());
    computerBoard = std::make_unique<GameBoard>(computerBoard->getWidth(), computerBoard->getHeight());

    // Востанавливаем менджер кораблей пользователя
    const auto& shipsPlayer = playerShipManager->getShips();
    std::vector<int> shipSizes;

    for (size_t i = 0; i < shipsPlayer.size(); ++i) {
        shipSizes.push_back(shipsPlayer[i].getLength());
    }

    // Создаётся новый менеджер пользователя
    playerShipManager = std::make_unique<ShipManager>(shipSizes);
    // Новая расстанвока кораблей пользователя
    placeShips(userPlacements, true);

    // Восстанавливаем менеджер компьютера 
    const auto& shipsComp = computerShipManager->getShips();
    std::vector<int> shipSizesComp;

    for (size_t i = 0; i < shipsComp.size(); ++i) {
        shipSizesComp.push_back(shipsComp[i].getLength());
    }
    // Создаётся новый менеджер компьютера
    computerShipManager = std::make_unique<ShipManager>(shipSizesComp);
    // Новая расстанвока кораблей компьютера
    placeShips(compPlacements, false);

    // Сбрасываем параметры игры
    playerAbilityManager = AbilityManager();
    playerTurnFlag = true;
    roundNumber = 1;
}

GameBoard& Game::getPlayerBoard() {
    return *playerBoard;
}

GameBoard& Game::getComputerBoard() {
    return *computerBoard;
}







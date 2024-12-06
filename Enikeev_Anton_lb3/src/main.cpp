#include <iostream>
#include <fstream>
#include <cassert>
#include "../include/GameState.h"
#include "../include/Game.h"
#include "../include/GameBoard.h"
#include "../include/ShipManager.h"
#include "../include/AbilityManager.h"

void testGameStateSaveLoad() {
    int boardWidth = 10;
    int boardHeight = 10;
    std::vector<int> shipSizes = {3, 2, 1}; // Размеры кораблей

    // Создаем игру
    Game game(boardWidth, boardHeight, shipSizes);

    // Веторы расположения кораблей
    std::vector<ShipPlacement> placementsPlayer = {
        {0, 0, 0, Orientation::Horizontal},
        {1, 2, 3, Orientation::Vertical},
        {2, 4, 1, Orientation::Horizontal}
    };

    std::vector<ShipPlacement> placementsComp = {
        {0, 1, 1, Orientation::Horizontal},
        {1, 6, 6, Orientation::Vertical},
        {2, 9, 9, Orientation::Horizontal}
    };

    game.placeShips(placementsPlayer, true); // Размещение кораблей на доске игрока
    game.placeShips(placementsComp, false); // Размещение кораблей на доске компа

    GameBoard& compBoard = game.getComputerBoard();
    std::cout << "Игровое поле компьюетра после расстановки" << std::endl;
    compBoard.printBoard(); 

    // Выполняем "комманды"
    game.playRound(Command{CommandType::Attack, 2, 1}); // Игрок атакует 1
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 2
    std::cout << "Игровое поле компьюетра после 1 атаки пользователем (2, 1)" << std::endl;
    compBoard.printBoard();

    // Сохраняем игру
    const std::string saveFile = "game_save_test.dat";
    game.saveGame(saveFile);
    std::cout << "Выполняем несколько атак по полю компьютера" << std::endl;
    game.playRound(Command{CommandType::Attack, 9, 9}); // Игрок атакует 3
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 4
    game.playRound(Command{CommandType::Attack, 9, 9}); // Игрок атакует 5
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 6
    compBoard.printBoard();

    // Загружаем игру
    GameState state;
    Game gameLoaded(state.getPlayerBoard().getWidth(), state.getPlayerBoard().getHeight(), state.getPlayerShipManager().getShipSizes());
    gameLoaded.loadGame(saveFile, state);
    GameBoard& compLoadedBoard = gameLoaded.getComputerBoard();
    std::cout << "Игровое поле компьюетра после восстановления" << std::endl;
    compLoadedBoard.printBoard();

    // Атака для проверки
    gameLoaded.playRound(Command{CommandType::Attack, 2, 3}); // Игрок атакует 3
    gameLoaded.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 4
    std::cout << "Игровое поле компьюетра после 1 атаки пользователем (2, 3)" << std::endl;
    compLoadedBoard.printBoard();

    // Проверяем восстановление состояний
    if (gameLoaded.getRoundNumber() == 5){
        std::cout << "сохранение раунда: OK" << std::endl;
    }
    if (gameLoaded.isPlayerLost() == false){
        std::cout << "Проверка окончания пользователем игры: OK" << std::endl;
    }
    if(gameLoaded.isComputerLost() == false){
        std::cout << "Проверка окончания игры комп.: ОК" << std::endl;
    }
}

void testEndGame(){
    int boardWidth = 5;
    int boardHeight = 5;
    std::vector<int> shipSizes = {2, 2}; // Размеры кораблей

    // Создаем игру
    Game game(boardWidth, boardHeight, shipSizes);

    std::vector<ShipPlacement> placementsPlayer = {
        {0, 0, 0, Orientation::Horizontal},
        {1, 4, 3, Orientation::Vertical},
    };

    std::vector<ShipPlacement> placementsComp = {
        {0, 1, 1, Orientation::Horizontal},
        {1, 0, 3, Orientation::Vertical},
    };

    game.placeShips(placementsPlayer, true); // Размещение кораблей на доске игрока
    game.placeShips(placementsComp, false); // Размещение кораблей на доске компа

    GameBoard& compBoard = game.getComputerBoard();
    std::cout << "Игровое поле компьюетра после расстановки" << std::endl;
    compBoard.printBoard(); 

    // Устанавливаем состояние игры
    std::cout << "Уничтожаем все корабли компьютера" << std::endl;
    // Игровой цикл 
    game.playRound(Command{CommandType::Attack, 1, 1}); // Игрок атакует 1
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 2
    game.playRound(Command{CommandType::Attack, 1, 1}); // Игрок атакует 1
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 2
    game.playRound(Command{CommandType::Attack, 2, 1}); // Игрок атакует 1
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 2
    game.playRound(Command{CommandType::Attack, 2, 1}); // Игрок атакует 1
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 2
    game.playRound(Command{CommandType::Attack, 0, 3}); // Игрок атакует 1
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 2
    game.playRound(Command{CommandType::Attack, 0, 3}); // Игрок атакует 1
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 2
    game.playRound(Command{CommandType::Attack, 0, 4}); // Игрок атакует 1
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 2
    game.playRound(Command{CommandType::Attack, 0, 4}); // Игрок атакует 1
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 2

    if (game.isComputerLost() == true){
        std::cout << "Игровое поле компьюетра" << std::endl;
        compBoard.printBoard();
        std::cout << "Проверка проигрыша комп.: OK" << std::endl;
    }else{
        std::cout << "Игровое поле компьюетра" << std::endl;
        compBoard.printBoard();
        std::cout << "Проверка проигрыша комп.: wrong" << std::endl;
    }
    std::cout << "Пользователь выиграл в раунде, восстановим поле компьютера (поле пользователя сохраняется)" << std::endl;
    game.computerBoardRecovery(placementsComp);
    std::cout << "Игровое поле компьюетра после восстановления\n(При управлении игрой можно изменить расположение кораблей)" << std::endl;
    GameBoard& newCompBoard = game.getComputerBoard();
    newCompBoard.printBoard();
}

void testResetGame(){
    int boardWidth = 5;
    int boardHeight = 5;
    std::vector<int> shipSizes = {2, 2}; // Размеры кораблей

    // Создаем игру
    Game game(boardWidth, boardHeight, shipSizes);

    std::vector<ShipPlacement> placementsPlayer = {
        {0, 0, 0, Orientation::Horizontal},
        {1, 4, 3, Orientation::Vertical},
    };

    std::vector<ShipPlacement> placementsComp = {
        {0, 1, 1, Orientation::Horizontal},
        {1, 0, 3, Orientation::Vertical},
    };

    game.placeShips(placementsPlayer, true); // Размещение кораблей на доске игрока
    game.placeShips(placementsComp, false); // Размещение кораблей на доске компа

    GameBoard& userBoard = game.getPlayerBoard();
    std::cout << "Игровое поле игрока после расстановки" << std::endl;
    userBoard.printBoard(); 

    // Устанавливаем состояние игры
    std::cout << "Компьютер выполняет несколько атак" << std::endl;
    game.playRound(Command{CommandType::Attack, 1, 1}); // Игрок атакует 1
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 2
    game.playRound(Command{CommandType::Attack, 1, 1}); // Игрок атакует 1
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 2
    game.playRound(Command{CommandType::Attack, 2, 1}); // Игрок атакует 1
    game.playRound(Command{CommandType::Attack, 0, 0}); // Компьютер атакует 2
    userBoard.printBoard();
    std::cout << "Сбросим игру (расстановку кораблей не меняем, \nно её монжо изменить при управлении игрой )" << std::endl;
    game.resetGame(placementsPlayer, placementsComp);
    std::cout << "Поле сброшено" << std::endl;
    GameBoard& playerBoard = game.getPlayerBoard();
    playerBoard.printBoard();
}

void loadingGameNewProgram(){
    const std::string defaultSaveFile = "game_save_test.dat";

    std::cout << "Будет использован файл: " << defaultSaveFile << std::endl;
    // Загружаем игру
    GameState state;
    Game gameLoaded(state.getPlayerBoard().getWidth(), state.getPlayerBoard().getHeight(), state.getPlayerShipManager().getShipSizes());
    gameLoaded.loadGame(defaultSaveFile, state);
    GameBoard& compLoadedBoard = gameLoaded.getComputerBoard();
    std::cout << "Игровое поле компьюетра" << std::endl;
    compLoadedBoard.printBoard();
    GameBoard& userLoadedBoard = gameLoaded.getPlayerBoard();
    std::cout << "Игровое поле игрока" << std::endl;
    userLoadedBoard.printBoard();
}

int main() {
    int choice;

    std::cout << "Выберите тест для запуска:\n";
    std::cout << "1. Test GameState Save/Load\n Тестируем сохранение и загрузку\n";
    std::cout << "2. Test End Game\n Тестируем окончание игры\n";
    std::cout << "3. Test Reset Game\n Тестирует восстановление игры\n";
    std::cout << "4. loadingGameNewProgram\n Тестируеv загрузку игры (в директории игра должна быть уже сохранена)\n Прежде воспользуйтесь тестом 1\n";
    std::cout << "Введите номер теста: ";
    std::cin >> choice;
    try {
        switch(choice) {
            case 1:
                testGameStateSaveLoad();
                break;
            case 2:
                testEndGame();
                break;
            case 3:
                testResetGame();
                break;
            case 4:
                loadingGameNewProgram();
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
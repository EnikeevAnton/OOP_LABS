#include "../include/GameBoard.h"


GameBoard::GameBoard(int width, int height){
    if (width <= 0 || height <= 0) {
        std::cerr << "Invalid board dimensions" << std::endl;
        return;
    }
    this->width = width;
    this->height = height;
    this->isDoubleDamageUsed = false;
    board.resize(width, std::vector<Cell>(height));
}

GameBoard::GameBoard(const GameBoard& other)
    : width(other.width), height(other.height), board(other.board) {}

GameBoard::GameBoard(const GameBoard& other, ShipManager& shipManager)
    : width(other.width), height(other.height), board(other.board) {
    // Перенастроить указатели на корабли
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Cell& cell = board[y][x];
            if (cell.shipSegment.has_value()) {
                int shipID = cell.shipSegment->first->getID();
                int segmentIndex = cell.shipSegment->second;

                // Перенастраиваем ссылку на корабль
                Ship& ship = shipManager.getShip(shipID);
                cell.shipSegment = std::make_pair(&ship, segmentIndex);
            }
        }
    }
}


GameBoard::GameBoard(GameBoard&& other) noexcept
    : width(other.width), height(other.height), board(std::move(other.board)) {
    other.width = 0;
    other.height = 0;
}

GameBoard& GameBoard::operator=(const GameBoard& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        board = other.board;
    }
    return *this;
}

GameBoard& GameBoard::operator=(GameBoard&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        board = std::move(other.board);
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

void GameBoard::placeShip(ShipManager& shipManager, int shipIndex, int x, int y, Orientation orientation) {
    Ship& ship = shipManager.getShip(shipIndex);

    if (!canPlaceShip(ship, x, y, orientation)) {
        throw InvalidShipPlacementException();
    }

    int dx = (orientation == Orientation::Horizontal) ? 1 : 0;
    int dy = (orientation == Orientation::Vertical) ? 1 : 0;

    for (int i = 0; i < ship.getLength(); ++i) {
        board[x + i * dx][y + i * dy].status = CellStatus::Ship;
        board[x + i * dx][y + i * dy].shipSegment = {&ship, i};
    }
}

int GameBoard::getWidth() const{
    return width;
}

int GameBoard::getHeight() const{
    return height;
}

bool GameBoard::attackCell(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw AttackOutOfBoundsException();
    }

    Cell& cell = board[x][y];

    if (cell.status == CellStatus::Ship && cell.shipSegment.has_value()) {
        auto [ship, segmentIndex] = cell.shipSegment.value();
        if (ship->getSegmentStatus(segmentIndex) != SegmentStatus::Destroyed) {
            ship->hit(segmentIndex);

            if (isDoubleDamageUsed) {
                ship->hit(segmentIndex);
                isDoubleDamageUsed = false;
            }  

            return ship->isDestroyed();
        }
    } else {
        cell.status = CellStatus::Empty;
    }
    return false;
}

bool GameBoard::isShipSegment(int x, int y){
    if (x >= 0 && x < width && y >= 0 && y < height && board[x][y].status == CellStatus::Ship){
        return true;
    }
    return false;
}

Cell& GameBoard::getCell(int x, int y){
    Cell& cell = board[x][y];
    return cell;
}

void GameBoard::printBoard() const {
    std::cout << "   ";
    for (int x = 0; x < width; ++x) {
        std::cout << std::setw(2) << x << " ";
    }
    std::cout << std::endl;

    for (int y = 0; y < height; ++y) {
        std::cout << std::setw(2) << y << " ";
        for (int x = 0; x < width; ++x) {
            char cellChar;
            if (board[x][y].status == CellStatus::Ship && board[x][y].shipSegment.has_value()) {
                auto [ship, segmentIndex] = board[x][y].shipSegment.value();
                switch (ship->getSegmentStatus(segmentIndex)) {
                    case SegmentStatus::Intact:
                        cellChar = 'X';
                        break;
                    case SegmentStatus::Damaged:
                        cellChar = 'D';
                        break;
                    case SegmentStatus::Destroyed:
                        cellChar = '!';
                        break;
                }
            } else {
                switch (board[x][y].status) {
                    case CellStatus::Unknown:
                        cellChar = '~';
                        break;
                    case CellStatus::Empty:
                        cellChar = 'O';
                        break;
                    case CellStatus::Ship:
                        cellChar = 'X'; 
                        break;
                }
            }
            std::cout << " " << cellChar << " ";
        }
        std::cout << std::endl;
    }
}

void GameBoard::activateDoubleDamage() {
    isDoubleDamageUsed = true;
}

bool GameBoard::areNeighborsValid(int nx, int ny) const{
    // Проверяем все соседние клетки
    const int directions[8][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1},
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1}    
    };

    for (size_t i = 0; i < 8; ++i) {
        int neighborX = nx + directions[i][0];
        int neighborY = ny + directions[i][1];

        if (neighborX >= 0 && neighborX < height && neighborY >= 0 && neighborY < width) {
            if (board[neighborX][neighborY].status != CellStatus::Unknown) {
                return false; // Найдена клетка с недопустимым значением
            }
        }
    }

    return true;
}

bool GameBoard::canPlaceShip(const Ship& ship, int x, int y, Orientation orientation) const {
    int dx = (orientation == Orientation::Horizontal) ? 1 : 0;
    int dy = (orientation == Orientation::Vertical) ? 1 : 0;

    for (int i = 0; i < ship.getLength(); ++i) {
        int nx = x + i * dx;
        int ny = y + i * dy;
        if (nx < 0 || nx >= width || ny < 0 || ny >= height || board[nx][ny].status != CellStatus::Unknown || !areNeighborsValid(nx, ny)) {
            return false;
        }
    }
    return true;
}
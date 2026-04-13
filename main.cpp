#include "raylib.h"
#include <utility>
#include <vector>
#include <iostream>
#include <cstdlib>  
#include <ctime>
#include "simple_bot.h"
#include "smart_bot.h"
#include "my_types.h"
#include "ship.h"
const int CELL_SIZE = 50;
const int BOARD_SIZE = 10;
const int WINDOW_WIDTH = BOARD_SIZE * CELL_SIZE * 2 + 200; 
const int WINDOW_HEIGHT = BOARD_SIZE * CELL_SIZE + 100;

struct GameState 
{
    Field playerField; // Поле игрока
    Field botField; // Поле бота
    Bot* bot;// Текущий бот
    bool isPlayerTurn;// Ход true - игрок, false - bot
    bool gameOver;
    std::string message;
    bool placingShips;

    GameState()
    {
        playerField = Field(BOARD_SIZE, std::vector<int>(BOARD_SIZE, CELL::EMPTY));
        botField = Field(BOARD_SIZE, std::vector<int>(BOARD_SIZE, CELL::EMPTY));
        placingShips = true; 
        isPlayerTurn = true;
        gameOver = false;
        message = "Ходи!";
    }
};
// прототипы
void DrawBoard(const Field& field, int startX, int startY, bool showShips); // отрисовка поля
void DrawGrid(int startX, int startY); // отрисовка сетки
void DrawShips(const Field& field, int startX, int startY); // отрисовка кораблей
void DrawHits(const Field& field, int startX, int startY); // отрисовка попаданий и промахов напримор Х и О
Dot GetMouseCell(int startX, int startY); // по координатам мыши в какую клетку кликнули
bool PlaceShipOnField(Field& field, const Ship& ship); //размещение
bool IsValidPlacement(const Field& field, const Ship& ship); //можно ли вообще разместить
void PlaceBotShips(Field& field); //размещение всех 10 кораблей для бота
bool CheckShot(Field& field, const Dot& shot, CELL::SHOTRESULTS& result); //проверка выстрела
void MarkSink(Field& field, const Dot& hit); // отметим клетки вокруг потопленного корабля
void GameLoop();// основной цикл


// рисуем сетку
void DrawGrid(int startX, int startY)
{
    for (int i = 0; i <= BOARD_SIZE; i++)
    {
        DrawLine(startX + i * CELL_SIZE, startY, startX + i * CELL_SIZE, startY + BOARD_SIZE * CELL_SIZE, GRAY);
        DrawLine(startX, startY + i * CELL_SIZE, startX + BOARD_SIZE * CELL_SIZE, startY + i * CELL_SIZE, GRAY);
    }
}
// рисуем корабли
void DrawShips(const Field& field, int startX, int startY)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (field[i][j] == CELL::PLACED)
            {
                DrawRectangle(startX + j * CELL_SIZE, startY + i * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
            }
        }
    }
}
// отрисовка выстрелов
void DrawHits(const Field& field, int startX, int startY)
{
    for (int i = 0; i < BOARD_SIZE; i++)          
    {
        for (int j = 0; j < BOARD_SIZE; j++)     
        {
            if (field[i][j] == CELL::HITED)      
            {
                DrawRectangle(startX + j * CELL_SIZE, startY + i * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);
            }
            else if (field[i][j] == CELL::MISSED) 
            {
                DrawRectangle(startX + j * CELL_SIZE, startY + i * CELL_SIZE, CELL_SIZE, CELL_SIZE, LIGHTGRAY);
            }
            else if (field[i][j] == CELL::SINKED)
            {
                DrawRectangle(startX + j * CELL_SIZE, startY + i * CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKGRAY);
            }
        }
    }
}

// написав промежут. функции отрисовки, рисуем все сразу
void DrawBoard(const Field& field, int startX, int startY, bool showShips)
{
    DrawGrid(startX, startY);
    if (showShips) DrawShips(field, startX, startY);
    DrawHits(field, startX, startY);
}
//верните координату курсора
Dot GetMouseCell(int startX, int startY)
{
    int col = (GetMouseX() - startX) / CELL_SIZE;
    int row = (GetMouseY() - startY) / CELL_SIZE;

    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
    {
        return Dot(row, col);
    }
    return Dot(-1, -1);
}
bool IsValidPlacement(const Field& field, const Ship& ship)
{
    for (int i = 0; i < ship.positions.size(); i++)
    {
        int row = ship.positions[i].first;
        int col = ship.positions[i].second;

        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        {
            return false;
        }

        if (field[row][col] != CELL::EMPTY)
        {
            return false;
        }

        for (int dr = -1; dr <= 1; dr++)
        {
            for (int dc = -1; dc <= 1; dc++)
            {
                int nr = row + dr;
                int nc = col + dc;
                if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE)
                {
                    if (field[nr][nc] == CELL::PLACED)
                        return false;
                }
            }
        }
    }
    return true;
}
//размещение
bool PlaceShipOnField(Field& field, const Ship& ship)
{
    if (!IsValidPlacement(field, ship))
    {
        return false;
    }
    for (const auto& pos : ship.positions)
    {
        field[pos.first][pos.second] = CELL::PLACED;
    }
    return true;
}
// корабли бота
void PlaceBotShips(Field& field)
{
    std::vector<int> shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    for (int size : shipSizes)
    {
        bool placed = false;
        while (!placed)
        {
            int direction = rand() % 4;
            int row = rand() % BOARD_SIZE;
            int col = rand() % BOARD_SIZE;

            Ship ship(size, Dot{row, col}, FIELD::dirs[direction]);

            if (IsValidPlacement(field, ship))
            {
                PlaceShipOnField(field, ship);
                placed = true;
            }
        }
    }
}
//проверка выстрела
bool CheckShot(Field& field, const Dot& shot, CELL::SHOTRESULTS& result)
{
    int row = shot.first;
    int col = shot.second;
    
    //выход за границы
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
    {
        result = CELL::INVALID;
        return false;
    }
    
    int cell = field[row][col];
    
    // уже стреляли
    if (cell == CELL::MISSED || cell == CELL::HITED || cell == CELL::SINKED)
    {
        result = CELL::INVALID;
        return false;
    }
    
    //попадание или промах
    if (cell == CELL::PLACED)
    {
        field[row][col] = CELL::HITED;
        result = CELL::HIT;
        return true;
    }
    
    field[row][col] = CELL::MISSED;
    result = CELL::MISS;
    return false;
}

void GameLoop()
{
    std::vector<int> shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    int shipIndex = 0;
    Dot MouseDot;
    FIELD::Directions dir = FIELD::Directions::RIGHT;
    GameState game;

    while (!WindowShouldClose() && game.placingShips)
    {
        //расстановка кораблей
        if (shipIndex >= (int)shipSizes.size())
        {
            game.placingShips = false;
            continue;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            dir = (dir == FIELD::Directions::RIGHT) ? FIELD::Directions::DOWN : FIELD::Directions::RIGHT;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Dot start = GetMouseCell(50, 50);
            if (start.first == -1)
            {
                continue;
            }
            Ship newShip(shipSizes[shipIndex], start, FIELD::dirs[dir]);

            if (!PlaceShipOnField(game.playerField, newShip))
            {
                continue;
            }
            shipIndex++;
            
        }
    }
    while (!WindowShouldClose() && !game.gameOver)
    {
        //ход игрока

        if (game.isPlayerTurn)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Dot shot = GetMouseCell(50 + BOARD_SIZE * CELL_SIZE + 100, 50);
                CELL::SHOTRESULTS shotresult;
                CheckShot(game.botField, shot, shotresult);
                if (shotresult == CELL::SHOTRESULTS::HIT)
                {
                    game.botField[shot.first][shot.second] = CELL::HITED;
                
                }
                if (shotresult == CELL::SHOTRESULTS::MISS)
                {
                    game.botField[shot.first][shot.second] = CELL::MISSED;
                }
                
                if (shotresult == CELL::SHOTRESULTS::SINK)
                {
                    // здесь сложнее, не только отметить потопление , но и отметить клетки вокруг, можно отдельно выдать функцию
                }
            }
        }
        //ход бота
        else if (!game.isPlayerTurn)
        {
            
        }


    }
}

int main(void)
{

    return 0;
}

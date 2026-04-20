#include "raylib.h"
#include <utility>
#include <vector>
#include <iostream>
#include <cstdlib> 
#include <string>
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
//void DrawBoard(const Field& field, int startX, int startY, bool showShips); // отрисовка поля
//void DrawGrid(int startX, int startY); // отрисовка сетки
//void DrawShips(const Field& field, int startX, int startY); // отрисовка кораблей
//void DrawHits(const Field& field, int startX, int startY); // отрисовка попаданий и промахов напримор Х и О
//Dot GetMouseCell(int startX, int startY); // по координатам мыши в какую клетку кликнули
//bool PlaceShipOnField(Field& field, const Ship& ship); //размещение
//bool IsValidPlacement(const Field& field, const Ship& ship); //можно ли вообще разместить
//void PlaceBotShips(Field& field); //размещение всех 10 кораблей для бота
//bool CheckShot(Field& field, const Dot& shot, CELL::SHOTRESULTS& result); //проверка выстрела
//void MarkSink(Field& field, const Dot& hit); // отметим клетки вокруг потопленного корабля
//void GameLoop();// основной цикл
//bool CheckVictory(const Field& field);
//bool IsShipSunk(const Field& field, const Dot& hit, const std::vector<Ship>& ships);
//void MarkShipAsSunk(Field& field, const std::vector<Ship>& ships, const Dot& hit);


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
// проверка каждого на существование
bool IsShipSunk(const Field& field, const Dot& hit, const std::vector<Ship>& ships)
{
    int row = hit.first;
    int col = hit.second;

    const Ship* hitShip = nullptr;
    for (const auto& ship : ships)
    {
        for (const auto& pos : ship.positions)
        {
            if (pos.first == row && pos.second == col)
            {
                hitShip = &ship;
                break;
            }
        }
        if (hitShip) break;
    }
    if (!hitShip) return false;

    for (const auto& pos : hitShip->positions)
    {
        if (field[pos.first][pos.second] == CELL::PLACED)
        {
            return false; 
        }
    }

    return true; 
}
void MarkShipAsSunk(Field& field, const std::vector<Ship>& ships, const Dot& hit)
{
    const Ship* sunkShip = nullptr;
    for (const auto& ship : ships)
    {
        for (const auto& pos : ship.positions)
        {
            if (pos.first == hit.first && pos.second == hit.second)
            {
                sunkShip = &ship;
                break;
            }
        }
        if (sunkShip) break;
    }

    if (!sunkShip) return;

    // отмечаем SINKED
    for (const auto& pos : sunkShip->positions)
    {
        field[pos.first][pos.second] = CELL::SINKED;
    }

    // отмечаем  MISSED вокруг этого корабля
    for (const auto& pos : sunkShip->positions)
    {
        for (int dr = -1; dr <= 1; dr++)
        {
            for (int dc = -1; dc <= 1; dc++)
            {
                int nr = pos.first + dr;
                int nc = pos.second + dc;
                if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE)
                {
                    if (field[nr][nc] == CELL::EMPTY)
                    {
                        field[nr][nc] = CELL::MISSED;
                    }
                }
            }
        }
    }
}
bool CheckVictory(const Field& field)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (field[i][j] == CELL::PLACED)
            {
                return false;
            }
        }
    }
    return true;
}

void DrawPlacementPhase(Font text, const Field& playerField, int shipIndex, int totalShips, FIELD::Directions dir, const std::vector<int>& shipSizes)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTextEx(text, "РАССТАНОВКА КОРАБЛЕЙ", { WINDOW_WIDTH / 2 - 150, 10 }, 20, 5, DARKBLUE);
    DrawBoard(playerField, 50, 50, true);

    DrawRectangle(0, WINDOW_HEIGHT - 80, WINDOW_WIDTH, 80, LIGHTGRAY);

    if (shipIndex < totalShips)
    {
        std::string hint;

        if (dir == FIELD::Directions::RIGHT)
        {
            hint = "Корабль размером " + std::to_string(shipSizes[shipIndex]) + " | Направление: вправо (ПКМ для смены)";
        }
        else
        {
            hint = "Корабль размером " + std::to_string(shipSizes[shipIndex]) + " | Направление: вниз (ПКМ для смены)";
        }
        DrawTextEx(text, hint.c_str(), { 50, WINDOW_HEIGHT - 55 }, 20, 5, DARKGREEN);

        std::string remaining = "Осталось кораблей: " + std::to_string(totalShips - shipIndex);
        DrawTextEx(text, remaining.c_str(), { 50, WINDOW_HEIGHT - 30 }, 20, 5, DARKGREEN);
    }

    EndDrawing();
}
void ProcessPlayerShot(Font text, GameState& game)
{
    Dot shot = GetMouseCell(50 + BOARD_SIZE * CELL_SIZE + 100, 50);
    if (shot.first == -1)
    {
        return;
    }

    int cell = game.botField[shot.first][shot.second];
    if (cell == CELL::HITED || cell == CELL::MISSED || cell == CELL::SINKED)
    {
        return;
    }

    CELL::SHOTRESULTS shotresult;
    bool hit = CheckShot(game.botField, shot, shotresult);

    if (hit)
    {
        game.message = "ПОПАДАНИЕ! Еще ход!";
        if (CheckVictory(game.botField))
        {
            game.gameOver = true;
            game.message = "ВЫ ПОБЕДИЛИ!";
        }
    }
    else
    {
        game.message = "ПРОМАХ! Ход бота";
        game.isPlayerTurn = false;
    }
}
void ProcessBotShot(Font text, GameState& game, const std::vector<Ship>& playerShips)
{
    Dot shot = game.bot->hit();

    if (shot.first == -1)
    {
        return;
    }

    CELL::SHOTRESULTS shotresult;
    bool hit = CheckShot(game.playerField, shot, shotresult);

    game.bot->setResults(shotresult);

    if (hit)
    {
        game.message = "БОТ ПОПАЛ!";

        if (IsShipSunk(game.playerField, shot, playerShips))
        {
            MarkShipAsSunk(game.playerField, playerShips, shot);
            game.message = "БОТ ПОТОПИЛ КОРАБЛЬ! ";
        }

        if (CheckVictory(game.playerField))
        {
            game.gameOver = true;
            game.message = "БОТ ПОБЕДИЛ!";
        }
        else
        {
            game.message = game.message + "Он стреляет еще раз";
        }
    }
    else
    {
        game.message = "БОТ ПРОМАХНУЛСЯ! Ваш ход";
        game.isPlayerTurn = true;
    }
}
void DrawGamePhase(Font text, const GameState& game)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTextEx(text, "ВАШЕ ПОЛЕ", { 50, 20 }, 20, 5, DARKBLUE);
    DrawTextEx(text, "ПОЛЕ БОТА", { 50 + BOARD_SIZE * CELL_SIZE + 100, 20 }, 20, 5, DARKBLUE);

    DrawBoard(game.playerField, 50, 50, true);
    DrawBoard(game.botField, 50 + BOARD_SIZE * CELL_SIZE + 100, 50, false);

    DrawRectangle(0, WINDOW_HEIGHT - 80, WINDOW_WIDTH, 80, LIGHTGRAY);
    DrawTextEx(text, game.message.c_str(), { 50, WINDOW_HEIGHT - 55 }, 25, 5, DARKBLUE);

    if (!game.gameOver)
    {
        if (game.isPlayerTurn)
        {
            DrawTextEx(text, "ВАШ ХОД", { 50, WINDOW_HEIGHT - 30 }, 20, 5, RED);
        }
        else
        {
            DrawTextEx(text, "ХОД БОТА...", { 50, WINDOW_HEIGHT - 30 }, 20, 5, ORANGE);
        }
    }
    EndDrawing();
}
void GameLoop(Font text)
{
    std::vector<int> shipSizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    int shipIndex = 0;
    FIELD::Directions dir = FIELD::Directions::RIGHT;
    GameState game;
    std::vector<Ship> playerShips;

    SmartBot smartBot;
    game.bot = &smartBot;

    PlaceBotShips(game.botField);

    //расстановка
    while (!WindowShouldClose() && game.placingShips)
    {
        if (shipIndex >= (int)shipSizes.size())
        {
            game.placingShips = false;
            game.message = "Корабли расставлены! Ваш ход!";
            continue;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if (dir == FIELD::Directions::RIGHT)
            {
                dir = FIELD::Directions::DOWN;
            }
            else
            {
                dir = FIELD::Directions::RIGHT;
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Dot start = GetMouseCell(50, 50);
            if (start.first != -1)
            {
                Ship newShip(shipSizes[shipIndex], start, FIELD::dirs[dir]);
                if (PlaceShipOnField(game.playerField, newShip))
                {
                    playerShips.push_back(newShip);
                    shipIndex = shipIndex + 1;
                }
            }
        }
        DrawPlacementPhase(text, game.playerField, shipIndex, (int)shipSizes.size(), dir, shipSizes);
    }

    if (WindowShouldClose())
    {
        return;
    }

    //игра
    while (!WindowShouldClose() && !game.gameOver)
    {
        if (game.isPlayerTurn)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                ProcessPlayerShot(text, game);
            }
        }
        else
        {
            ProcessBotShot(text, game, playerShips);
        }
        DrawGamePhase(text, game);
    }
}
Font InitRussianFont(const char* fontPath, int fontSize) {
    int charsCount = 0;
    // Загружаем все русские буквы, цифры и знаки препинания
    int* chars = LoadCodepoints(
        "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
        "0123456789"
        ".,!?-+()[]{}:;/\\\"'`~@#$%^&*=_|<> ",
        &charsCount
    );

    Font font = LoadFontEx(fontPath, fontSize, chars, charsCount);
    UnloadCodepoints(chars);
    return font;
}

int main(void)
{
    srand(time(NULL));
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Морской бой");
    Font text = InitRussianFont("C:/Windows/Fonts/Arial.ttf", 32);
    std::cout << text.texture.id;
    GameLoop(text);
    CloseWindow();
    return 0;
}

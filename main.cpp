#include "raylib.h"
#include <utility>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "simple_bot.h"
#include "smart_bot.h"
#include "my_types.h"
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

    GameState()
    {
        playerField = Field(BOARD_SIZE, std::vector<int>(BOARD_SIZE, CELL::EMPTY));
        botField = Field(BOARD_SIZE, std::vector<int>(BOARD_SIZE, CELL::EMPTY));
        botRealField = Field(BOARD_SIZE, std::vector<int>(BOARD_SIZE, CELL::EMPTY));
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
bool PlaceShipOnField(Field& field, const Ship& ship); // размещение
bool IsValidPlacement(const Field& field, const Ship& ship); // можно ли вообще разместить
void PlaceBotShips(Field& field); // размещение всех 10 кораблей для бота
bool CheckShot(Field& field, const Dot& shot, CELL::SHOTRESULTS& result); //проверка выстрела
void MarkSink(Field& field, const Dot& hit); // отметим клетки вокруг потопленного корабля
void GameLoop();// основной цикл


void GameLoop()
{
    std::vector<int> shipSizes = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    int sizeIdX = 0;
    Dot MouseDot;
    FIELD::Directions dir = FIELD::Directions::RIGHT;
    GameState game;
    
    while (!WindowShouldClose() && !game.gameOver)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            dir = FIELD::Directions::UP;

        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            MouseDot.first = (GetMouseX() - 50) / 50;
            MouseDot.second = (GetMouseY() - 50) / 50;
            Ship ship(shipSizes[sizeIdX]);
            for (int i = 0; i < shipSizes[sizeIdX]; i++)
            {

            }
        }
    }
}

//int CheckHit(int mouseX, int mouseY)
//{
//    for (int i = 0; i < positions.size(); i++)
//    {
//        int rectX = positions[i].first * CELL_SIZE;
//        int rectY = positions[i].second * CELL_SIZE;
//
//        if (mouseX >= rectX && mouseX <= rectX + CELL_SIZE &&
//            mouseY >= rectY && mouseY <= rectY + CELL_SIZE)
//        {
//            return i;
//        }
//    }
//    return -1;
//}

#include <cstdlib>
int main(void)
{
    srand(time(NULL));
    std::cout << rand() % 10 << std::endl;
    std::cout << rand() % 10 << std::endl;
    std::cout << rand() % 10 << std::endl;
    std::cout << rand() % 10 << std::endl;
    std::cout << rand() % 10 << std::endl;
    std::cout << rand() % 10 << std::endl;
    std::cout << rand() % 10 << std::endl;


    //InitWindow(800, 450, "raylib [core] example - basic window");
    //SetTargetFPS(1);
    //bool isgot = false;
    //while (!WindowShouldClose())
    //{

    //    BeginDrawing();
    //    ClearBackground(RAYWHITE);
    //    if (isgot == false)
    //    {
    //        DrawRectangle(100, 100, 200, 200, BLUE);
    //    }
    //    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    //    {
    //        int MouseX = GetMouseX();
    //        int MouseY = GetMouseY();
    //        if (MouseX >= 100 && MouseX <= 300 && MouseY >= 100 && MouseY <= 300)
    //        {
    //            isgot = true;
    //        }
    //    }

    //    EndDrawing();
    //}

    //CloseWindow();

    return 0;
}

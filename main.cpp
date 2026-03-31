#include "raylib.h"
#include <utility>
#include <vector>
#include <iostream>
const int CELL_SIZE = 50;

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

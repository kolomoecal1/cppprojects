#include "ship.h"

Ship::Ship() = default;
Ship::Ship(int size, Dot first, Dir dir) : positions(size)
{
    for (int i = 0; i < size; i++)
    {
        positions[i].first = first.first + dir.first * i;
        positions[i].second = first.second + dir.second * i;

    }
}
#pragma once
#include <vector>
#include "my_types.h"

class Ship
{
protected:
    std::vector<Dot> positions;
public:
    Ship();
    Ship(int size, Dot first, Dir dir);
};
#pragma once
#include <vector>
#include "my_types.h"

class Ship
{
protected:
    
public:
    std::vector<Dot> positions;
    Ship();
    Ship(int size, Dot first, Dir dir);
};
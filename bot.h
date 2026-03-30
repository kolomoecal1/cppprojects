#pragma once
#include "my_types.h"
class Bot
{
public:
	virtual Dot hit() = 0;
	virtual std::pair<Dot, Dir> place();
	virtual ~Bot();
};
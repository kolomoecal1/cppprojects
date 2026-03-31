#pragma once
#include "my_types.h"
class Bot
{
public:
	virtual void setResults(CELL::SHOTRESULTS shotres) = 0;
	virtual Dot hit() = 0;
	virtual std::pair<Dot, Dir> place();
	virtual ~Bot();
};
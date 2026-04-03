#pragma once
#include "my_types.h"
class Bot
{
public:
<<<<<<< HEAD
	virtual void setResults(CELL::SHOTRESULTS shotres) = 0;
=======
>>>>>>> a19bc8c (Flat Arch implemented, NEED TO WRITE SMART BOT)
	virtual Dot hit() = 0;
	virtual std::pair<Dot, Dir> place();
	virtual ~Bot();
};
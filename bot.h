#pragma once
#include "my_types.h"
class Bot
{
public:
<<<<<<< HEAD
	virtual void setResults(CELL::SHOTRESULTS shotres) = 0;
=======
>>>>>>> 1bbf94d8bf81a7720985adced9869e361c63ea1c
	virtual Dot hit() = 0;
	virtual std::pair<Dot, Dir> place();
	virtual ~Bot();
};
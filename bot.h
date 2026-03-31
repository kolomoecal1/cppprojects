#pragma once
#include "my_types.h"
class Bot
{
public:
<<<<<<< HEAD
<<<<<<< HEAD
	virtual void setResults(CELL::SHOTRESULTS shotres) = 0;
=======
>>>>>>> 1bbf94d8bf81a7720985adced9869e361c63ea1c
=======
	virtual void setResults(CELL::SHOTRESULTS shotres) = 0;
>>>>>>> a3e9ffe8e624005a82bf546d7cfe44ca4e4b391f
	virtual Dot hit() = 0;
	virtual std::pair<Dot, Dir> place();
	virtual ~Bot();
};
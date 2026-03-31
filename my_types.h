<<<<<<< HEAD
п»ї#pragma once
=======
#pragma once
>>>>>>> a3e9ffe8e624005a82bf546d7cfe44ca4e4b391f
#include <utility>
#include <vector>
typedef std::pair<int, int> Dot;
typedef std::pair<int, int> Dir;
typedef std::vector<std::vector<int>> Field;


namespace CELL {
	enum SHOTRESULTS
	{
<<<<<<< HEAD
		INVALID,
		MISS,
		HIT, 
		SINK,
=======
		INVALID, // в ту же клетку, за пределы поля и тд
		MISS, // не попали по кораблю
		HIT, // попали
		SINK, //потопили
>>>>>>> a3e9ffe8e624005a82bf546d7cfe44ca4e4b391f
	};
	enum CELL
	{
		EMPTY,
		PLACED,
		MISSED,
		HITED,
		SINKED,
	};
}

namespace FIELD {
	enum Directions {
		UP,
		RIGHT,
		DOWN,
		LEFT,
<<<<<<< HEAD
		COUNT,
=======
		COUNT
>>>>>>> a3e9ffe8e624005a82bf546d7cfe44ca4e4b391f
	};

	Dir dirs[] = {
		{-1, 0},
		{0, 1},
		{1, 0},
		{0, -1}
	};

	const int SIZE = 10;
}
#pragma once
#include <utility>
#include <vector>
typedef std::pair<int, int> Dot;
typedef std::pair<int, int> Dir;
typedef std::vector<std::vector<int>> Field;


namespace CELL {
<<<<<<< HEAD
	enum SHOTRESULTS
	{
		INVALID, // в ту же клетку, за пределы поля и тд
		MISS, // не попали по кораблю
		HIT, // попали
		SINK, //потопили
	};
=======
>>>>>>> a19bc8c (Flat Arch implemented, NEED TO WRITE SMART BOT)
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
		COUNT
	};

	Dir dirs[] = {
		{-1, 0},
		{0, 1},
		{1, 0},
		{0, -1}
	};

	const int SIZE = 10;
}
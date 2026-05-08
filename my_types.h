#pragma once
#include <utility>
#include <vector>
typedef std::pair<int, int> Dot;
typedef std::pair<int, int> Dir;
typedef std::vector<std::vector<int>> Field;
//typedef std::vector<std::pair<int, int>> Ship;

namespace CELL {
	enum SHOTRESULTS
	{
		INVALID, 
		MISS, 
		HIT, 
		SINK, 
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
		COUNT
	};

	extern Dir dirs[4];

	const int SIZE = 10;
}
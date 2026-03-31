#pragma once
#include "smart_bot.h"
#include <algorithm>
#include <cstdlib>
#include <vector>
Dot SmartBot::hit()
{



	Dot result;
	switch (state)
	{
		case State::RANDOM:
		{
			result = randomShot();
			break;
		}
		case State::HIT:
		{
			if (!task.empty()) // счастье какое, попали
			{
				result = task.back();
				task.pop_back();
			}
			else // пусто, сначала ищем вокруг последнего попадания или возв. к случ.
			{
				result = searchAround(lastHit);
				if (result.first == -1)
				{
					state = State::RANDOM;
					result = randomShot();
				}
			}
			break;
		}
	}
}



Dot SmartBot::randomShot() // генерируем случайные значения
{
	Dot result;
	result = { rand() % FIELD::SIZE, rand() % FIELD::SIZE };
	return result;
}

Dot SmartBot::searchAround(const Dot& dot)
{
	std::vector(Dir) dirs = { {-1 ,0}, {1, 0}, {0, -1}, {0, 1} };
}
for (int i = 0; i < 4; i++)
{
	int j = rand() % 4;
	std::swap(dirs[i], dirs[j])
}

void setResults(CELL::SHOTRESULTS shotres)
{
	switch (shotres)
	{
		case CELL::HIT:
		{
			updateStateAfterHit(lastHit);
			break;
		}
		case CELL::MISS:
		{
			updateStateAfterMiss(lastHit);
			break;
		}
		case CELL::SINK:
		{
			state = State::RANDOM;
			task.clear();
			break;
		}
		case CELL::INVALID:
		{
			break;
		}
	}
}

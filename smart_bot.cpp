#pragma once
#include "my_types.h"
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

void SmartBot::updateStateAfterHit()
{
	if (state == State::RANDOM)
	{
		state = State::HIT;
		for (auto dir : FIELD::dirs)
		{
			Dot next = { lastHit.first + dir.first, lastHit.second + dir.second };
			task.push_back(next);
		}
		
	}
	else
	{
		Dot forward = { lastHit.first + currentDir.first, lastHit.second + currentDir.second };

	}
}
void  SmartBot::setResults(CELL::SHOTRESULTS shotres)
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

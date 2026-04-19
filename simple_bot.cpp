#include "simple_bot.h"
#include <cstdlib>

Dot SimpleBot::hit()
{
	Dot dot{ rand() % FIELD::SIZE, rand() % FIELD::SIZE };
	return dot;
}
void setResults(CELL::SHOTRESULTS shotres)
{
	return;
}
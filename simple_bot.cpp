#include "simple_bot.h"
#include <cstdlib>

Dot SimpleBot::hit()
{
    Dot result;
    result.first = rand() % FIELD::SIZE;
    result.second = rand() % FIELD::SIZE;
    return result;
}

void SimpleBot::setResults(CELL::SHOTRESULTS shotres)
{
    // Простой бот не использует результаты выстрелов
    // Метод нужен только для соответствия интерфейсу Bot
}
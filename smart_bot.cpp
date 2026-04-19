#include "smart_bot.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

SmartBot::SmartBot()
    : state(State::RANDOM)
    , currentDir({ 0, 0 })
{
    lastHit.first = -1;
    lastHit.second = -1;

    static bool seeded = false;
    if (!seeded)
    {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }
}

SmartBot::~SmartBot()
{
}


Dot SmartBot::randomShot()
{
    Dot result;
    result.first = rand() % FIELD::SIZE;
    result.second = rand() % FIELD::SIZE;
    return result;
}

Dot SmartBot::searchAround(const Dot& dot)
{
    std::vector<Dir> directions;
    for (int i = 0; i < 4; i++)
    {
        directions.push_back(FIELD::dirs[i]);
    }

    for (int i = 0; i < 4; ++i)
    {
        int j = rand() % 4;
        Dir temp = directions[i];
        directions[i] = directions[j];
        directions[j] = temp;
    }

    for (int i = 0; i < 4; ++i)
    {
        int newX = dot.first + directions[i].first;
        int newY = dot.second + directions[i].second;

        if (newX >= 0 && newX < FIELD::SIZE &&
            newY >= 0 && newY < FIELD::SIZE)
        {
            Dot result;
            result.first = newX;
            result.second = newY;
            return result;
        }
    }

    return randomShot();
}

void SmartBot::updateStateAfterMiss()
{
    if (state == State::HIT)
    {
        currentDir.first = -currentDir.first;
        currentDir.second = -currentDir.second;

        int newX = lastHit.first + currentDir.first;
        int newY = lastHit.second + currentDir.second;

        if (newX < 0 || newX >= FIELD::SIZE ||
            newY < 0 || newY >= FIELD::SIZE)
        {
            state = State::RANDOM;
            currentDir.first = 0;
            currentDir.second = 0;
        }
    }
}

void SmartBot::updateStateAfterHit(const Dot& hit)
{
    lastHit = hit;
    state = State::HIT;

    if (currentDir.first == 0 && currentDir.second == 0)
    {
        int dirIndex = rand() % 4;
        currentDir = FIELD::dirs[dirIndex];
    }
}

Dot SmartBot::getNextDirection()
{
    int newX = lastHit.first + currentDir.first;
    int newY = lastHit.second + currentDir.second;

    if (newX >= 0 && newX < FIELD::SIZE &&
        newY >= 0 && newY < FIELD::SIZE)
    {
        Dot result;
        result.first = newX;
        result.second = newY;
        return result;
    }

    currentDir.first = -currentDir.first;
    currentDir.second = -currentDir.second;

    newX = lastHit.first + currentDir.first;
    newY = lastHit.second + currentDir.second;

    if (newX < 0 || newX >= FIELD::SIZE || newY < 0 || newY >= FIELD::SIZE)
    {
        state = State::RANDOM;
        currentDir.first = 0;
        currentDir.second = 0;
        return randomShot();
    }

    Dot result;
    result.first = newX;
    result.second = newY;
    return result;
}

Dot SmartBot::hit()
{
    Dot result;

    switch (state)
    {
    case State::RANDOM:
        result = randomShot();
        break;

    case State::HIT:
        result = getNextDirection();
        break;
    }

    return result;
}

void SmartBot::setResults(CELL::SHOTRESULTS result)
{
    switch (result)
    {
    case CELL::HIT:
        updateStateAfterHit(lastHit);
        break;

    case CELL::SINK:
        reset();
        break;

    case CELL::MISS:
        updateStateAfterMiss();
        break;

    default:
        break;
    }
}

void SmartBot::reset()
{
    state = State::RANDOM;
    lastHit.first = -1;
    lastHit.second = -1;
    currentDir.first = 0;
    currentDir.second = 0;
}
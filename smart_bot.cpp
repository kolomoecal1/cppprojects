#include "smart_bot.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

SmartBot::SmartBot()
    : state(State::RANDOM)
    , currentDir({0, 0})
{
    lastHit.first = -1;
    lastHit.second = -1;
    firstHit.first = -1;
    firstHit.second = -1;

    static bool seeded = false;
    if (!seeded)
    {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }
    
    initAvailableShots();
}

SmartBot::~SmartBot()
{
}

void SmartBot::initAvailableShots()
{
    availableShots.clear();
    for (int i = 0; i < FIELD::SIZE; i++)
    {
        for (int j = 0; j < FIELD::SIZE; j++)
        {
            availableShots.insert({i, j});
        }
    }
}

bool SmartBot::isAlreadyShot(int row, int col) const
{
    return availableShots.find({row, col}) == availableShots.end();
}

void SmartBot::removeShotFromSets(int row, int col)
{
    availableShots.erase({row, col});
    possibleTargets.erase({row, col});
}

Dot SmartBot::getRandomShot()
{
    if (availableShots.empty())
    {
        return {-1, -1};
    }
    
    int index = rand() % availableShots.size();
    auto it = availableShots.begin();
    std::advance(it, index);
    
    Dot result = {it->first, it->second};
    removeShotFromSets(result.first, result.second);
    
    return result;
}

void SmartBot::addNeighborsToPossibleTargets(const Dot& dot)
{
    for (int i = 0; i < 4; i++)
    {
        int newX = dot.first + FIELD::dirs[i].first;
        int newY = dot.second + FIELD::dirs[i].second;
        
        if (newX >= 0 && newX < FIELD::SIZE && 
            newY >= 0 && newY < FIELD::SIZE &&
            !isAlreadyShot(newX, newY))
        {
            possibleTargets.insert({newX, newY});
        }
    }
}

Dot SmartBot::getNextHuntingShot()
{
    if (possibleTargets.empty())
    {
        state = State::RANDOM;
        return getRandomShot();
    }
    
    int index = rand() % possibleTargets.size();
    auto it = possibleTargets.begin();
    std::advance(it, index);
    
    Dot result = {it->first, it->second};
    removeShotFromSets(result.first, result.second);
    
    return result;
}

Dot SmartBot::getNextLineShot()
{
    // Пытаемся стрелять в текущем направлении
    int newX = lastHit.first + currentDir.first;
    int newY = lastHit.second + currentDir.second;
    
    if (newX >= 0 && newX < FIELD::SIZE && 
        newY >= 0 && newY < FIELD::SIZE &&
        !isAlreadyShot(newX, newY))
    {
        Dot result = {newX, newY};
        removeShotFromSets(result.first, result.second);
        return result;
    }
    
    // Если в текущем направлении нельзя, пробуем противоположное
    Dir oppositeDir = {-currentDir.first, -currentDir.second};
    newX = firstHit.first + oppositeDir.first;
    newY = firstHit.second + oppositeDir.second;
    
    if (newX >= 0 && newX < FIELD::SIZE && 
        newY >= 0 && newY < FIELD::SIZE &&
        !isAlreadyShot(newX, newY))
    {
        currentDir = oppositeDir;
        Dot result = {newX, newY};
        removeShotFromSets(result.first, result.second);
        return result;
    }
    
    // Если оба направления заблокированы, возвращаемся к случайным выстрелам
    state = State::RANDOM;
    clearPossibleTargets();
    return getRandomShot();
}

Dot SmartBot::hit()
{
    Dot result = {-1, -1};
    
    switch (state)
    {
    case State::RANDOM:  result = getRandomShot(); break;
    case State::HUNTING: result = getNextHuntingShot(); break;
    case State::LINE:    result = getNextLineShot(); break;
    }
    
    lastHit = result; // КРИТИЧНО: запоминаем, куда стрельнули
    return result;
}


void SmartBot::setResults(CELL::SHOTRESULTS result)
{
    if (lastHit.first == -1 && lastHit.second == -1)
    {
        // Если lastHit не установлен, не обрабатываем
        return;
    }
    
    switch (result)
    {
    case CELL::HIT:
        {
            if (state == State::RANDOM)
            {
                // Первое попадание - переходим в режим охоты
                state = State::HUNTING;
                firstHit = lastHit;
                addNeighborsToPossibleTargets(lastHit);
            }
            else if (state == State::HUNTING)
            {
                // Второе попадание - определяем направление и переходим в линейный режим
                state = State::LINE;
                currentDir.first = lastHit.first - firstHit.first;
                currentDir.second = lastHit.second - firstHit.second;
                // Нормализуем направление
                if (currentDir.first != 0) currentDir.first = (currentDir.first > 0) ? 1 : -1;
                if (currentDir.second != 0) currentDir.second = (currentDir.second > 0) ? 1 : -1;
                
                // Добавляем соседей в направлении
                int nextX = lastHit.first + currentDir.first;
                int nextY = lastHit.second + currentDir.second;
                if (nextX >= 0 && nextX < FIELD::SIZE && 
                    nextY >= 0 && nextY < FIELD::SIZE &&
                    !isAlreadyShot(nextX, nextY))
                {
                    possibleTargets.insert({nextX, nextY});
                }
            }
            else if (state == State::LINE)
            {
                // Продолжаем по линии
                int nextX = lastHit.first + currentDir.first;
                int nextY = lastHit.second + currentDir.second;
                if (nextX >= 0 && nextX < FIELD::SIZE && 
                    nextY >= 0 && nextY < FIELD::SIZE &&
                    !isAlreadyShot(nextX, nextY))
                {
                    possibleTargets.clear();
                    possibleTargets.insert({nextX, nextY});
                }
                else
                {
                    // Пробуем противоположное направление
                    Dir oppositeDir = {-currentDir.first, -currentDir.second};
                    nextX = firstHit.first + oppositeDir.first;
                    nextY = firstHit.second + oppositeDir.second;
                    if (nextX >= 0 && nextX < FIELD::SIZE && 
                        nextY >= 0 && nextY < FIELD::SIZE &&
                        !isAlreadyShot(nextX, nextY))
                    {
                        currentDir = oppositeDir;
                        possibleTargets.clear();
                        possibleTargets.insert({nextX, nextY});
                    }
                    else
                    {
                        // Корабль потоплен (должен быть SINK, но на всякий случай)
                        state = State::RANDOM;
                        clearPossibleTargets();
                    }
                }
            }
        }
        break;

    case CELL::SINK:
        // Корабль потоплен - сбрасываем всё и ищем новый
        reset();
        break;

    case CELL::MISS:
        {
            if (state == State::HUNTING)
            {
                // Удаляем этот промах из возможных целей
                possibleTargets.erase({lastHit.first, lastHit.second});
                if (possibleTargets.empty())
                {
                    // Нет больше целей - возвращаемся к случайным
                    state = State::RANDOM;
                }
            }
            else if (state == State::LINE)
            {
                // Промах по линии - пробуем противоположное направление
                Dir oppositeDir = {-currentDir.first, -currentDir.second};
                int newX = firstHit.first + oppositeDir.first;
                int newY = firstHit.second + oppositeDir.second;
                
                if (newX >= 0 && newX < FIELD::SIZE && 
                    newY >= 0 && newY < FIELD::SIZE &&
                    !isAlreadyShot(newX, newY))
                {
                    currentDir = oppositeDir;
                    possibleTargets.clear();
                    possibleTargets.insert({newX, newY});
                }
                else
                {
                    // Нет вариантов - возвращаемся к случайным
                    state = State::RANDOM;
                    clearPossibleTargets();
                }
            }
        }
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
    firstHit.first = -1;
    firstHit.second = -1;
    currentDir.first = 0;
    currentDir.second = 0;
    clearPossibleTargets();
}

void SmartBot::clearPossibleTargets()
{
    possibleTargets.clear();
}
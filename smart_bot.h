#pragma once
#include "bot.h"
#include <vector>
#include <set>

class SmartBot : public Bot
{
private:
    enum class State
    {
        RANDOM,          // Случайные выстрелы
        HUNTING,         // Охота вокруг последнего попадания
        LINE             // Стрельба по линии (после второго попадания)
    };

    State state;
    Dot lastHit;           // последнее попадание
    Dot firstHit;          // первое попадание в текущем корабле
    Dir currentDir;        // текущее направление поиска
    std::set<std::pair<int, int>> availableShots;  // доступные для выстрела клетки
    std::set<std::pair<int, int>> possibleTargets;  // потенциальные цели (для режима HUNTING)
    
    void initAvailableShots();
    bool isAlreadyShot(int row, int col) const;

public:
    SmartBot();
    ~SmartBot();

    Dot hit() override;
    void setResults(CELL::SHOTRESULTS result) override;
    void reset();

private:
    void addNeighborsToPossibleTargets(const Dot& dot);
    void updatePossibleTargetsAfterMiss(const Dot& shot);
    void clearPossibleTargets();
    Dot getRandomShot();
    Dot getNextHuntingShot();
    Dot getNextLineShot();
    void removeShotFromSets(int row, int col);
};
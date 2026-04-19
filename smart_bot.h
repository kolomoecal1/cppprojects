#pragma once
#include "bot.h"
#include <vector>

class SmartBot : public Bot
{
private:
    enum class State
    {
        RANDOM,          // Случайные выстрелы
        HIT,             // Обстрел вокруг попадания
    };

    State state;
    Dot lastHit;
    Dir currentDir;

public:
    SmartBot();
    ~SmartBot();

    Dot hit() override;
    void setResults(CELL::SHOTRESULTS result) override;
    void reset();

private:
    Dot randomShot();
    Dot searchAround(const Dot& dot);
    void updateStateAfterMiss();
    void updateStateAfterHit(const Dot& hit);
    Dot getNextDirection();
};
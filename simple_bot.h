#pragma once
#include "bot.h"

class SimpleBot : public Bot
{
public:
    Dot hit() override;
    void setResults(CELL::SHOTRESULTS shotres) override;
};
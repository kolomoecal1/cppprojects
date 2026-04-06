#pragma once
#include "bot.h"

class SmartBot : public Bot 
{
private:
    enum class State 
	{
        RANDOM,          
        HIT,              
    };
    
    State state = State::RANDOM;
    Dot lastHit;                 
    Dir currentDir;             
    std::vector<Dot> task;
    
    
public:
    SmartBot();
    Dot hit() override;
    void reset();

private:
    Dot randomShot(); //генерим случ коорд
    Dot searchAround(const Dot& dot); //ищет случайную неиспольз. клетку из четырех вокруг той, в которой попали по кораблю
    void updateStateAfterMiss(); // обновить бот после промаха, снова рандом. или если он попал а вотрой раз не попал, должен следующий раз сменить направление выстрела
    void updateStateAfterHit(const Dot& hit); // что делаем после попадания, стратегия след выстрела
    Dot getNextDirection(); // возвращает следующее направление по кругу вверх - вправо - вниз - влево
};

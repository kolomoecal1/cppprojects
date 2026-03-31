#pragma once
#include "bot.h"

class SmartBot : public Bot 
{
private:
    enum class State 
	{
        RANDOM,          
        HIT,              
<<<<<<< HEAD
    };
    
    State state = State::RANDOM;
    Dot lastHit;                 
    Dir currentDir;             
    std::vector<Dot> task;
    
=======
        SEARCHING,       
        BILATERAL // хотим найти противоположное направление
    };
    
    State state = State::RANDOM;
    Dot lastHit;          
    Dot firstHit;         
    Dir currentDir;     
    int dirIndex;         
    std::vector<Dot> hitCells;
    std::vector<std::vector<bool>> usedShots; 
>>>>>>> 1bbf94d8bf81a7720985adced9869e361c63ea1c
    
public:
    SmartBot();
    Dot hit() override;
    void reset();
    
private:
<<<<<<< HEAD
    Dot randomShot(); //генерим случ коорд 
    Dot searchAround(const Dot& dot); //ищет случайную неиспольз. клетку из четырех вокруг той, в которой попали по кораблю
    void updateStateAfterMiss(); // обновить бот после промаха, снова рандом. или если он попал а вотрой раз не попал, должен следующий раз сменить направление выстрела
    void updateStateAfterHit(const Dot& hit); // что делаем после попадания, стратегия след выстрела
=======
    Dot randomShot(); //генерим случ коорд и проверяем стреляли в них или нет
    Dot searchAround(const Dot& dot); //ищет случайную неиспольз. клетку из четырех вокруг той, в которой попали по кораблю
    Dot continueDirection(const Dot& start, const Dir& dir, bool forward); // вычисляет следующую клетку в заданном направлении вперед или назад
    bool isInBounds(const Dot& dot) const; // клетка вообще в пределах поля?
    bool isUsed(const Dot& dot) const; // проверяет, стреляли ли уже в эту клетку
    void markUsed(const Dot& dot); // отмечает клетку как обстрелянную в матрице выстрелов
    void updateStateAfterMiss(); // обновить бот после промаха, снова рандом. или если он попал а вотрой раз не попал, должен следующий раз сменить направление выстрела
    void updateStateAfterHit(const Dot& hit); // что делаем после попадания, стратегия след выстрела
    bool checkSink(); // все ли клетки корабля поражены?
>>>>>>> 1bbf94d8bf81a7720985adced9869e361c63ea1c
    Dot getNextDirection(); // возвращает следующее направление по кругу вверх - вправо - вниз - влево
};
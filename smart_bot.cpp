#include "smart_bot.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

// КОНСТРУКТОР
SmartBot::SmartBot() 
    : state(State::RANDOM)
    , lastHit({-1, -1})
    , currentDir({0, 0})
{
    // Инициализируем генератор случайных чисел (один раз)
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }
}

// ПРИВАТНЫЕ МЕТОДЫ

// Генерирует случайный выстрел
Dot SmartBot::randomShot() {
    int x = rand() % FIELD::SIZE;
    int y = rand() % FIELD::SIZE;
    return {x, y};
}

// Ищет случайную непроверенную клетку из четырёх вокруг заданной после попадания
Dot SmartBot::searchAround(const Dot& dot) {
    std::vector<Dir> directions = {
        {-1, 0},  // ВВЕРХ
        {0, 1},   // ВПРАВО
        {1, 0},   // ВНИЗ
        {0, -1}   // ВЛЕВО
    };
    
    // Перемешиваем направления 
    for (int i = 0; i < 4; ++i) {
        int j = rand() % 4;
        std::swap(directions[i], directions[j]);
    }
    
    // Проверяем каждое направление
    for (const auto& dir : directions) {
        int newX = dot.first + dir.first;
        int newY = dot.second + dir.second;
        
        // Проверяем, что координаты в пределах поля
        if (newX >= 0 && newX < FIELD::SIZE && 
            newY >= 0 && newY < FIELD::SIZE) {
            return {newX, newY};
        }
    }
    
    return randomShot();
}

// Обновляет бота после промаха
// Меняет направление или возвращается в режим RANDOM
void SmartBot::updateStateAfterMiss() {
    if (state == State::HIT) {
        // Меняем направление на противоположное
        // (вверх -> вниз, вправо -> влево)
        currentDir.first = -currentDir.first;
        currentDir.second = -currentDir.second;
        
        // Проверяем, можем ли выстрелить в новом направлении
        int newX = lastHit.first + currentDir.first;
        int newY = lastHit.second + currentDir.second;
        
        // Если выходим за пределы, возвращаемся к случайному поиску
        if (newX < 0 || newX >= FIELD::SIZE || 
            newY < 0 || newY >= FIELD::SIZE) {
            state = State::RANDOM;
            currentDir = {0, 0};
        }
    }
}

// Обновляет бота после попадания
// Запоминает координаты и переходит в режим HIT
void SmartBot::updateStateAfterHit(const Dot& hit) {
    lastHit = hit;
    state = State::HIT;
    
    // Если направление ещё не задано, выбираем случайное
    if (currentDir.first == 0 && currentDir.second == 0) {
        int dirIndex = rand() % 4;
        switch (dirIndex) {
            case 0: currentDir = {-1, 0}; break;  // ВВЕРХ
            case 1: currentDir = {0, 1}; break;   // ВПРАВО
            case 2: currentDir = {1, 0}; break;   // ВНИЗ
            case 3: currentDir = {0, -1}; break;  // ВЛЕВО
        }
    }
}

// Возвращает следующую клетку в текущем направлении от последнего попадания
// Используется в режиме HIT для последовательных выстрелов
Dot SmartBot::getNextDirection() {
    int newX = lastHit.first + currentDir.first;
    int newY = lastHit.second + currentDir.second;
    
    // Проверяем, что координаты в пределах поля
    if (newX >= 0 && newX < FIELD::SIZE && 
        newY >= 0 && newY < FIELD::SIZE) {
        return {newX, newY};
    }
    
    // Если вышли за пределы, меняем направление на противоположное
    currentDir.first = -currentDir.first;
    currentDir.second = -currentDir.second;
    
    newX = lastHit.first + currentDir.first;
    newY = lastHit.second + currentDir.second;
    
    // Если и так выходит за пределы, возвращаем случайный выстрел
    if (newX < 0 || newX >= FIELD::SIZE || 
        newY < 0 || newY >= FIELD::SIZE) {
        state = State::RANDOM;
        currentDir = {0, 0};
        return randomShot();
    }
    
    return {newX, newY};
}

// ============================================================================
// ПУБЛИЧНЫЕ МЕТОДЫ
// ============================================================================

// Основной метод выстрела
// Возвращает координаты для выстрела в зависимости от текущего состояния
Dot SmartBot::hit() {
    Dot result;
    
    switch (state) {
        case State::RANDOM:
            // Случайный поиск нового корабля
            result = randomShot();
            break;
            
        case State::HIT:
            // Добивание подбитого корабля
            result = getNextDirection();
            break;
    }
    
    return result;
}

// Передаёт боту результат выстрела
// Вызывается из Game после каждого выстрела
void SmartBot::setResult(CELL::SHOTRESULTS result, const Dot& shot) {
    switch (result) {
        case CELL::HIT:
            // Попали, но не потопили
            updateStateAfterHit(shot);
            break;
            
        case CELL::SINK:
            // Потопили корабль
            reset();  // Сбрасываем состояние, ищем новый корабль
            break;
            
        case CELL::MISS:
            // Промах
            updateStateAfterMiss();
            break;
            
        default:
            break;
    }
}

// Сбрасывает состояние бота для новой игры
void SmartBot::reset() {
    state = State::RANDOM;
    lastHit = {-1, -1};
    currentDir = {0, 0};
    task.clear();
}

// Метод расстановки кораблей (наследуем от Bot)
std::pair<Dot, Dir> SmartBot::place() {
    // Простая случайная расстановка
    int dirIndex = rand() % FIELD::COUNT;
    Dir dir = FIELD::dirs[dirIndex];
    Dot dot{ rand() % FIELD::SIZE, rand() % FIELD::SIZE };
    
    return {dot, dir};
}
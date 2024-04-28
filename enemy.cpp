#include <iostream>
#include "enemy.h"
#include "raylib.h"

Zombie::Zombie()
{
    int zombiePosX = GetRandomValue(200,400);
    int zombiePosY = GetRandomValue(200,400);
    Vector2 zombie = {zombiePosX, zombiePosY};

}

void Zombie::movement()
{
    
}
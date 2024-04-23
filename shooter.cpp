#include <iostream>
#include <cmath>
#include <vector>
#include <raylib.h>

using namespace std;


struct Bullet
{
    Vector2 position;
    Vector2 direction;
    Color WHITE;
    bool active;
};

void move(float& zombieX, float& zombieY, float playerX, float playerY)
{
    float relativePositionX = playerX - zombieX;
    float relativePositionY = playerY - zombieY;

    float magnitude = sqrt(relativePositionX*relativePositionX + relativePositionY*relativePositionY);
    if (magnitude != 0)
    {
        relativePositionX /= magnitude;
        relativePositionY /= magnitude;
    }

    float speed = 2;
    zombieX += speed*relativePositionX;
    zombieY += speed*relativePositionY;
    


}

Vector2 shoot(Vector2 mouse, float playerX, float playerY)
{   
    float relativePositionX = playerX - mouse.x;
    float relativePositionY = playerY - mouse.y;
    float aimLine = sqrt(relativePositionX*relativePositionX + relativePositionY*relativePositionY);
    if (aimLine != 0)
    {
        relativePositionX /= aimLine;
        relativePositionY /= aimLine;
        
    }
    Vector2 bullet = {playerX,playerY};
    return bullet;
}

int main()
{   
    InitWindow(800, 800, "shooter");

    vector<Bullet> bullets;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int movementSpeed = 5;
    int zombiePosX = GetRandomValue(200,400);
    int zombiePosY = GetRandomValue(200,400);
    
    Vector2 zombie = {zombiePosX, zombiePosY};
    Rectangle player = {screenWidth/2, screenHeight/2, 50, 50};
    int zombieHealth = 20;


    
    
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {   
        if (IsKeyDown(KEY_W))
        {
            player.y -= movementSpeed;
        }
        else if (IsKeyDown(KEY_S))
        {
            player.y += movementSpeed;
        }

        if (IsKeyDown(KEY_A))
        {
            player.x -= movementSpeed;
        }
        else if (IsKeyDown(KEY_D))
        {
            player.x += movementSpeed;
        }

        move(zombie.x, zombie.y, player.x, player.y);

        Vector2 mousePos = {GetMousePosition()};
        Vector2 playerPos = {player.x+player.width/2, player.y+player.height/2};
        Vector2 direction = {mousePos.x - player.x, mousePos.y - player.y};
        float magnitude = sqrt(direction.x*direction.x + direction.y*direction.y);
        if (magnitude != 0)
        {
            direction = {direction.x/magnitude, direction.y/magnitude};
        }


        float relativePositionX;
        float relativePositionY;
        float aimLine;
        Vector2 bullet;
        float speed = 2;
        
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {   
            Bullet newBullet;
            mousePos = GetMousePosition();
            newBullet.position = playerPos;

            newBullet.direction.x =  mousePos.x-player.x;
            newBullet.direction.y =  mousePos.y-player.y;

            float magnitude = sqrt(newBullet.direction.x * newBullet.direction.x + newBullet.direction.y * newBullet.direction.y);
            if (magnitude != 0) 
            {
                newBullet.direction.x /= magnitude;
                newBullet.direction.y /= magnitude;
            }

            newBullet.direction.x *= speed; 
            newBullet.direction.y *= speed;

            bullets.push_back(newBullet);
        
        }
        for (size_t i = 0; i < bullets.size(); ++i) 
        {
            bullets[i].position.x += bullets[i].direction.x;
            bullets[i].position.y += bullets[i].direction.y;
        }
        for (int i = 0; i < bullets.size(); ++i)
        {
            bool hit = CheckCollisionCircles(zombie, 25, bullets[i].position, 5);
            if (hit)
            {
                bullets.erase(bullets.begin()+i);
                zombieHealth--;
                cout << zombieHealth <<endl;
            }
        }

        BeginDrawing();
        
        for (const auto& bullet : bullets) 
        {
            DrawCircleV(bullet.position, 5, WHITE);
        }
        
        DrawRectangleRec(player, RED);
        DrawCircleV(zombie, 25, BLUE);
        DrawLineV(playerPos, mousePos, WHITE); 
        EndDrawing();
        ClearBackground(BLACK);
    }
    CloseWindow();
    return 0;
}
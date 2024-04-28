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

struct Zombie
{
    Vector2 position;
    Vector2 direction;
    Color BLUE;
    bool active;
    int maxHealth;
    int currentHealth;
    int radius = 25;
    int widht = radius*2;
    int height = radius*2;
};

struct Giant
{
    Vector2 position;
    Vector2 direction;
    Color BLUE;
    bool active;
    int maxHealth;
    int currentHealth;
    int radius = 50;
    int widht = radius*2;
    int height = radius*2;
};


struct healthBar
{
    Vector2 position;
    Color WHITE;
    bool active;
    int health;
    int width = health*10;
    int height = 10;
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

void spawnZombies(vector<Zombie>& zombies, int screenWidth, int screenHeight)
{
    Zombie newZombie;
            newZombie.position = {GetRandomValue(0,screenWidth), GetRandomValue(0,screenHeight)};
            newZombie.maxHealth = 10;
            newZombie.currentHealth = newZombie.maxHealth;
            zombies.push_back(newZombie);
}

void spawnGiant(vector<Giant>& giants, int screenWidth, int screenHeight)
{
    Giant newGiant;
    newGiant.position ={GetRandomValue(0,screenWidth), GetRandomValue(0,screenHeight)}; 
    newGiant.maxHealth = 100;
    newGiant.currentHealth = newGiant.maxHealth;
    giants.push_back(newGiant);

}




int main()
{   
    InitWindow(1920, 1080, "shooter");

    vector<Bullet> bullets;
    vector<Zombie> zombies;
    vector<Giant> giants;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int movementSpeed = 5;
    float timer = 0;
    float spawnInterval = 5.0f;
    int score = 0;

    
    Rectangle player = {screenWidth/2, screenHeight/2, 50, 50};
    // Rectangle healthBar = {zombies[0].position.x, zombies[0].position.y + 25, 50, 50};


    
    
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


        timer += GetFrameTime();
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
        float speed = 5;

        
        
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

        if (IsKeyPressed(KEY_O)||timer >= spawnInterval)
        {
            spawnZombies(zombies, screenWidth, screenHeight);
            timer = 0;

        }
        for (int i = 0; i < bullets.size(); ++i)
        {
            for (int j = 0; j < zombies.size(); ++j)
            {
                bool zombiehit = CheckCollisionCircles(zombies[j].position, 25, bullets[i].position, 5);
                if (zombiehit)
                    {
                        bullets.erase(bullets.begin()+i);
                        zombies[j].currentHealth -= 1;
                    }
                    
            }

            for (int k = 0; k < giants.size(); ++k)
            { 
                    bool gianthit = CheckCollisionCircles(giants[k].position, 50, bullets[i].position, 5);
                    if (gianthit)
                    {
                        bullets.erase(bullets.begin()+i);
                        giants[k].currentHealth -= 1;
                    }

            }
        }

        if (IsKeyPressed(KEY_P))
        {
            spawnGiant(giants, screenWidth, screenHeight);
        }

        BeginDrawing();
        
        for (const auto& bullet : bullets) 
        {
            DrawCircleV(bullet.position, 5, WHITE);
        }

        for (const auto& zombie : zombies)
        {   
            float healtPercentage = float(zombie.currentHealth)/zombie.maxHealth;
            float healthBarWidth = 50*healtPercentage;



            DrawCircleV(zombie.position, 25, zombie.Color);
            DrawRectangle(zombie.position.x - zombie.widht/2, zombie.position.y - 45, 50, 10, GRAY);
            DrawRectangle(zombie.position.x - zombie.widht/2, zombie.position.y - 45, healthBarWidth, 10, RED);
            DrawText(to_string(zombie.currentHealth).c_str(), zombie.position.x - 3 , zombie.position.y - 45, 6, WHITE);
            
        }
        for (int i = 0; i < zombies.size(); ++i)
        {
            move(zombies[i].position.x, zombies[i].position.y, player.x, player.y);
            if (zombies[i].currentHealth <= 0)
            {   
                zombies.erase(zombies.begin()+i);
                ++score;
            }

            
        }

        for (const auto& giant : giants)
        {
            float healtPercentage = float(giant.currentHealth)/giant.maxHealth;
            float healtbarWidth = 100*healtPercentage;

            DrawCircleV(giant.position, 50, GREEN);
            DrawRectangle(giant.position.x - giant.widht/2, giant.position.y - (giant.radius+10), 100, 10, GRAY);
            DrawRectangle(giant.position.x - giant.widht/2, giant.position.y - (giant.radius+10), healtbarWidth, 10, RED);
            DrawText(to_string(giant.currentHealth).c_str(), giant.position.x - 3 , giant.position.y - (giant.radius+10), 6, WHITE);
        }
        for (int i = 0; i < giants.size(); ++i)
        {
            move(giants[i].position.x, giants[i].position.y, player.x, player.y);
            if (giants[i].currentHealth <= 0)
            {   
                giants.erase(giants.begin()+i);
                ++score;
            }
        }



        DrawText(to_string(score).c_str(), screenWidth/2, 40, 20, WHITE);
        DrawRectangleRec(player, RED);
        DrawLineV(playerPos, mousePos, WHITE); 
        EndDrawing();
        ClearBackground(BLACK);
    }
    CloseWindow();
    return 0;
}
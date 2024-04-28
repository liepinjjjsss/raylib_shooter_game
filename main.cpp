#include <iostream>
#include <string>
#include <sstream>
#include "raylib.h"

using namespace std;
int main() {
    InitWindow(800, 800, "Hello world");
    int screenWidth = GetRenderWidth();
    int screenHeight = GetRenderHeight();

    int posX = screenWidth/2;
    int posY = screenHeight/2;

    cout << screenWidth << endl;
    cout << screenHeight << endl;

    int pointPosX = GetRandomValue(0, screenWidth);
    int pointPosY = GetRandomValue(0, screenHeight);

    Rectangle rectangle ={posX,posY, 50, 50};

    float gravity = 0.5f;
    float velocityY = 0;
    float velocityX = 3;
    int counter = 0;
    
    string text = "Score: ";
    
    
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        string score = text + to_string(counter);

        BeginDrawing();
        DrawRectangleRec(rectangle, RED);
        DrawCircle(pointPosX, pointPosY, 20, BLUE); 
        DrawText(score.c_str(), posX, 100, 32, WHITE);
        EndDrawing();

        velocityY += gravity;
        rectangle.y += velocityY;
        rectangle.x += velocityX; 

        ClearBackground(BLACK);
        cout << rectangle.x <<":" << rectangle.y <<endl;
        // cout << counter << endl;
        cout << score << endl;

        if (IsKeyPressed(KEY_SPACE)){
            velocityY += -20;
        }
        if (rectangle.x >= screenWidth-100) {
            velocityX = -velocityX;
        }else if (rectangle.x <= screenWidth-(screenWidth-100)) {
            velocityX = -velocityX;
        }

        
        bool collision = CheckCollisionCircleRec(Vector2{pointPosX, pointPosY}, 20, rectangle);

        if (collision) {
                cout << "Collision detected" << endl;
                pointPosX = GetRandomValue(0, screenWidth);
                pointPosY = GetRandomValue(0, screenHeight);
                counter++;
        }
        
    }

    CloseWindow();
    return 0;
}
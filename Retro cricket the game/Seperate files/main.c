#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include<stdbool.h>
#include<stdio.h>
typedef enum GameScreen {
    SCREEN_MAIN_MENU,
    SCREEN_FLAG_SELECTION,
    SCREEN_TOSS,
    SCREEN_TOSS_RESULT,
    SCREEN_CHOICE,
    SCREEN_GAME_START
} GameScreen;

int main(void) {
    const int screenWidth = 1600;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Retro Cricket");

    InitAudioDevice();

        Texture2D background = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/backgr2.png");
    Texture2D logo = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/test.png");
    Texture2D startButtonSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/start.png");

    Texture2D flag1 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/image.png");
    Texture2D flag2 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/india.png");
    Texture2D flag3 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/new zeeland.png");
    Texture2D flag4 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/Australia.png");

    Texture2D tossSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/tos.png");
    Texture2D headButtonSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/heads.png");
    Texture2D tailButtonSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/tails.png");
    Texture2D headWinsSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/headwins.png");
    Texture2D tailWinsSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/tailwins.png");
    
    Texture2D youWonTossSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/wintoss.png");
    Texture2D youLostTossSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/losstoss.png");
    Texture2D youWillBatSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/ballfirst.png");
    Texture2D youWillBowlSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/batfirst.png");
    Texture2D chooseBatOrBowlSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/choose.png");
    Texture2D batButtonSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/BAT.png");
    Texture2D bowlButtonSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/BALL.png");

    Sound backgroundMusic = LoadSound("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/backmusic.mp3");
    Sound clickSound = LoadSound("C:/raylib/raylib/projects/Notepad++/Retrocricket/assets/clicksound.wav");

    PlaySound(backgroundMusic);

    Rectangle startButtonBounds = { screenWidth / 2 - startButtonSprite.width / 2, 380, startButtonSprite.width, startButtonSprite.height };

    Rectangle flagBounds[4] = {
        { 325, 300, flag1.width, flag1.height },
        { 575, 300, flag2.width, flag2.height },
        { 825, 300, flag3.width, flag3.height },
        { 1075, 300, flag4.width, flag4.height }
    };

    Rectangle headButtonBounds = { screenWidth / 2 - headButtonSprite.width / 2 - 150, screenHeight / 2, headButtonSprite.width, headButtonSprite.height };
    Rectangle tailButtonBounds = { screenWidth / 2 + 50, screenHeight / 2, tailButtonSprite.width, tailButtonSprite.height };

    int buttonGap = 50;
    Rectangle batButtonBounds = { 
        screenWidth / 2 - batButtonSprite.width - buttonGap / 2, 
        screenHeight / 2 + 50, 
        batButtonSprite.width, 
        batButtonSprite.height 
    };
    Rectangle bowlButtonBounds = { 
        screenWidth / 2 + buttonGap / 2, 
        screenHeight / 2 + 50, 
        bowlButtonSprite.width, 
        bowlButtonSprite.height 
    };

    GameScreen currentScreen = SCREEN_MAIN_MENU;
    bool flagSelected = false;
    int selectedFlag = -1;

    bool isTossing = false;
    bool tossResult = false;
    int playerChoice = -1;
    float coinRotation = 0.0f;
    float flipSpeed = 10.0f;
    Vector2 coinPosition = { screenWidth / 2.0f, screenHeight / 2.0f - 50 };

    float tossResultTimer = 0.0f;
    bool showTossResult = false;
    bool userWonToss = false;
    int gameChoice = -1;

    // Animation variables
    float batButtonScale = 1.0f;
    float bowlButtonScale = 1.0f;
    float animationSpeed = 0.05f;

    // Music stop timer
    float musicStopTimer = 0.0f;
    bool musicStopped = false;

    srand(time(NULL)); 
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector2 mousePosition = GetMousePosition();

        float startButtonScale = 1.0f;
        bool startButtonHovered = CheckCollisionPointRec(mousePosition, startButtonBounds);
        if (startButtonHovered) {
            startButtonScale = 1.1f;
        }

        float flagScale[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        for (int i = 0; i < 4; i++) {
            if (CheckCollisionPointRec(mousePosition, flagBounds[i])) {
                flagScale[i] = 1.1f;
            }
        }

        // Bat and Bowl button animations
        bool batButtonHovered = CheckCollisionPointRec(mousePosition, batButtonBounds);
        bool bowlButtonHovered = CheckCollisionPointRec(mousePosition, bowlButtonBounds);

        if (batButtonHovered) {
            batButtonScale += animationSpeed;
            if (batButtonScale > 1.1f) batButtonScale = 1.1f;
        } else {
            batButtonScale -= animationSpeed;
            if (batButtonScale < 1.0f) batButtonScale = 1.0f;
        }

        if (bowlButtonHovered) {
            bowlButtonScale += animationSpeed;
            if (bowlButtonScale > 1.1f) bowlButtonScale = 1.1f;
        } else {
            bowlButtonScale -= animationSpeed;
            if (bowlButtonScale < 1.0f) bowlButtonScale = 1.0f;
        }

        if (currentScreen == SCREEN_MAIN_MENU) {
            if (startButtonHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                PlaySound(clickSound);
                currentScreen = SCREEN_FLAG_SELECTION;
            }
        } else if (currentScreen == SCREEN_FLAG_SELECTION) {
            for (int i = 0; i < 4; i++) {
                if (CheckCollisionPointRec(mousePosition, flagBounds[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    PlaySound(clickSound);
                    selectedFlag = i;
                    flagSelected = true;
                    currentScreen = SCREEN_TOSS;
                    flipSpeed = 10.0f;
                }
            }
        } else if (currentScreen == SCREEN_TOSS) {
            if (playerChoice == -1) {
                if (CheckCollisionPointRec(mousePosition, headButtonBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    PlaySound(clickSound);
                    playerChoice = 0;
                    isTossing = true;
                    tossResult = (rand() % 2) == 0;
                }
                if (CheckCollisionPointRec(mousePosition, tailButtonBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    PlaySound(clickSound);
                    playerChoice = 1;
                    isTossing = true;
                    tossResult = (rand() % 2) == 0;
                }
            } else if (isTossing) {
                coinRotation += flipSpeed;
                if (flipSpeed > 0.1f) {
                    flipSpeed *= 0.98f;
                } else {
                    isTossing = false;
                    tossResultTimer = 3.0f;
                    if ((tossResult && (int)(coinRotation / 45) % 2 != 0) ||
                        (!tossResult && (int)(coinRotation / 45) % 2 == 0)) {
                        coinRotation += 45.0f;
                    }
                    currentScreen = SCREEN_TOSS_RESULT;
                    userWonToss = (playerChoice == 0 && tossResult) || (playerChoice == 1 && !tossResult);
                }
            }
        } else if (currentScreen == SCREEN_TOSS_RESULT) {
            tossResultTimer -= GetFrameTime();
            if (tossResultTimer <= 0.0f) {
                if (userWonToss) {
                    currentScreen = SCREEN_CHOICE;
                } else {
                    gameChoice = rand() % 2;
                    currentScreen = SCREEN_GAME_START;
                    musicStopTimer = 0.0f;  // Start the music stop timer
                }
            }
        } else if (currentScreen == SCREEN_CHOICE) {
            if (batButtonHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                PlaySound(clickSound);
                gameChoice = 0;
                currentScreen = SCREEN_GAME_START;
                musicStopTimer = 0.0f;  // Start the music stop timer
            }
            if (bowlButtonHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                PlaySound(clickSound);
                gameChoice = 1;
                currentScreen = SCREEN_GAME_START;
                musicStopTimer = 0.0f;  // Start the music stop timer
            }
        } else if (currentScreen == SCREEN_GAME_START) {
            if (!musicStopped) {
                musicStopTimer += GetFrameTime();
                if (musicStopTimer >= 4.0f) {
                    StopSound(backgroundMusic);
                    musicStopped = true;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(background, 0, 0, WHITE);

        if (currentScreen == SCREEN_MAIN_MENU) {
            DrawTexture(logo, screenWidth / 2 - logo.width / 2, 50, WHITE);
            DrawTextureEx(startButtonSprite, (Vector2){ startButtonBounds.x, startButtonBounds.y }, 0.0f, startButtonScale, WHITE);
        } else if (currentScreen == SCREEN_FLAG_SELECTION) {
            DrawText("Choose Country", screenWidth / 2 - MeasureText("Choose Country", 40) / 2, 50, 50, RED);

            for (int i = 0; i < 4; i++) {
                DrawTextureEx(i == 0 ? flag1 : (i == 1 ? flag2 : (i == 2 ? flag3 : flag4)), (Vector2){flagBounds[i].x, flagBounds[i].y}, 0.0f, flagScale[i], WHITE);
                const char *flagName = (i == 0) ? "Pakistan" : (i == 1) ? "India" : (i == 2) ? "New Zealand" : "Australia";
                DrawText(flagName, flagBounds[i].x + flagBounds[i].width / 2 - MeasureText(flagName, 20) / 2, flagBounds[i].y + flagBounds[i].height + 10, 24, BLACK );
            }
        } else if (currentScreen == SCREEN_TOSS) {
            if (playerChoice == -1) {
                DrawTexture(tossSprite, screenWidth / 2 - tossSprite.width / 2, 50, WHITE);
                DrawTexture(headButtonSprite, headButtonBounds.x, headButtonBounds.y, WHITE);
                DrawTexture(tailButtonSprite, tailButtonBounds.x, tailButtonBounds.y, WHITE);
            } else if (isTossing) {
                float coinRadius = 100.0f;
                DrawCircleV(coinPosition, coinRadius + 2, DARKGRAY);
                DrawCircleV(coinPosition, coinRadius, YELLOW);
                const char *text = ((int)(coinRotation / 45) % 2 == 0) ? "Head" : "Tail";
                Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 50, 1);
                DrawTextEx(GetFontDefault(), text,
                           (Vector2){ coinPosition.x - textSize.x / 2, coinPosition.y - textSize.y / 2 },
                           50, 1, BLACK);
            }
        } else if (currentScreen == SCREEN_TOSS_RESULT) {
            if (tossResult) {
                DrawTexture(headWinsSprite, screenWidth / 2 - headWinsSprite.width / 2, screenHeight / 2 - headWinsSprite.height / 2, WHITE);
            } else {
                DrawTexture(tailWinsSprite, screenWidth / 2 - tailWinsSprite.width / 2, screenHeight / 2 - tailWinsSprite.height / 2, WHITE);
            }
            float messageYPosition = screenHeight / 2 + 100;
            if (userWonToss) {
                DrawTexture(youWonTossSprite, screenWidth / 2 - youWonTossSprite.width / 2, messageYPosition, WHITE);
            } else {
                DrawTexture(youLostTossSprite, screenWidth / 2 - youLostTossSprite.width / 2, messageYPosition, WHITE);
            }
        } else if (currentScreen == SCREEN_CHOICE) {
            DrawTexture(chooseBatOrBowlSprite, screenWidth / 2 - chooseBatOrBowlSprite.width / 2, screenHeight / 2 - chooseBatOrBowlSprite.height - 100, WHITE);
            DrawTextureEx(batButtonSprite, (Vector2){ batButtonBounds.x, batButtonBounds.y }, 0.0f, batButtonScale, WHITE);
            DrawTextureEx(bowlButtonSprite, (Vector2){ bowlButtonBounds.x, bowlButtonBounds.y }, 0.0f, bowlButtonScale, WHITE);
        } else if (currentScreen == SCREEN_GAME_START) {
            if (gameChoice == 0) {
                DrawTexture(youWillBatSprite, screenWidth / 2 - youWillBatSprite.width / 2, screenHeight / 2 - youWillBatSprite.height / 2, WHITE);
            } else {
                DrawTexture(youWillBowlSprite, screenWidth / 2 - youWillBowlSprite.width / 2, screenHeight / 2 - youWillBowlSprite.height / 2, WHITE);
            }
        }

        EndDrawing();
    }

    StopSound(backgroundMusic);
    
    UnloadTexture(background);
    UnloadTexture(logo);
    UnloadTexture(startButtonSprite);
    UnloadTexture(flag1);
    UnloadTexture(flag2);
    UnloadTexture(flag3);
    UnloadTexture(flag4);
    UnloadTexture(tossSprite);
    UnloadTexture(headButtonSprite);
    UnloadTexture(tailButtonSprite);
    UnloadTexture(headWinsSprite);
    UnloadTexture(tailWinsSprite);
    UnloadTexture(youWonTossSprite);
    UnloadTexture(youLostTossSprite);
    UnloadTexture(youWillBatSprite);
    UnloadTexture(youWillBowlSprite);
    UnloadTexture(chooseBatOrBowlSprite);
    UnloadTexture(batButtonSprite);
    UnloadTexture(bowlButtonSprite);
    UnloadSound(backgroundMusic);
    UnloadSound(clickSound);
    CloseWindow();

    return 0;
}
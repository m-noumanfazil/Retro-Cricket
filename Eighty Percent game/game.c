#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

typedef enum GameScreen {
    SCREEN_MAIN_MENU,
    SCREEN_FLAG_SELECTION,
    SCREEN_TOSS,
    SCREEN_TOSS_RESULT,
    SCREEN_CHOICE,
    SCREEN_GAME_START,
    SCREEN_INNINGS
} GameScreen;

// Global Variables
const int screenWidth = 1600;
const int screenHeight = 800;

Texture2D background;
Texture2D logo;
Texture2D startButtonSprite;
Texture2D flag1, flag2, flag3, flag4;
Texture2D tossSprite, headButtonSprite, tailButtonSprite;
Texture2D headWinsSprite, tailWinsSprite;
Texture2D youWonTossSprite, youLostTossSprite;
Texture2D youWillBatSprite, youWillBowlSprite;
Texture2D chooseBatOrBowlSprite;
Texture2D batButtonSprite, bowlButtonSprite;
Texture2D button1, button2, button3, button4, button6;
Texture2D handGestures[6];

Sound backgroundMusic, clickSound, six, wicket, cheer;

float buttonScale = 0.2f;
float hoverScale = 0.15f;

Vector2 button1Position, button2Position, button3Position, button4Position, button6Position;

int userScore = 0;
int aiScore = 0;
int lastUserScore = 0;
int wickets = 0;
int totalScore = 0;
int target = 0;
bool youWillFirstBat = false;

int userGesture = -1;
int aiGesture = -1;
bool isOut = false;
float outTimer = 0.0f;

void InitializeGame(void) {
    InitWindow(screenWidth, screenHeight, "Retro Cricket");
    InitAudioDevice();

    // Load textures
    background = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/backgr2.png");
    logo = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/test.png");
    startButtonSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/start.png");
    flag1 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/image.png");
    flag2 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/india.png");
    flag3 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/new zeeland.png");
    flag4 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/Australia.png");
    tossSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/tos.png");
    headButtonSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/heads.png");
    tailButtonSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/tails.png");
    headWinsSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/headwins.png");
    tailWinsSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/tailwins.png");
    youWonTossSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/wintoss.png");
    youLostTossSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/losstoss.png");
    youWillBatSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/ballfirst.png");
    youWillBowlSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/batfirst.png");
    chooseBatOrBowlSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/choose.png");
    batButtonSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/BAT.png");
    bowlButtonSprite = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/BALL.png");
    button1 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/1.png");
    button2 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/2.png");
    button3 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/3.png");
    button4 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/4.png");
    button6 = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/6.png");

    handGestures[0] = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/a1.png");
    handGestures[1] = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/a2.png");
    handGestures[2] = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/a3.png");
    handGestures[3] = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/a4.png");
    handGestures[4] = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/out.png");
    handGestures[5] = LoadTexture("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/a5.png");

    // Load sounds
    backgroundMusic = LoadSound("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/backmusic.mp3");
    clickSound = LoadSound("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/clicksound.wav");
    six = LoadSound("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/six.wav");
    wicket = LoadSound("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/wicket.wav");
    cheer = LoadSound("C:/raylib/raylib/projects/Notepad++/Retrocricket/assest/cheer.wav");

    // Calculate button positions
    int totalButtons = 5;
    float spacing = (screenWidth - (totalButtons * button1.width * buttonScale)) / (totalButtons + 1);
    button1Position = (Vector2){ spacing, screenHeight - (button1.height * buttonScale) - 50 };
    button2Position = (Vector2){ button1Position.x + button1.width * buttonScale + spacing, button1Position.y };
    button3Position = (Vector2){ button2Position.x + button2.width * buttonScale + spacing, button1Position.y };
    button4Position = (Vector2){ button3Position.x + button3.width * buttonScale + spacing, button1Position.y };
    button6Position = (Vector2){ button4Position.x + button4.width * buttonScale + spacing, button1Position.y };

    SetTargetFPS(60);
}

void Cleanup(void) {
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
    UnloadTexture(button1);
    UnloadTexture(button2);
    UnloadTexture(button3);
    UnloadTexture(button4);
    UnloadTexture(button6);
    for (int i = 0; i < 6; i++) {
        UnloadTexture(handGestures[i]);
    }
    UnloadSound(backgroundMusic);
    UnloadSound(clickSound);
    UnloadSound(six);
    UnloadSound(wicket);
    UnloadSound(cheer);
    CloseAudioDevice();
    CloseWindow();
}

int GetAIScore(int userLastScore) {
    int aiScore;
    int strategyChance = rand() % 100;

    do {
        if (strategyChance < 50) {
            aiScore = userLastScore + (rand() % 3 - 1);
            if (aiScore < 1) aiScore = 1;
            if (aiScore > 6) aiScore = 6;
        } else {
            aiScore = rand() % 6 + 1;
        }
    } while (aiScore == 5);

    return aiScore;
}

void PlayInnings(bool isUserBatting) {
    int innings = 1;
    bool cheerPlayed = false;
    bool gameEnded = false;

    while (!WindowShouldClose() && !gameEnded) {
        Vector2 mousePosition = GetMousePosition();
        float deltaTime = GetFrameTime();

        bool isButton1Hovered = (wickets < 5) && (CheckCollisionPointRec(mousePosition, (Rectangle){ button1Position.x, button1Position.y, button1.width * buttonScale, button1.height * buttonScale }) || IsKeyDown(KEY_ONE));
        bool isButton2Hovered = (wickets < 5) && (CheckCollisionPointRec(mousePosition, (Rectangle){ button2Position.x, button2Position.y, button2.width * buttonScale, button2.height * buttonScale }) || IsKeyDown(KEY_TWO));
        bool isButton3Hovered = (wickets < 5) && (CheckCollisionPointRec(mousePosition, (Rectangle){ button3Position.x, button3Position.y, button3.width * buttonScale, button3.height * buttonScale }) || IsKeyDown(KEY_THREE));
        bool isButton4Hovered = (wickets < 5) && (CheckCollisionPointRec(mousePosition, (Rectangle){ button4Position.x, button4Position.y, button4.width * buttonScale, button4.height * buttonScale }) || IsKeyDown(KEY_FOUR));
        bool isButton6Hovered = (wickets < 5) && (CheckCollisionPointRec(mousePosition, (Rectangle){ button6Position.x, button6Position.y, button6.width * buttonScale, button6.height * buttonScale }) || IsKeyDown(KEY_SIX));

        if (isButton1Hovered && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ONE))) {
            PlaySound(clickSound);
            lastUserScore = userScore = 1;
            userGesture = userScore - 1;
        }
        if (isButton2Hovered && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_TWO))) {
            PlaySound(clickSound);
            lastUserScore = userScore = 2;
            userGesture = userScore - 1;
        }
        if (isButton3Hovered && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_THREE))) {
            PlaySound(clickSound);
            lastUserScore = userScore = 3;
            userGesture = userScore - 1;
        }
        if (isButton4Hovered && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_FOUR))) {
            PlaySound(clickSound);
            lastUserScore = userScore = 4;
            userGesture = userScore - 1;
        }
        if (isButton6Hovered && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SIX))) {
            PlaySound(clickSound);
            lastUserScore = userScore = 6;
            PlaySound(six);
            userGesture = userScore - 1;
        }

        if (userScore > 0 && wickets < 5) {
            aiScore = GetAIScore(userScore);
            aiGesture = aiScore - 1;

            if (aiScore == userScore) {
                wickets++;
                PlaySound(wicket);
                isOut = true;
                outTimer = 2.0f;
            } else {
                if (innings == 1) {
                    totalScore += isUserBatting ? userScore : aiScore;
                } else {
                    totalScore += isUserBatting ? aiScore : userScore;
                }
            }
            userScore = 0;
        }

        if (isOut) {
            outTimer -= deltaTime;
            if (outTimer <= 0) {
                isOut = false;
                userGesture = -1;
                aiGesture = -1;
            }
        }

        if (innings == 1 && wickets == 5) {
            target = totalScore + 1;

            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("End of 1st Innings. Starting 2nd Innings...", screenWidth / 2 - MeasureText("End of 1st Innings. Starting 2nd Innings...", 30) / 2, screenHeight / 2, 30, DARKBLUE);
            DrawText(TextFormat("Target: %i", target), screenWidth / 2 - MeasureText(TextFormat("Target: %i", target), 30) / 2, screenHeight / 2 + 50, 30, DARKGREEN);
            EndDrawing();

            WaitTime(4.0);

            innings = 2;
            wickets = 0;
            totalScore = 0;
            userScore = 0;
            lastUserScore = 0;
            aiScore = 0;
            userGesture = -1;
            aiGesture = -1;
            isOut = false;
        }

        bool userWon = (innings == 2 && ((isUserBatting && totalScore >= target) || (!isUserBatting && wickets == 5)));
        bool aiWon = (innings == 2 && ((!isUserBatting && totalScore >= target) || (isUserBatting && wickets == 5)));

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, screenWidth / 2 - background.width / 2, screenHeight / 2 - background.height / 2, WHITE);

        float currentScale1 = isButton1Hovered ? hoverScale : buttonScale;
        float currentScale2 = isButton2Hovered ? hoverScale : buttonScale;
        float currentScale3 = isButton3Hovered ? hoverScale : buttonScale;
        float currentScale4 = isButton4Hovered ? hoverScale : buttonScale;
        float currentScale6 = isButton6Hovered ? hoverScale : buttonScale;

        DrawTextureEx(button1, button1Position, 0.0f, currentScale1, WHITE);
        DrawTextureEx(button2, button2Position, 0.0f, currentScale2, WHITE);
        DrawTextureEx(button3, button3Position, 0.0f, currentScale3, WHITE);
        DrawTextureEx(button4, button4Position, 0.0f, currentScale4, WHITE);
        DrawTextureEx(button6, button6Position, 0.0f, currentScale6, WHITE);

        DrawText(innings == 1 ? "1st Innings" : "2nd Innings", screenWidth / 2 - MeasureText(innings == 1 ? "1st Innings" : "2nd Innings", 30) / 2, 20, 30, DARKBLUE);
        DrawText(TextFormat("Last Entered Score: %i", lastUserScore), 50, 60, 30, DARKGREEN);
        DrawText(TextFormat("AI Score: %i", aiScore), screenWidth - 400, 60, 30, DARKGREEN);
        DrawText(TextFormat("Total Score: %i", totalScore), screenWidth / 2 - MeasureText(TextFormat("Total Score: %i", totalScore), 30) / 2, 60, 30, DARKGREEN);
        DrawText(TextFormat("Wickets: %i", wickets), screenWidth / 2 - MeasureText(TextFormat("Wickets: %i", wickets), 30) / 2, 100, 30, DARKGREEN);

        if (innings == 2) {
            DrawText(TextFormat("Target: %i", target), screenWidth / 2 - MeasureText(TextFormat("Target: %i", target), 30) / 2, 140, 30, DARKGREEN);
        }

        if (aiWon) {
            int margin = target - totalScore;
            DrawText(TextFormat("AI Won the Game by %i runs!", margin),
                     screenWidth / 2 - MeasureText(TextFormat("AI Won the Game by %i runs!", margin), 24) / 2,
                     screenHeight / 2 - 190,
                     30,
                     RED);
        } else if (userWon) {
            int remainingWickets = 5 - wickets;
            DrawText(TextFormat("User Won the Game by %i wickets!", remainingWickets),
                     screenWidth / 2 - MeasureText(TextFormat("User Won the Game by %i wickets!", remainingWickets), 26) / 2,
                     screenHeight / 2 - 190,
                     30,
                     DARKGREEN);
        }

        DrawTexture(flag1, 50, -20 + MeasureText(TextFormat("Wickets: %i", wickets), 30), WHITE);
        DrawTexture(flag2, screenWidth - flag2.width - 250, -20 + MeasureText(TextFormat("Wickets: %i", wickets), 30), WHITE);

        if (userGesture >= 0) {
            DrawTexture(handGestures[userGesture], screenWidth / 4 - handGestures[userGesture].width / 2, screenHeight / 2 - handGestures[userGesture].height / 2, WHITE);
        }
        if (aiGesture >= 0) {
            DrawTexture(handGestures[aiGesture], 3 * screenWidth / 4 - handGestures[aiGesture].width / 2, screenHeight / 2 - handGestures[aiGesture].height / 2, WHITE);
        }

        if (isOut) {
            DrawTexture(handGestures[4], screenWidth / 2 - handGestures[5].width / 2, screenHeight / 2 - handGestures[5].height / 2, WHITE);
        }

        EndDrawing();

        if ((userWon || aiWon) && !cheerPlayed) {
            PlaySound(cheer);
            cheerPlayed = true;
        }

        if ((userWon || aiWon) && cheerPlayed) {
            WaitTime(4.0);
            gameEnded = true;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            gameEnded = true;
        }
    }
}

int main(void) {
    InitializeGame();

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
    Rectangle batButtonBounds = { screenWidth / 2 - batButtonSprite.width - buttonGap / 2, screenHeight / 2 + 50, batButtonSprite.width, batButtonSprite.height };
    Rectangle bowlButtonBounds = { screenWidth / 2 + buttonGap / 2, screenHeight / 2 + 50, bowlButtonSprite.width, bowlButtonSprite.height };

    float musicStopTimer = 0.0f;
    bool musicStopped = false;

    PlaySound(backgroundMusic);

    while (!WindowShouldClose()) {
        Vector2 mousePosition = GetMousePosition();

        switch (currentScreen) {
            case SCREEN_MAIN_MENU:
                if (CheckCollisionPointRec(mousePosition, startButtonBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    PlaySound(clickSound);
                    currentScreen = SCREEN_FLAG_SELECTION;
                }
                break;

            case SCREEN_FLAG_SELECTION:
                for (int i = 0; i < 4; i++) {
                    if (CheckCollisionPointRec(mousePosition, flagBounds[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        PlaySound(clickSound);
                        selectedFlag = i;
                        flagSelected = true;
                        currentScreen = SCREEN_TOSS;
                        flipSpeed = 10.0f;
                    }
                }
                break;

            case SCREEN_TOSS:
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
                break;

            case SCREEN_TOSS_RESULT:
                tossResultTimer -= GetFrameTime();
                if (tossResultTimer <= 0.0f) {
                    if (userWonToss) {
                        currentScreen = SCREEN_CHOICE;
                    } else {
                        gameChoice = rand() % 2;
                        currentScreen = SCREEN_GAME_START;
                        musicStopTimer = 0.0f;
                    }
                }
                break;

            case SCREEN_CHOICE:
                if (CheckCollisionPointRec(mousePosition, batButtonBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    PlaySound(clickSound);
                    gameChoice = 0;
                    currentScreen = SCREEN_GAME_START;
                    musicStopTimer = 0.0f;
                }
                if (CheckCollisionPointRec(mousePosition, bowlButtonBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    PlaySound(clickSound);
                    gameChoice = 1;
                    currentScreen = SCREEN_GAME_START;
                    musicStopTimer = 0.0f;
                }
                break;

            case SCREEN_GAME_START:
                if (!musicStopped) {
                    musicStopTimer += GetFrameTime();
                    if (musicStopTimer >= 4.0f) {
                        StopSound(backgroundMusic);
                        musicStopped = true;
                        youWillFirstBat = (userWonToss && gameChoice == 0) || (!userWonToss && gameChoice == 1);
                        currentScreen = SCREEN_INNINGS;
                    }
                }
                break;

            case SCREEN_INNINGS:
                PlayInnings(youWillFirstBat);
                currentScreen = SCREEN_MAIN_MENU;
                break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);

        switch (currentScreen) {
            case SCREEN_MAIN_MENU:
                DrawTexture(logo, screenWidth / 2 - logo.width / 2, 50, WHITE);
                DrawTexture(startButtonSprite, startButtonBounds.x, startButtonBounds.y, WHITE);
                break;

            case SCREEN_FLAG_SELECTION:
                DrawText("Choose Country", screenWidth / 2 - MeasureText("Choose Country", 40) / 2, 50, 50, RED);
                for (int i = 0; i < 4; i++) {
                    DrawTexture(i == 0 ? flag1 : (i == 1 ? flag2 : (i == 2 ? flag3 : flag4)), flagBounds[i].x, flagBounds[i].y, WHITE);
                    const char *flagName = (i == 0) ? "Pakistan" : (i == 1) ? "India" : (i == 2) ? "New Zealand" : "Australia";
                    DrawText(flagName, flagBounds[i].x + flagBounds[i].width / 2 - MeasureText(flagName, 20) / 2, flagBounds[i].y + flagBounds[i].height + 10, 24, BLACK);
                }
                break;

            case SCREEN_TOSS:
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
                    DrawTextEx(GetFontDefault(), text, (Vector2){ coinPosition.x - textSize.x / 2, coinPosition.y - textSize.y / 2 }, 50, 1, BLACK);
                }
                break;

            case SCREEN_TOSS_RESULT:
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
                break;

            case SCREEN_CHOICE:
                DrawTexture(chooseBatOrBowlSprite, screenWidth / 2 - chooseBatOrBowlSprite.width / 2, screenHeight / 2 - chooseBatOrBowlSprite.height - 100, WHITE);
                DrawTexture(batButtonSprite, batButtonBounds.x, batButtonBounds.y, WHITE);
                DrawTexture(bowlButtonSprite, bowlButtonBounds.x, bowlButtonBounds.y, WHITE);
                break;

            case SCREEN_GAME_START:
                if (gameChoice == 0) {
                    DrawTexture(youWillBatSprite, screenWidth / 2 - youWillBatSprite.width / 2, screenHeight / 2 - youWillBatSprite.height / 2, WHITE);
                } else {
                    DrawTexture(youWillBowlSprite, screenWidth / 2 - youWillBowlSprite.width / 2, screenHeight / 2 - youWillBowlSprite.height / 2, WHITE);
                }
                break;

            case SCREEN_INNINGS:
                // The drawing for this screen is handled in the PlayInnings function
                break;
        }

        EndDrawing();
    }

    Cleanup();
    return 0;
}
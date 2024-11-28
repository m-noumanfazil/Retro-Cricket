#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>  // For rand() function
#include <time.h>    // For seeding random number generation
#include<unistd.h>
// Global Variables
const int screenWidth = 1600;
const int screenHeight = 800;

Texture2D background;
Texture2D flagLeft;
Texture2D flagRight;
Texture2D button1;
Texture2D button2;
Texture2D button3;
Texture2D button4;
Texture2D button6;
Texture2D handGestures[6]; 
float buttonScale = 0.2f;  // Default button scale
float hoverScale = 0.15f;  // Scale when hovered

Vector2 button1Position;
Vector2 button2Position;
Vector2 button3Position;
Vector2 button4Position;
Vector2 button6Position;

int userScore = 0;
int aiScore = 0;
int lastUserScore = 0;  // It holds the last user score that user entered
int wickets = 0;
int totalScore = 0;
int target = 0;  // It is a target variable which is initially zero

int userGesture = -1;  // Index of the user's selected gesture
int aiGesture = -1;    // Index of the AI's selected gesture
bool isOut = false;    // Flag to indicate if a wicket has fallen
float outTimer = 0.0f; // Timer for displaying the out gesture
// The function for AI scoring 
int GetAIScore(int userLastScore) {
    int aiScore;
    int strategyChance = rand() % 100;  // It Generate a random number between 0 and 99

    do {
        if (strategyChance < 50) {  // It check the strategy chance within 50% 
            aiScore = userLastScore + (rand() % 3 - 1);  // It will choose a number near the user's last score
            if (aiScore < 1) aiScore = 1;  // Clamp to valid range
            if (aiScore > 6) aiScore = 6;
        } else {  // 50% chance to choose completely random score
            aiScore = rand() % 6 + 1;  // Random score between 1 and 6
        }
    } while (aiScore == 5);  // Repeat until aiScore is not 5

    return aiScore;
}

// Function to initialize the game
void InitializeGame() {
    InitWindow(screenWidth, screenHeight, "Raylib - Multiple Hover Buttons");

    // Seed random number generator
    srand(time(NULL));

    // Load textures for background, flags, and buttons
    background = LoadTexture("C:/raylib/raylib/projects/Notepad++/RetroCricket/assets/Stadium2.png");
    flagLeft = LoadTexture("C:/raylib/raylib/projects/Notepad++/RetroCricket/assets/Australia.png");
    flagRight = LoadTexture("C:/raylib/raylib/projects/Notepad++/RetroCricket/assets/india.png");
    button1 = LoadTexture("C:/raylib/raylib/projects/Notepad++/RetroCricket/assets/1.png");
    button2 = LoadTexture("C:/raylib/raylib/projects/Notepad++/RetroCricket/assets/2.png");
    button3 = LoadTexture("C:/raylib/raylib/projects/Notepad++/RetroCricket/assets/3.png");
    button4 = LoadTexture("C:/raylib/raylib/projects/Notepad++/RetroCricket/assets/4.png");
    button6 = LoadTexture("C:/raylib/raylib/projects/Notepad++/RetroCricket/assets/6.png");
    
    handGestures[0] = LoadTexture("C:\\raylib\\raylib\\projects\\Notepad++\\Retrocricket\\assets\\a1.png");
    handGestures[1] = LoadTexture("C:\\raylib\\raylib\\projects\\Notepad++\\Retrocricket\\assets\\a2.png");
    handGestures[2] = LoadTexture("C:\\raylib\\raylib\\projects\\Notepad++\\Retrocricket\\assets\\a3.png");
    handGestures[3] = LoadTexture("C:\\raylib\\raylib\\projects\\Notepad++\\Retrocricket\\assets\\a4.png");
    handGestures[4] = LoadTexture("C:\\raylib\\raylib\\projects\\Notepad++\\Retrocricket\\assets\\out.png");
    handGestures[5] = LoadTexture("C:\\raylib\\raylib\\projects\\Notepad++\\Retrocricket\\assets\\a5.png");
    // Check if all textures loaded correctly
    if (background.id == 0 || flagLeft.id == 0 || flagRight.id == 0 || button1.id == 0 || button2.id == 0 || button3.id == 0 || button4.id == 0 || button6.id == 0) {
        CloseWindow();
        printf("Failed to load textures. Check file paths.\n");
        exit(-1);
    }

    // Calculate button positions
    int totalButtons = 5;
    float spacing = (screenWidth - (totalButtons * button1.width * buttonScale)) / (totalButtons + 1);

    button1Position = (Vector2){ spacing, screenHeight - (button1.height * buttonScale) - 50 };
    button2Position = (Vector2){ button1Position.x + button1.width * buttonScale + spacing, button1Position.y };
    button3Position = (Vector2){ button2Position.x + button2.width * buttonScale + spacing, button1Position.y };
    button4Position = (Vector2){ button3Position.x + button3.width * buttonScale + spacing, button1Position.y };
    button6Position = (Vector2){ button4Position.x + button4.width * buttonScale + spacing, button1Position.y };
}



void user_ai_innings() {
    int innings = 1;
    bool cheerPlayed = false;    // Track the current innings
    InitAudioDevice(); 
    Sound clicksound = LoadSound("C:/raylib/raylib/projects/Notepad++/RetroCricket/assets/clicksound.wav");
    Sound six = LoadSound("C:/raylib/raylib/projects/Notepad++/RetroCricket/assets/six.wav");
    Sound wicket = LoadSound("C:/raylib/raylib/projects/Notepad++/RetroCricket/assets/wicket.wav");
    Sound cheer = LoadSound("C:/raylib/raylib/projects/Notepad++/RetroCricket/assets/cheer.wav");
    while (!WindowShouldClose()) {
        Vector2 mousePosition = GetMousePosition();
        float deltaTime = GetFrameTime();
        
        // Check hover and key press for each button only if wickets < 5
        bool isButton1Hovered = (wickets < 5) && (CheckCollisionPointRec(mousePosition, (Rectangle){ button1Position.x, button1Position.y, button1.width * buttonScale, button1.height * buttonScale }) || IsKeyDown(KEY_ONE));
        bool isButton2Hovered = (wickets < 5) && (CheckCollisionPointRec(mousePosition, (Rectangle){ button2Position.x, button2Position.y, button2.width * buttonScale, button2.height * buttonScale }) || IsKeyDown(KEY_TWO));
        bool isButton3Hovered = (wickets < 5) && (CheckCollisionPointRec(mousePosition, (Rectangle){ button3Position.x, button3Position.y, button3.width * buttonScale, button3.height * buttonScale }) || IsKeyDown(KEY_THREE));
        bool isButton4Hovered = (wickets < 5) && (CheckCollisionPointRec(mousePosition, (Rectangle){ button4Position.x, button4Position.y, button4.width * buttonScale, button4.height * buttonScale }) || IsKeyDown(KEY_FOUR));
        bool isButton6Hovered = (wickets < 5) && (CheckCollisionPointRec(mousePosition, (Rectangle){ button6Position.x, button6Position.y, button6.width * buttonScale, button6.height * buttonScale }) || IsKeyDown(KEY_SIX));

        // Detect button clicks or key presses and update game state
        if (isButton1Hovered && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ONE))) {
            PlaySound(clicksound); // Play sound effect
            lastUserScore = userScore = 1;
            userGesture = userScore - 1;  // Set the user gesture based on the user's score
        }
        if (isButton2Hovered && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_TWO))) {
            PlaySound(clicksound); // Play sound effect
            lastUserScore = userScore = 2;
            userGesture = userScore - 1;  // Set the user gesture based on the user's score
        }
        if (isButton3Hovered && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_THREE))) {
            PlaySound(clicksound); // Play sound effect
            lastUserScore = userScore = 3;
            userGesture = userScore - 1;  // Set the user gesture based on the user's score
        }
        if (isButton4Hovered && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_FOUR))) {
            PlaySound(clicksound); // Play sound effect
            lastUserScore = userScore = 4;
            userGesture = userScore - 1;  // Set the user gesture based on the user's score
        }
        if (isButton6Hovered && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SIX))) {
            PlaySound(clicksound); // Play sound effect
            lastUserScore = userScore = 6;
            PlaySound(six);
            userGesture = userScore - 1;  // Set the user gesture based on the user's score
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
                    totalScore += userScore; 
                       // Add user score to total in the first innings
                } else {
                    totalScore += aiScore; // Add AI score to total in the second innings
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

        // Reset variables and introduce a pause after 1st innings
        if (innings == 1 && wickets == 5) {
            target = totalScore + 1; // Set the target

            // Display a pause message
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("End of 1st Innings. Starting 2nd Innings...", screenWidth / 2 - MeasureText("End of 1st Innings. Starting 2nd Innings...", 30) / 2, screenHeight / 2, 30, DARKBLUE);

            // Display the target
            DrawText(TextFormat("Target: %i", target), screenWidth / 2 - MeasureText(TextFormat("Target: %i", target), 30) / 2, screenHeight / 2 + 50, 30, DARKGREEN);

            EndDrawing();

            WaitTime(2.0); // Pause for 2 seconds (function implementation below)

            // Reset for the 2nd innings
            innings = 2;
            wickets = 0;
            totalScore = 0;
            userScore = 0;
            lastUserScore = 0;
            aiScore = 0; // Reset AI score
            userGesture = -1;
            aiGesture = -1;
            isOut = false;
        }

        // Check win/loss conditions
        bool aiWon = (innings == 2 && totalScore >= target);
        bool userWon = (innings == 2 && wickets == 5);

        // Set the button scale depending on whether it's hovered
        float currentScale1 = isButton1Hovered ? hoverScale : buttonScale;
        float currentScale2 = isButton2Hovered ? hoverScale : buttonScale;
        float currentScale3 = isButton3Hovered ? hoverScale : buttonScale;
        float currentScale4 = isButton4Hovered ? hoverScale : buttonScale;
        float currentScale6 = isButton6Hovered ? hoverScale : buttonScale;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw background
        DrawTexture(background, screenWidth / 2 - background.width / 2, screenHeight / 2 - background.height / 2, WHITE);

        // Draw buttons with hover effect
        DrawTextureEx(button1, button1Position, 0.0f, currentScale1, WHITE);
        DrawTextureEx(button2, button2Position, 0.0f, currentScale2, WHITE);
        DrawTextureEx(button3, button3Position, 0.0f, currentScale3, WHITE);
        DrawTextureEx(button4, button4Position, 0.0f, currentScale4, WHITE);
        DrawTextureEx(button6, button6Position, 0.0f, currentScale6, WHITE);

        // Draw scores and game state
        if (innings == 1) {
            DrawText("1st Innings", screenWidth / 2 - MeasureText("1st Innings", 30) / 2, 20, 30, DARKBLUE);
           DrawText("You are batting first", 
         screenWidth / 2 - MeasureText("You are batting first", 30) / 2, // Center horizontally
         150,  // Adjusted position below the innings title
         30, DARKGREEN); // Increased font size to 30

        } else {
            DrawText("2nd Innings", screenWidth / 2 - MeasureText("2nd Innings", 30) / 2, 20, 30, DARKBLUE);
        }
        DrawText(TextFormat("Last Entered Score: %i", lastUserScore), 50, 60, 30, DARKGREEN);
        DrawText(TextFormat("AI Score: %i", aiScore), screenWidth - 400, 60, 30, DARKGREEN);
        DrawText(TextFormat("Total Score: %i", totalScore), screenWidth / 2 - MeasureText(TextFormat("Total Score: %i", totalScore), 30) / 2, 60, 30, DARKGREEN);
        DrawText(TextFormat("Wickets: %i", wickets), screenWidth / 2 - MeasureText(TextFormat("Wickets: %i", wickets), 30) / 2, 100, 30, DARKGREEN);

        // If wickets reached 5, display target
        if (innings == 1 && wickets == 5) {
            DrawText(TextFormat("Target: %i", target), screenWidth / 2 - MeasureText(TextFormat("Target: %i", target), 30) / 2, 150, 30, DARKGREEN);
        }
 // Display the target score during 2nd innings
        if (innings == 2) {
            DrawText(TextFormat("Target: %i", target), screenWidth / 2 - MeasureText(TextFormat("Target: %i", target), 30) / 2, 140, 30, DARKGREEN);
        }
        // Display win/loss messages
if (userWon) { // User wins now means AI lost
    int margin = target - totalScore; // Calculate the run margin for the user
    DrawText(TextFormat("User Won the Game by %i runs!", margin),
             screenWidth / 2 - MeasureText(TextFormat("User Won the Game by %i runs!", margin), 24) / 2,
             screenHeight / 2 - 190, // Adjusted position to be a little higher
             30,
             DARKGREEN); // Green color for User win
} else if (aiWon) { // AI wins now means the User lost
    int remainingWickets = 5 - wickets; // Calculate remaining wickets for AI
    DrawText(TextFormat("AI Won the Game by %i wickets!", remainingWickets),
             screenWidth / 2 - MeasureText(TextFormat("AI Won the Game by %i wickets!", remainingWickets), 26) / 2,
             screenHeight / 2 - 190, // Adjusted position so that it comes in the box
             30,
             RED); // Red color for AI win
}




        // Draw flags below the scores
        DrawTexture(flagLeft, 50, -20 + MeasureText(TextFormat("Wickets: %i", wickets), 30), WHITE);
        DrawTexture(flagRight, screenWidth - flagRight.width - 250, -20 + MeasureText(TextFormat("Wickets: %i", wickets), 30), WHITE);
        if (userGesture >= 0) {
            DrawTexture(handGestures[userGesture], screenWidth / 4 - handGestures[userGesture].width / 2, screenHeight / 2 - handGestures[userGesture].height / 2, WHITE);
        }
        if (aiGesture >= 0) {
            DrawTexture(handGestures[aiGesture], 3 * screenWidth / 4 - handGestures[aiGesture].width / 2, screenHeight / 2 - handGestures[aiGesture].height / 2, WHITE);
        }

        // Draw "Out" gesture in the middle when a wicket falls
        if (isOut) {
            DrawTexture(handGestures[4], screenWidth / 2 - handGestures[5].width / 2, screenHeight / 2 - handGestures[5].height / 2, WHITE);
        }
        EndDrawing();
           if ((userWon || aiWon) && !cheerPlayed) {
        PlaySound(cheer);
        cheerPlayed = true;
    }

    // Exit loop if ESC is pressed or the game is over
    if ((userWon || aiWon) && cheerPlayed) {
        WaitTime(4.0); // Optional: Add a delay for the cheer sound to play fully
        break;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        break;
    }

        // Exit loop if game ends, or allow exit with ESC key
        if (aiWon || userWon || IsKeyPressed(KEY_ESCAPE)) break;
        
}
     UnloadSound(clicksound);  
    UnloadSound(six);
    UnloadSound(wicket);// Unload the sound
    CloseAudioDevice();
}
     


// Function to clean up resources

void Cleanup() {
    UnloadTexture(background);
    UnloadTexture(flagLeft);
    UnloadTexture(flagRight);
    UnloadTexture(button1);
    UnloadTexture(button2);
    
    UnloadTexture(button3);
    UnloadTexture(button4);
    UnloadTexture(button6);
     for (int i = 0; i < 6; i++) {
        UnloadTexture(handGestures[i]);
    }
    CloseWindow();
}

int main(void) {
    InitializeGame();
    user_ai_innings();
    WaitTime(4.0); // wait for 4 secs to close the window automatically
    Cleanup();
    return 0;
}





#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#include <raylib.h>

// Pentru getcwd() -  calea curentă de lucru
#include <unistd.h>

#define PATH_MAX 1024
#define MAX_TRIES 6
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 100

// Declarari globale importante pentru joc
typedef enum GameScreen 
{
    MAIN_MENU = 0,
    DIFFICULTY_SELECTION,
    LANGUAGE_SELECTION,
    DOMAIN_SELECTION,
    PLAYING_LEVEL,
    LEVEL_FINISHED,
    GAME_OVER
} GameScreen;

GameScreen current_screen = MAIN_MENU;

char words[MAX_WORDS][MAX_WORD_LENGTH];
int word_count;
char guessed_word[MAX_WORD_LENGTH];       // Cuvantul pe care trebuie sa il ghicim
char current_display_word[MAX_WORD_LENGTH]; // Afisare cuvant (cu _ )
int word_len;
int tries;                             // Numar greseli curente
double current_level_score;
int correct_guesses;
time_t level_start_time;
int time_limit;
double points_per_letter;

int current_level = 1;
double total_score = 0.0;
int max_levels = 5;

int chosen_difficulty=0;

//Culori definite pentru interfata jocului
#define BACKGROUND_COLOR CLITERAL(Color){ 231, 213, 243, 255 } // Background mov
#define PRIMARY_TEXT_COLOR CLITERAL(Color){ 65, 105, 225, 255 }  // Royal Blue
#define ACCENT_COLOR CLITERAL(Color){ 65, 105, 225, 255 }     // Royal Blue
#define HANGMAN_COLOR CLITERAL(Color){65, 105, 255, 255 }   //Royal Blue si pentru titlul HANGMAN
#define WORD_DISPLAY_COLOR CLITERAL(Color){ 65, 105, 225, 255 } // Royal Blue pentru cuvant
#define HINT_COLOR CLITERAL(Color){ 65, 105, 225, 255 }     // Royal Blue pentru instructiuni/restul textului

// Variabile globale pentru fonturi
Font customFontTitle; // Fontul DOCUMENTA pentru titlu
Font customFontArial;     // Fontul CODE BOLD REGULAR pentru restul textului



// Functie  pentru centrarea textului
float CenteredText(Font font, const char *text, float fontSize, float spacing, int screenWidth) 
{
    Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);  // Measure string size for Font
    return (screenWidth - textSize.x) / 2.0f;
}


void hangman_draw(int current_tries)
{
    // Am folosit offset pentru a pozitiona mai bine spanzuratoarea (in jos)
    int y_offset = 50; 
    int x_offset = 150; // Pozitionez spanzuratoarea mai in dreapta dupa plac

    // DrawLineEx pentru ilustratia spanzuratorii
    DrawLineEx((Vector2){100 + x_offset, 400 + y_offset}, (Vector2){200 + x_offset, 400 + y_offset}, 4, HANGMAN_COLOR); // Baza
    DrawLineEx((Vector2){150 + x_offset, 400 + y_offset}, (Vector2){150 + x_offset, 100 + y_offset}, 4, HANGMAN_COLOR); // Stalp vertical
    DrawLineEx((Vector2){150 + x_offset, 100 + y_offset}, (Vector2){250 + x_offset, 100 + y_offset}, 4, HANGMAN_COLOR); // Bara orizontala de sus
    DrawLineEx((Vector2){250 + x_offset, 100 + y_offset}, (Vector2){250 + x_offset, 130 + y_offset}, 2, HANGMAN_COLOR); // "Franghia"

    // Coordonate cap si corp raportate si la offset-urile de mai sus
    int head_x = 250 + x_offset;
    int head_y = 150 + y_offset;
    int body_start_y = 170 + y_offset;
    int body_end_y = 250 + y_offset;

    // Am ajustat grosimea pentru componentele ilustratiei
    float hangman_thickness = 4; 

    // Aici se deseneaza/apare cate o componenta (progresiv) la fiecare greseala facuta
    if (current_tries >= 1) 
    { // Capul
        DrawCircle(head_x, head_y, 20, HANGMAN_COLOR);
    }
    if (current_tries >= 2) 
    { // Corpul
        DrawLineEx((Vector2){(float)head_x, (float)body_start_y}, (Vector2){(float)head_x, (float)body_end_y}, hangman_thickness, HANGMAN_COLOR);
    }
    if (current_tries >= 3) 
    { // Bratul stang
        DrawLineEx((Vector2){(float)head_x, (float)body_start_y + 10}, (Vector2){(float)head_x - 30, (float)body_start_y + 50}, hangman_thickness, HANGMAN_COLOR);
    }
    if (current_tries >= 4) 
    { // Bratul drept
        DrawLineEx((Vector2){(float)head_x, (float)body_start_y + 10}, (Vector2){(float)head_x + 30, (float)body_start_y + 50}, hangman_thickness, HANGMAN_COLOR);
    }
    if (current_tries >= 5) 
    { // Piciorul stang
        DrawLineEx((Vector2){(float)head_x, (float)body_end_y}, (Vector2){(float)head_x - 30, (float)body_end_y + 50}, hangman_thickness, HANGMAN_COLOR);
    }
    if (current_tries >= 6) 
    { // Piciorul drept (daca apare, atunci nivelul e pierdut)
        DrawLineEx((Vector2){(float)head_x, (float)body_end_y}, (Vector2){(float)head_x + 30, (float)body_end_y + 50}, hangman_thickness, HANGMAN_COLOR);
    }
}


void read_from_chosen_file(const char *filename_str, char words_arr[][MAX_WORD_LENGTH], int *word_count_ptr)
{
    FILE *sourcefile = fopen(filename_str, "r");
    if (sourcefile == NULL)
     {
        perror("Error opening file");
        exit(-1);
    }
    char buffer[4096];
    if (fgets(buffer, sizeof(buffer), sourcefile) != NULL) 
    {
        char *p = strtok(buffer, ",");
        *word_count_ptr = 0;
        while (p != NULL && *word_count_ptr < MAX_WORDS) 
        {
            while (isspace((unsigned char)*p)) 
                p++;
            int end = strlen(p) - 1;
            while (end >= 0 && isspace((unsigned char)p[end])) 
            {
                p[end] = '\0';
                end--;
            }
            strncpy(words_arr[*word_count_ptr], p, MAX_WORD_LENGTH - 1);
            words_arr[*word_count_ptr][MAX_WORD_LENGTH - 1] = '\0';
            (*word_count_ptr)++;
            p = strtok(NULL, ",");
        }
    } else 
    {
        
        perror("No words found in this file");
        fclose(sourcefile);
        exit(-1);
    }
    fclose(sourcefile);
}



void InitLevel(int difficulty, int language, int domain) 
{
    char filename[50];
    
    if (language == 1)
     { // English
        switch (domain)
         {
            case 1: strcpy(filename, "food_en.txt"); break;
            case 2: strcpy(filename, "nature_en.txt"); break;
            case 3: strcpy(filename, "school_en.txt"); break;
            default: TraceLog(LOG_WARNING, "Invalid domain, defaulting to food_en.txt"); strcpy(filename, "food_en.txt"); break;
        }
    } 
    else if (language == 2) 
    { // Romanian
        switch (domain) 
        {
            case 1: strcpy(filename, "food_ro.txt"); break;
            case 2: strcpy(filename, "nature_ro.txt"); break;
            case 3: strcpy(filename, "school_ro.txt"); break;
            default: TraceLog(LOG_WARNING, "Invalid domain, defaulting to food_ro.txt"); strcpy(filename, "food_ro.txt"); break;
        }
    } 
    else 
    {
        TraceLog(LOG_WARNING, "Invalid language, defaulting to English");
        strcpy(filename, "food_en.txt");
    }

    read_from_chosen_file(filename, words, &word_count);

    if (word_count == 0) 
    {
        perror("No words were read from this file");
        current_screen = GAME_OVER;
        return;
    }

    // Setare dificultate
    switch (difficulty) 
    {
        case 1: time_limit = 60; points_per_letter = 0.1; break; // Easy
        case 2: time_limit = 40; points_per_letter = 0.25; break; // Medium
        case 3: time_limit = 25; points_per_letter = 0.5; break; // Hard
        default: time_limit = 40; points_per_letter = 0.25; break;
    }

    // Resetare variabile nivel
    srand(time(NULL));
    strncpy(guessed_word, words[rand() % word_count], MAX_WORD_LENGTH - 1);
    guessed_word[MAX_WORD_LENGTH - 1] = '\0';
    word_len = strlen(guessed_word);

    correct_guesses = 0;
    for (int i = 0; i < word_len; i++) 
    {
        if (isspace(guessed_word[i])) 
        {
            current_display_word[i] = ' ';
            correct_guesses++; // Pt ca spatiile sa fie considerate ghicite corect
        } else 
        {
            current_display_word[i] = '_';
        }
    }
    current_display_word[word_len] = '\0';

    tries = 0;
    current_level_score = 0.0;
    level_start_time = time(NULL); // inregistraza timpul de start al nivelului
}


void UpdatePlayingLevel() 
{
    double elapsed = difftime(time(NULL), level_start_time); //functie din time.h: timpul curent - timpul de start

    // Verificare timp
    if (elapsed >= time_limit) 
    {
        TraceLog(LOG_INFO, "Time's up! Level %d finished.", current_level);
        current_screen = LEVEL_FINISHED;
        return;
    }

    // Verificare input (litere)
    int key = GetKeyPressed();
    if (key >= KEY_A && key <= KEY_Z)  //RayLib: KEY_A=65: A sau a, KEY_Z=90: Z sau z
    {
        char guess_char = (char)tolower(key);
        int found = 0;
        for (int i = 0; i < word_len; i++) {
            if (tolower(guessed_word[i]) == guess_char) 
            {
                if (current_display_word[i] == '_')
                 { 
                    current_display_word[i] = guessed_word[i];
                    correct_guesses++;
                    current_level_score = current_level_score + points_per_letter;
                    found = 1;
                }
            }
        }
        if (!found) 
        {
            tries++; 
        }
    }

    // Verificare daca nivelul a fost castigat sau pierdut
    if (correct_guesses == word_len) 
    {
        TraceLog(LOG_INFO, "You won level %d!", current_level);
        current_screen = LEVEL_FINISHED;
    } 
    else if (tries >= MAX_TRIES)
     {
        TraceLog(LOG_INFO, "You lost level %d!", current_level);
        current_screen = LEVEL_FINISHED;
    }
}

 

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Hangman Game");
    SetTargetFPS(60);

    // Pentru directorul curent 
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        TraceLog(LOG_INFO, "Current working directory: %s", cwd);
    } 
    else 
    {
        TraceLog(LOG_WARNING, "Error getting current working directory.");
    }

    // Am pus un custom font
    customFontTitle = LoadFontEx("/Users/mara-karina/Library/Fonts/Documenta.otf", 130, 0, 0);
    if (customFontTitle.texture.id == 0) 
    {
        perror("Error loading custom title");
        customFontTitle = GetFontDefault();
    } 
    else 
    {
        TraceLog(LOG_INFO, "Custom title font loaded successfully!");
        SetTextureFilter(customFontTitle.texture, TEXTURE_FILTER_BILINEAR); 
    }

    // Încărcarea celuilalt font
    customFontArial = LoadFontEx("/Users/mara-karina/Library/Fonts/CODE Bold.otf", 30, 0, 0); 
    if (customFontArial.texture.id == 0) 
    {
         perror("Failed to load the other font. Using Raylib default font for general text.");
        customFontArial = GetFontDefault(); 
    } 
    else 
    {
        TraceLog(LOG_INFO, "Arial font loaded successfully!");
        SetTextureFilter(customFontArial.texture, TEXTURE_FILTER_BILINEAR);
    }


    
    int lang_choice = 0;
    int domain_choice = 0;

    while (!WindowShouldClose())
    {
    
        switch (current_screen) 
        {
            case MAIN_MENU:
                if (IsKeyPressed(KEY_SPACE)) 
                {
                    current_screen = DIFFICULTY_SELECTION;
                    current_level = 1;
                    total_score = 0;
                }
                break;
            case DIFFICULTY_SELECTION:
                if (IsKeyPressed(KEY_ONE)) { chosen_difficulty = 1; current_screen = LANGUAGE_SELECTION; }
                else if (IsKeyPressed(KEY_TWO)) { chosen_difficulty = 2; current_screen = LANGUAGE_SELECTION; }
                else if (IsKeyPressed(KEY_THREE)) { chosen_difficulty = 3; current_screen = LANGUAGE_SELECTION; }
                break;
            case LANGUAGE_SELECTION:
                if (IsKeyPressed(KEY_ONE)) { lang_choice = 1; current_screen = DOMAIN_SELECTION; }
                else if (IsKeyPressed(KEY_TWO)) { lang_choice = 2; current_screen = DOMAIN_SELECTION; }
                break;
            case DOMAIN_SELECTION:
                if (IsKeyPressed(KEY_ONE)) { domain_choice = 1; current_screen = PLAYING_LEVEL; InitLevel(chosen_difficulty, lang_choice, domain_choice); }
                else if (IsKeyPressed(KEY_TWO)) { domain_choice = 2; current_screen = PLAYING_LEVEL; InitLevel(chosen_difficulty, lang_choice, domain_choice); }
                else if (IsKeyPressed(KEY_THREE)) { domain_choice = 3; current_screen = PLAYING_LEVEL; InitLevel(chosen_difficulty, lang_choice, domain_choice); }
                break;
            case PLAYING_LEVEL:
                UpdatePlayingLevel();
                break;
            case LEVEL_FINISHED:
                if (IsKeyPressed(KEY_SPACE))
                {
                    total_score = total_score+ current_level_score;
                    current_level++;
                    if (current_level <= max_levels) 
                    {
                        //revenim la LANGUAGE_SELECTION in loc de dificultate
                        current_screen = LANGUAGE_SELECTION;
                    } 
                    else 
                    {
                        current_screen = GAME_OVER;
                    }
                }
                break;
            case GAME_OVER:
                if (IsKeyPressed(KEY_SPACE)) 
                {
                    current_screen = MAIN_MENU;
                }
                if (IsKeyPressed(KEY_ESCAPE)) 
                {
                    CloseWindow();
                }
                break;
        }


        //Desen
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        float default_spacing = 2.0f; // Spatiere pentru textul implicit
        float word_spacing = 5.0f;    // Spatiere pentru cuvantul care trebuie ghicit

        switch (current_screen) 
        {
            case MAIN_MENU: 
            {
                // Titlul "HANGMAN GAME" 
                const char* titleText = "HANGMAN GAME";
                float titleFontSize = 80;
                float titleX = CenteredText(customFontTitle, titleText, titleFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontTitle, titleText, (Vector2){titleX, screenHeight / 2 - 100}, titleFontSize, default_spacing, ACCENT_COLOR);

                
                const char* startText = "Enter SPACE to start the game";
                float startFontSize = 25;
                float startX = CenteredText(customFontArial, startText, startFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, startText, (Vector2){startX, screenHeight / 2 + 20}, startFontSize, default_spacing, PRIMARY_TEXT_COLOR);
                break;
            }
            case DIFFICULTY_SELECTION: 
            {
                const char* header = "Choose the difficulty:";
                float headerFontSize = 50;
                float headerX = CenteredText(customFontArial, header, headerFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, header, (Vector2){headerX, 150}, headerFontSize, default_spacing, PRIMARY_TEXT_COLOR);

                float optionFontSize = 25;
                const char* opt1 = "1. Easy (60 sec)";
                const char* opt2 = "2. Medium (40 sec)";
                const char* opt3 = "3. Hard (25 sec)";

                DrawTextEx(customFontArial, opt1, (Vector2){CenteredText(customFontArial, opt1, optionFontSize, default_spacing, screenWidth), 220}, optionFontSize, default_spacing, PRIMARY_TEXT_COLOR);
                DrawTextEx(customFontArial, opt2, (Vector2){CenteredText(customFontArial, opt2, optionFontSize, default_spacing, screenWidth), 260}, optionFontSize, default_spacing, PRIMARY_TEXT_COLOR);
                DrawTextEx(customFontArial, opt3, (Vector2){CenteredText(customFontArial, opt3, optionFontSize, default_spacing, screenWidth), 300}, optionFontSize, default_spacing, PRIMARY_TEXT_COLOR);
                break;
            }
            case LANGUAGE_SELECTION: 
            {
                const char* header = "Choose the language:";
                float headerFontSize = 50;
                float headerX = CenteredText(customFontArial, header, headerFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, header, (Vector2){headerX, 150}, headerFontSize, default_spacing, PRIMARY_TEXT_COLOR);

                float optionFontSize = 25;
                const char* opt1 = "1. English";
                const char* opt2 = "2. Romanian";

                DrawTextEx(customFontArial, opt1, (Vector2){CenteredText(customFontArial, opt1, optionFontSize, default_spacing, screenWidth), 220}, optionFontSize, default_spacing, PRIMARY_TEXT_COLOR);
                DrawTextEx(customFontArial, opt2, (Vector2){CenteredText(customFontArial, opt2, optionFontSize, default_spacing, screenWidth), 260}, optionFontSize, default_spacing, PRIMARY_TEXT_COLOR);
                break;
            }
            case DOMAIN_SELECTION: 
            {
                const char* header = "Choose a topic for your word:";
                float headerFontSize = 45;
                float headerX = CenteredText(customFontArial, header, headerFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, header, (Vector2){headerX, 150}, headerFontSize, default_spacing, PRIMARY_TEXT_COLOR);

                float optionFontSize = 25;
                const char* opt1 = "1. Food";
                const char* opt2 = "2. Nature";
                const char* opt3 = "3. School";

                DrawTextEx(customFontArial, opt1, (Vector2){CenteredText(customFontArial, opt1, optionFontSize, default_spacing, screenWidth), 220}, optionFontSize, default_spacing, PRIMARY_TEXT_COLOR);
                DrawTextEx(customFontArial, opt2, (Vector2){CenteredText(customFontArial, opt2, optionFontSize, default_spacing, screenWidth), 260}, optionFontSize, default_spacing, PRIMARY_TEXT_COLOR);
                DrawTextEx(customFontArial, opt3, (Vector2){CenteredText(customFontArial, opt3, optionFontSize, default_spacing, screenWidth), 300}, optionFontSize, default_spacing, PRIMARY_TEXT_COLOR);
                break;
            }
            case PLAYING_LEVEL: 
            {
                // Textele  (nivel, scor, timp, greseli) 
                DrawTextEx(customFontArial, TextFormat("Level: %d out of %d", current_level, max_levels), (Vector2){50, 50}, 22, default_spacing, PRIMARY_TEXT_COLOR);
                DrawTextEx(customFontArial, TextFormat("Total score: %.2f", total_score), (Vector2){50, 80}, 22, default_spacing, PRIMARY_TEXT_COLOR);
                DrawTextEx(customFontArial, TextFormat("Time left: %.0f sec", time_limit - difftime(time(NULL), level_start_time)), (Vector2){550, 50}, 22, default_spacing, PRIMARY_TEXT_COLOR);
                DrawTextEx(customFontArial, TextFormat("Wrong guesses: %d out of %d", tries, MAX_TRIES), (Vector2){50, 110}, 22, default_spacing, PRIMARY_TEXT_COLOR);
                
                // Afisare cuvantul de ghicit cu spatii intre litere 
                char spaced_display_word[MAX_WORD_LENGTH * 2]; // Spatiu pentru '_' si spatii
                int k = 0;
                for (int i = 0; i < word_len; i++) 
                {
                    spaced_display_word[k++] = current_display_word[i];
                    if (i < word_len - 1) 
                    {
                        spaced_display_word[k++] = ' '; // Adauga spatiu intre litere
                    }
                }
                spaced_display_word[k] = '\0';
                
                // Marime cuvant ghicit - centrat
                float wordDisplayFontSize = 30;
                float wordDisplayX = CenteredText(customFontArial, spaced_display_word, wordDisplayFontSize, word_spacing, screenWidth);
                DrawTextEx(customFontArial, TextFormat("%s", spaced_display_word), (Vector2){wordDisplayX, 465}, wordDisplayFontSize, word_spacing, WORD_DISPLAY_COLOR);
                
                // Instructiunile sa fie centrate
                const char* hintText = "Choose a letter...";
                float hintFontSize = 20;
                float hintX = CenteredText(customFontArial, hintText, hintFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, hintText, (Vector2){hintX, 500}, hintFontSize, default_spacing, HINT_COLOR);
                
                hangman_draw(tries);
                break;
            }
            case LEVEL_FINISHED: 
            {
                // Toate textele centrate 
                const char* levelFinishedHeader = TextFormat("Level %d complete!", current_level);
                float levelFinishedHeaderFontSize = 35;
                float levelFinishedHeaderX = CenteredText(customFontArial, levelFinishedHeader, levelFinishedHeaderFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, levelFinishedHeader, (Vector2){levelFinishedHeaderX, 150}, levelFinishedHeaderFontSize, default_spacing, PRIMARY_TEXT_COLOR);

                const char* levelScore = TextFormat("This level's score: %.2f", current_level_score);
                float levelScoreFontSize = 28;
                float levelScoreX = CenteredText(customFontArial, levelScore, levelScoreFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, levelScore, (Vector2){levelScoreX, 220}, levelScoreFontSize, default_spacing, PRIMARY_TEXT_COLOR);

                const char* totalScoreText = TextFormat("Total score: %.2f", total_score + current_level_score);
                float totalScoreFontSize = 28;
                float totalScoreX = CenteredText(customFontArial, totalScoreText, totalScoreFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, totalScoreText, (Vector2){totalScoreX, 260}, totalScoreFontSize, default_spacing, PRIMARY_TEXT_COLOR);

                const char* nextLevelPrompt = "Enter SPACE to start the next level";
                float nextLevelPromptFontSize = 22;
                float nextLevelPromptX = CenteredText(customFontArial, nextLevelPrompt, nextLevelPromptFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, nextLevelPrompt, (Vector2){nextLevelPromptX, 350}, nextLevelPromptFontSize, default_spacing, PRIMARY_TEXT_COLOR);

                if (correct_guesses == word_len) 
                {
                    const char* winText = TextFormat("You won! You've guessed the word: %s", guessed_word);
                    float winTextFontSize = 22;
                    float winTextX = CenteredText(customFontArial, winText, winTextFontSize, default_spacing, screenWidth);
                    DrawTextEx(customFontArial, winText, (Vector2){winTextX, 400}, winTextFontSize, default_spacing, PRIMARY_TEXT_COLOR); // Changed from GREEN to PRIMARY_TEXT_COLOR
                } 
                else 
                {
                    const char* loseText = TextFormat("You lost! The word was: %s", guessed_word);
                    float loseTextFontSize = 22;
                    float loseTextX = CenteredText(customFontArial, loseText, loseTextFontSize, default_spacing, screenWidth);
                    DrawTextEx(customFontArial, loseText, (Vector2){loseTextX, 400}, loseTextFontSize, default_spacing, PRIMARY_TEXT_COLOR); // Changed from RED to PRIMARY_TEXT_COLOR
                }
                break;
            }
            case GAME_OVER: 
            {
                // Toate textele centrate 
                const char* gameOverText = "Game Over!";
                float gameOverTextFontSize = 40;
                float gameOverTextX = CenteredText(customFontArial, gameOverText, gameOverTextFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, gameOverText, (Vector2){gameOverTextX, 150}, gameOverTextFontSize, default_spacing, ACCENT_COLOR);

                const char* finalScoreText = TextFormat("Total score: %.2f", total_score);
                float finalScoreFontSize = 30;
                float finalScoreX = CenteredText(customFontArial, finalScoreText, finalScoreFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, finalScoreText, (Vector2){finalScoreX, 220}, finalScoreFontSize, default_spacing, PRIMARY_TEXT_COLOR);

                const char* restartPrompt = "Enter SPACE to start again";
                float restartPromptFontSize = 22;
                float restartPromptX = CenteredText(customFontArial, restartPrompt, restartPromptFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, restartPrompt, (Vector2){restartPromptX, 300}, restartPromptFontSize, default_spacing, PRIMARY_TEXT_COLOR);

                const char* exitPrompt = "Enter ESC to exit the game";
                float exitPromptFontSize = 22;
                float exitPromptX = CenteredText(customFontArial, exitPrompt, exitPromptFontSize, default_spacing, screenWidth);
                DrawTextEx(customFontArial, exitPrompt, (Vector2){exitPromptX, 330}, exitPromptFontSize, default_spacing, PRIMARY_TEXT_COLOR);
                break;
            }
        }

        EndDrawing();
    }

    
    UnloadFont(customFontTitle);
    UnloadFont(customFontArial); 
    CloseWindow();
    return 0;
}
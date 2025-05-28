//
//  versiunea4.c
//  Hangman
//
//  Created by Mara-Karina Ciupa on 28.05.2025.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <time.h>
#define MAX_TRIES 6
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 100

void hangman(int try)
{
    printf("\n");
    printf("  -------\n");
    printf("  |     |\n");
    switch (try)
    {
        case 0:
        
            printf("  |      \n");
            printf("  |      \n");
            printf("  |      \n");
            break;
        case 1:
            
            
            printf("  |     O  \n");
            printf("  |      \n");
            printf("  |      \n");
            break;
        case 2:
            
            
            printf("  |     O  \n");
            printf("  |     |  \n");
            printf("  |      \n");
            break;
        case 3:
            printf("  |     O  \n");
            printf("  |    /|  \n");
            printf("  |      \n");
            break;
        case 4:
            
            
            printf("  |     O  \n");
            printf("  |    /|\\ \n");
            printf("  |      \n");
            break;
        case 5:
            
            printf("  |     O  \n");
            printf("  |    /|\\ \n");
            printf("  |    /   \n");
            break;
            
        case 6:
           
            printf("  |     O  \n");
            printf("  |    /|\\ \n");
            printf("  |    / \\ \n");
            break;
    }
}
void read_from_chosen_file(const char *filename, char words[][MAX_WORD_LENGTH], int *word_count)
{
    FILE *sourcefile = fopen(filename, "r");
    if (sourcefile == NULL)
    {
        perror("Error opening file");
        exit(-1);
    }
    char buffer[4096];
    if (fgets(buffer, sizeof(buffer), sourcefile) != NULL)
    {
        char *p = strtok(buffer, ",");
        *word_count = 0;
        while (p != NULL && *word_count < MAX_WORDS)
        {
            
            while (isspace((unsigned char)*p)) p++;
            int end = strlen(p) - 1;
            while (end >= 0 && isspace((unsigned char)p[end]))
            {
                p[end] = '\0';
                end--;
            }
            strncpy(words[*word_count], p, MAX_WORD_LENGTH - 1);
            words[*word_count][MAX_WORD_LENGTH - 1] = '\0';
            (*word_count)++;
            p = strtok(NULL, ",");
        }
    }
    else
    {
        printf("No words found in the file.\n");
        fclose(sourcefile);
        exit(-1);
    }
    fclose(sourcefile);
}
void play()
{
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int word_count;
    // Alegerea limbii
    int lang_choice;
    printf("Choose language:1. English 2. Romanian: ");
    scanf("%d", &lang_choice);
    // Alegerea domeniului
    int domain_choice;
    printf("Choose a domain:\n1. Food 2. Nature 3. School: ");
    scanf("%d", &domain_choice);
    // Fisierul in functie de alegerile limbii & domeniului
    char filename[50];
    if (lang_choice == 1)
    {
        // Engleza
        switch (domain_choice)
        {
            case 1:
                strcpy(filename, "food_en.txt");
                break;
            case 2:
                strcpy(filename, "nature_en.txt");
                break;
            case 3:
                strcpy(filename, "school_en.txt");
                break;
            default:
                printf("Invalid domain choice!\n");
                return;
        }
    }
    else if (lang_choice == 2)
    {
        // Romana
        switch (domain_choice)
        {
            case 1:
                strcpy(filename, "food_ro.txt");
                break;
            case 2:
                strcpy(filename, "nature_ro.txt");
                break;
            case 3:
                strcpy(filename, "school_ro.txt");
                break;
            default:
                printf("Invalid domain choice!\n");
                return;
        }
    }
    else
    {
        printf("Invalid language choice!\n");
        return;
    }
    
    read_from_chosen_file(filename, words, &word_count);
    
    // Selectare cuvand random din fisier
    srand(time(NULL));
    char *guessed = words[rand() % word_count];
    int word_len = strlen(guessed);
    char current_guess[MAX_WORD_LENGTH];
    int tries = 0, correct_guesses = 0;
    for (int i = 0; i < word_len; i++)
    {
        if (guessed[i] == ' ')
        {
            current_guess[i] = ' ';
            correct_guesses++;
        }
        else
        {
            current_guess[i] = '_';
        }
    }
    current_guess[word_len] = '\0';
    // Incepe timer-ul
    time_t start_time = time(NULL);
    while (tries < MAX_TRIES)
    {
        //Verificare timp
        double elapsed = difftime(time(NULL), start_time);
        if (elapsed >= 60.0)
        {
            printf("\nTime's up! 60 seconds have passed.\n");
            break;
        }
        printf("Time left: %.0f seconds", 60.0 - elapsed);
        printf("Current word: %s", current_guess);
        hangman(tries);
        printf("Enter a letter: ");
        // Citirea literei
        char guess;
        scanf(" %c", &guess);
        guess = tolower(guess);
        int found = 0;
        for (int i = 0; i < word_len; i++)
        {
            if (tolower(guessed[i]) == guess)
            {
                if (current_guess[i] == '_')
                {
                    current_guess[i] = guessed[i];
                    correct_guesses++;
                    found = 1;
                }
            }
        }
        if (!found)
        {
            tries++;
            printf("Wrong guess! %d tries left.\n", MAX_TRIES - tries);
        }
        if (correct_guesses == word_len)
        {
            printf("You won! The word was: %s", guessed);
            return;
        }
    }
    if (tries >= MAX_TRIES)
    {
        printf("You lost! The word was: %s", guessed);
    }
}
int main()
{
    play();
    return 0;
}



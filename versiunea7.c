//
//  versiunea7.c
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
         perror("No words found in the file.\n");
         fclose(sourcefile);
         exit(-1);
     }
     fclose(sourcefile);
 }
 double play_level(double accumulated_score)
{
     // Returneaza scorul obtinut la nivelul curent, adunat la scorul total
     char words[MAX_WORDS][MAX_WORD_LENGTH];
     int word_count;
     // Alegerea nivelului de dificultate
     int difficulty_choice;
     printf("Choose the difficulty level:\n1. Easy (60 sec) 2. Medium (40 sec) 3. Hard (25 sec): ");
     scanf("%d", &difficulty_choice);
     int time_limit;
     double points_per_letter;
     switch (difficulty_choice)
     {
         case 1:
             time_limit = 60;
             points_per_letter = 0.1;
             break;
         case 2:
             time_limit = 40;
             points_per_letter = 0.25;
             break;
         case 3:
             time_limit = 25;
             points_per_letter = 0.5;
             break;
         default:
             printf("Invalid choice! Defaulting to Medium.\n");
             time_limit = 40;
             points_per_letter = 0.25;
     }
     // Alegerea limbii
     int lang_choice;
     printf("Choose language:\n1. English 2. Romanian: ");
     scanf("%d", &lang_choice);
     // Alegerea domeniului
     int domain_choice;
     printf("Choose a domain:\n1. Food 2. Nature 3. School: ");
     scanf("%d", &domain_choice);
     
     char filename[50];
     if (lang_choice == 1)
     {
         switch (domain_choice)
         {
             case 1: strcpy(filename, "food_en.txt"); break;
             case 2: strcpy(filename, "nature_en.txt"); break;
             case 3: strcpy(filename, "school_en.txt"); break;
             default: printf("Invalid domain!\n"); return 0;
         }
     }
     else if (lang_choice == 2)
     {
         switch (domain_choice)
         {
             case 1: strcpy(filename, "food_ro.txt"); break;
             case 2: strcpy(filename, "nature_ro.txt"); break;
             case 3: strcpy(filename, "school_ro.txt"); break;
             default: printf("Invalid domain!\n"); return 0;
         }
     }
     else
     {
         printf("Invalid language!\n");
         return 0;
     }
     read_from_chosen_file(filename, words, &word_count);
     // Selectare cuvant random
     srand(time(NULL));
     char *guessed = words[rand() % word_count];
     int word_len = strlen(guessed);
     char current_guess[MAX_WORD_LENGTH];
     int tries = 0;
     double score = 0.0;
     int correct_guesses = 0;
     // Calcul scor total posibil
     double max_score = points_per_letter * word_len;
     for (int i = 0; i < word_len; i++)
     {
         if (isspace(guessed[i]))
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
     
     time_t start_time = time(NULL);
     while (tries < MAX_TRIES) {
         double elapsed = difftime(time(NULL), start_time);
         if (elapsed >= time_limit)
         {
             printf("Time's up! %d seconds passed.\n", time_limit);
             break;
         }
         printf("\nTime left: %.0f seconds", time_limit - elapsed);
         printf("Current word: %s", current_guess);
         hangman(tries);
         printf("Enter a letter: ");
         char guess_char;
         scanf(" %c", &guess_char);
         guess_char = tolower(guess_char);
         int found = 0;
         for (int i = 0; i < word_len; i++)
         {
             if (tolower(guessed[i]) == guess_char)
             {
                 if (current_guess[i] == '_')
                 {
                     current_guess[i] = guessed[i];
                     correct_guesses++;
                     score += points_per_letter;
                     found = 1;
                 }
             }
         }
         if (!found)
         {
             tries++;
             printf("Wrong guess! Tries left: %d", MAX_TRIES - tries);
         }
         if (correct_guesses == word_len)
         {
             printf("You won! The word was: %s", guessed);
             break;
         }
     }
     if (correct_guesses != word_len)
     {
         printf("You lost! The word was: %s", guessed);
     }
     printf("Score this level: %.2f out of %.2f", score, max_score);
     return score;
 }
 int main()
{
     double total_score = 0.0;
     int total_levels = 5;
     
     for (int level = 1; level <= total_levels; level++)
     {
         printf("\n=== Level %d ===\n", level);
         double level_score = play_level(total_score);
         total_score += level_score;
         printf("Total score after level %d: %.2f", level, total_score);
     }
     printf("\nGame over! Your final score after %d levels is: %.2f", total_levels, total_score);
     return 0;
 }


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hangman_utils.h" // double check if there is alternative

void display_word_progress(char letter_final_arr[], int word_len)
{
    for (int i = 0; i < word_len; i++)
    {
        if (letter_final_arr[i] != 0)
            printf(" %c ", letter_final_arr[i]);
        else
            printf(" _ ");
    }
    printf("\n");
}

void display_guessed_letters(char guessed[], int len)
{
    printf("\t\tGuessed Letters: ");
    for (int i = 0; i < len; i++)
    {
        printf(" %c ", guessed[i]);
    }
    printf("\n");
}
void display_stats(char *username, char *letter_final_arr, int word_len, int num_guesses, int lives)
{
    printf("\n\t 🎉 Congratulations, %s! 🎉\n", username);
    printf("\t--------------------------------------\n");
    printf("\t| 🏆  The Correct Word: ");
    display_word_progress(letter_final_arr, word_len);
    printf("\t--------------------------------------\n");
    printf("\t| 📊  Game Stats                     |\n");
    printf("\t--------------------------------------\n");
    printf("\t| 🏹  Total Guesses    | %-4d       |\n", num_guesses);
    printf("\t| ❤️  Lives Remaining  | %-4d       |\n", lives);
    printf("\t--------------------------------------\n\n");
};

void display_leaderboard(char str_leaderboard[], char username[])
{
    char delim[] = ",";

    int player_count = calc_player_count(str_leaderboard);

    str_leaderboard[strlen(str_leaderboard) - 1] = '\0';
    User_Leaderboard user_leaderboard[player_count];
    char *entry = strtok(str_leaderboard, delim);
    // static ref in strtok

    int rank = 1;
    int i = 0;

    printf("\n\t 🏆  Hangman Leaderboard 🏆\n");
    printf("\t-----------------------------\n");
    printf("\t| %-3s | %-10s | %-5s |\n", "#", "Username", "Score");
    printf("\t-----------------------------\n");

    // "a:10,b:40,"

    while (entry != NULL)
    {
        // printf("%s ", tok);
        char *colon = strchr(entry, ':');
        if (colon != NULL)
        {
            *colon = '\0';
            char *username = entry;
            char *score = colon + 1;

            strcpy(user_leaderboard[i].username, username);
            user_leaderboard[i].score = atoi(score);
            i++;
        }
        entry = strtok(NULL, delim);
    }

    User_Leaderboard temp_user;

    for (int i = 0; i < player_count - 1; i++)
    {
        for (int j = 0; j < player_count - i - 1; j++)
        {
            if (user_leaderboard[j].score < user_leaderboard[j + 1].score)
            {
                temp_user = user_leaderboard[j];
                user_leaderboard[j] = user_leaderboard[j + 1];
                user_leaderboard[j + 1] = temp_user;
            }
        }
    }

    for (int i = 0; i < player_count; i++)
    {
        if (strcmp(user_leaderboard[i].username, username) == 0)
        {
            printf("\t| %-3d | %s %-7s | %-5d |\n", rank, user_leaderboard[i].username, "(you)", user_leaderboard[i].score);
        }
        else
        {
            printf("\t| %-3d | %-10s | %-5d |\n", rank, user_leaderboard[i].username, user_leaderboard[i].score);
        }
        rank++;
    }

    printf("\t-----------------------------\n\n");
}
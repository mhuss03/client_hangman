#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/hangman_utils.h"
void Hungman(int lives)
{
    switch (lives)
    {
    case 8:
        printf("\t\t      \n");
        printf("\t\t      \n");
        printf("\t\t      \n");
        printf("\t\t      \n");
        printf("\t\t      \n");
        printf("\t\t      \n");
        printf("\t\t=========\n");
        break;

    case 7:
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t=========\n");
        break;

    case 6:
        printf("\t\t  +---+\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t=========\n");
        break;

    case 5:
        printf("\t\t  +---+\n");
        printf("\t\t  |   |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t=========\n");
        break;

    case 4:
        printf("\t\t  +---+\n");
        printf("\t\t  |   |\n");
        printf("\t\t  O   |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t=========\n");
        break;

    case 3:
        printf("\t\t  +---+\n");
        printf("\t\t  |   |\n");
        printf("\t\t  O   |\n");
        printf("\t\t  |   |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t=========\n");
        break;

    case 2:
        printf("\t\t  +---+\n");
        printf("\t\t  |   |\n");
        printf("\t\t  O   |\n");
        printf("\t\t /|\\  |\n");
        printf("\t\t      |\n");
        printf("\t\t      |\n");
        printf("\t\t=========\n");
        break;

    case 1:
        printf("\t\t  +---+\n");
        printf("\t\t  |   |\n");
        printf("\t\t  O   |\n");
        printf("\t\t /|\\  |\n");
        printf("\t\t /    |\n");
        printf("\t\t      |\n");
        printf("\t\t=========\n");
        break;

    case 0:
        printf("\t\t  +---+\n");
        printf("\t\t  |   |\n");
        printf("\t\t  O   |\n");
        printf("\t\t /|\\  |\n");
        printf("\t\t / \\  |\n");
        printf("\t\t      |\n");
        printf("\t\t💀 GAME OVER!\n");
        printf("\t\t=========\n");
        break;
    }
}

int is_valid_username(char *username)
{
    int len = strnlen(username, sizeof(username));
    if (len < MIN_USERNAME_LEN || len > MAX_USERNAME_LEN)
    {
        return INVALID_INPUT;
    }

    for (int i = 0; i < len; i++)
    {
        if (!isalpha(username[i]) || username[i] == ' ')
        {
            return INVALID_INPUT;
        }
    }

    return VALID_INPUT;
}

char *get_user_username(char *buffer, int size)
{
    do
    {
        printf("Enter a username (3-20 characters only): ");
        fgets(buffer, size, stdin);
        buffer[strlen(buffer) - 1] = '\0';
        // while (getchar() != '\n')
        //     ;
    } while (is_valid_username(buffer) == INVALID_INPUT);

    return buffer;
}

int is_valid_guess(char letter, char already_guessed[], int size)
{
    if (!isalpha(letter))
    {
        printf("\t\tInvalid input. Please try again.\n");
        return INVALID_INPUT;
    }

    for (int i = 0; i < size + 1; i++)
    {
        if (letter == already_guessed[i])
        {
            printf("Invalid input. Please try again.\n");
            return INVALID_INPUT;
        }
    }
    return VALID_INPUT;
}

short calc_score(short word_len, short lives, int correct)
{
    return word_len * correct + lives;
}

int calc_player_count(char input_str[])
{
    char *temp = malloc(strlen(input_str) + 1);
    strcpy(temp, input_str);
    int player_count = 0;

    temp = strtok(temp, ",");
    while (temp != NULL)
    {
        player_count++;
        temp = strtok(NULL, ",");
    }

    free(temp);
    return player_count;
}
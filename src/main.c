/*
server order:
- recv - username
- send - message asking for r
- recv - recieve r
- send length of word
- recieve guess
- send bool array
*/

/*
client order:
- send - username done
- recv - message asking for r  done
- send - r done
- receive length of word
- send guess
- receive bool array
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "../include/hangman_utils.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_BUFFER_SIZE 1024
#define MAX_USERNAME_LEN 21
#define MIN_USERNAME_LEN 3
#define MAX_LIVES 8
#define MAX_LETTERS 26
#define SLEEP_TIME 1

int word_len = 0;

typedef struct
{
    char username[MAX_USERNAME_LEN];
    int num_guesses;
    int lives;
} User;

typedef enum
{
    VALID_INPUT = 1,
    INVALID_INPUT = -1
} Username_Status;

typedef struct
{
    char username[MAX_USERNAME_LEN];
    int score;
} Leaderboard;

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
        printf("Invalid input. Please try again.\n");
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
    printf("Gussed Letters: ");
    for (int i = 0; i < len; i++)
    {
        printf(" %c ", guessed[i]);
    }
    printf("\n");
}

short calc_score(short word_len, short lives)
{
    return word_len + lives;
}

void display_leaderboard(char leaderboard[])
{
    printf("\t 🏆  Hangman Leaderboard 🏆\n");
    char delim[] = ",";

    char *tok = strtok(leaderboard, delim);

    while (tok != NULL)
    {
        printf("%s\n", tok);
        tok = strtok(NULL, delim);
    }
}

int main(void)
{
    User player;
    player.num_guesses = 0;
    player.lives = MAX_LIVES;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    socklen_t server_len = sizeof(server_addr);

    if (connect(sockfd, (struct sockaddr *)&server_addr, server_len) < 0)
    {
        perror("❌ Connection failed");
        exit(EXIT_FAILURE);
    };

    char buffer[MAX_BUFFER_SIZE];

    int n;

    // n = recv(sockfd, buffer, sizeof(buffer), 0);
    // if (n < 0)
    // {
    //     perror("Receive failed");
    //     exit(1);
    // }

    // printf("Server response: %s\n", buffer);

    // memset(&buffer, 0, sizeof(buffer));

    // 1. send username
    get_user_username(player.username, MAX_USERNAME_LEN);

    n = send(sockfd, player.username, strlen(player.username) + 1, 0);
    if (n < 0)
    {
        perror("❌ Username Send failed");
        exit(1);
    }

    printf("⏳ Waiting for other players to Register ...\n");

    // 2. receive all players are ready
    n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n < 0)
    {
        perror("❌ Receive failed");
        exit(1);
    }

    printf("Server response: %s\n", buffer);

    // 3. send r char
    char ready;

    do
    {
        printf("Ready up by entering 'r': ");
        ready = fgetc(stdin);
        while (getchar() != '\n')
            ;
    } while (ready != 'r');

    n = send(sockfd, &ready, 1, 0);
    if (n < 0)
    {
        perror("❌ r Send failed");
        exit(1);
    }

    printf("⏳ Waiting for other players to ready up ...\n");

    // 4. receive length of word

    n = recv(sockfd, &word_len, sizeof(word_len), 0);
    if (n < 0)
    {
        perror("❌ Receive failed");
        exit(1);
    }

    // 5. send guess
    // 6. recieve results of guess
    // Guess Loop
    char guess;
    char already_guessed[MAX_LETTERS];
    char incorrect_guess[MAX_LETTERS - word_len];
    int game_won;

    int temp_arr[word_len];
    memset(temp_arr, 0, sizeof(temp_arr));

    int final_arr[word_len];
    char letter_final_arr[word_len];
    memset(final_arr, 0, sizeof(final_arr));

    while (1)
    {
        system("clear");

        game_won = 1;

        printf("❤️ Lives remaining: %d | 🎯 Guesses: %d\n", player.lives, player.num_guesses);
        Hungman(player.lives);
        printf("\n🔷 Word Progress: ");
        display_word_progress(letter_final_arr, word_len);
        display_guessed_letters(already_guessed, player.num_guesses);

        do
        {
            printf("Enter a letter: ");
            guess = fgetc(stdin);
            while (getchar() != '\n')
                ;
        } while (is_valid_guess(guess, already_guessed, player.num_guesses) == INVALID_INPUT);

        n = send(sockfd, &guess, 1, 0);
        if (n < 0)
        {
            perror("❌ Guess Send failed");
            exit(1);
        }

        // add to already_guessed
        already_guessed[player.num_guesses] = guess;

        n = recv(sockfd, temp_arr, sizeof(temp_arr), 0);
        if (n < 0)
        {
            perror("❌ Receive failed");
            exit(1);
        }

        int correct_guess = 0;
        for (int i = 0; i < word_len; i++)
        {
            if (temp_arr[i] == 1)
            {
                correct_guess = 1;
                final_arr[i] = 1;
                letter_final_arr[i] = guess;
            }
            else
            {
                incorrect_guess[i] = guess;
            }
        }

        if (!correct_guess)
        {
            player.lives--;
            printf("❌ Incorrect guess! You lost a life.\n");
        }
        else
        {
            printf("✅ Correct guess!\n");
        }

        player.num_guesses++;

        if (player.lives <= 0)
        {
            sleep(SLEEP_TIME);
            system("clear");
            Hungman(player.lives);
            // printf("🔍 The Correct word was: ");
            display_word_progress(letter_final_arr, word_len);

            break;
        }

        for (int i = 0; i < word_len; i++)
        {
            if (final_arr[i] == 0)
            {
                game_won = 0;
                break;
            }
        }

        if (game_won == 1)
        {
            sleep(SLEEP_TIME);
            system("clear");
            printf("🎉 Congratulations, %s! You guessed the correct word! 🎉\n", player.username);
            printf("🏆 The word was: ");
            display_word_progress(letter_final_arr, word_len);
            printf("📊 Game Stats: \n Guesses: %d\n Lives Remaining: %d\n", player.num_guesses, player.lives);
            break;
        }

        // printf("Final Array: ");
        // for (int i = 0; i < word_len; i++)
        // {
        //     printf("%d,", final_arr[i]);
        // }
        // printf("\n");

        sleep(SLEEP_TIME);
    }

    printf("Waiting for all players to finish...\n");

    memset(&buffer, 0, MAX_BUFFER_SIZE);
    n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n < 0)
    {
        perror("❌ Receive failed");
        exit(1);
    }

    printf("Server response: %s\n", buffer);

    short int final_score = calc_score((short)word_len, (short)player.lives);

    n = send(sockfd, &final_score, sizeof(final_score), 0);
    if (n < 0)
    {
        perror("❌ Send Score failed");
        exit(1);
    }

    memset(&buffer, 0, MAX_BUFFER_SIZE);
    n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n < 0)
    {
        perror("❌ Receiving Leaderboard failed");
        exit(1);
    }

    display_leaderboard(buffer);

    close(sockfd);
    return 0;
}
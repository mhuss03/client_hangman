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
#include "../include/client.h"
#include "../include/display.h"

#define MAX_BUFFER_SIZE 1024
#define MAX_LIVES 8
#define MAX_LETTERS 26
#define SLEEP_TIME 650000

int word_len = 1;

int main(void)
{
    User player;
    player.num_guesses = 0;
    player.lives = MAX_LIVES;

    int client_sockfd = connect_to_server();
    handle_gameplay(client_sockfd, &player);

    char buffer[MAX_BUFFER_SIZE];
    int n;

    printf("⏳ Waiting for other players to ready up ...\n");

    // 4. receive length of word

    n = recv(client_sockfd, &word_len, sizeof(word_len), 0);
    if (n < 0)
    {
        perror("❌ Receive failed");
        exit(1);
    }

    char guess = '\0';
    char already_guessed[MAX_LETTERS] = {'\0'};
    char incorrect_guess[MAX_LETTERS - word_len];
    int game_won = 0;

    int temp_arr[word_len];
    memset(&temp_arr, 0, sizeof(temp_arr));

    int final_arr[word_len];
    char letter_final_arr[word_len];
    memset(&final_arr, 0, sizeof(final_arr));
    memset(&letter_final_arr, 0, sizeof(letter_final_arr));

    while (1)
    {
        system("clear");

        game_won = 1;

        printf("\n\t\t❤️ Lives remaining: %d | 🎯 Guesses: %d\n", player.lives, player.num_guesses);
        Hungman(player.lives);
        printf("\n\t\t🔷 Word Progress: ");
        display_word_progress(letter_final_arr, word_len);
        display_guessed_letters(already_guessed, player.num_guesses);

        do
        {
            printf("\t\tEnter a letter: ");
            guess = fgetc(stdin);
            while (getchar() != '\n')
                ;
        } while (is_valid_guess(guess, already_guessed, player.num_guesses) == INVALID_INPUT);

        n = send(client_sockfd, &guess, 1, 0);
        if (n < 0)
        {
            perror("❌ Guess Send failed");
            exit(1);
        }

        // add to already_guessed
        already_guessed[player.num_guesses] = guess;

        n = recv(client_sockfd, temp_arr, sizeof(temp_arr), 0);
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
            printf("\t\t ❌ Incorrect guess! You lost a life.\n");
        }
        else
        {
            printf("\t\t✅ Correct guess!\n");
        }

        player.num_guesses++;

        if (player.lives <= 0)
        {
            usleep(SLEEP_TIME);
            system("clear");
            Hungman(player.lives);
            game_won = 0;
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
            usleep(SLEEP_TIME);
            system("clear");
            display_stats(player.username, letter_final_arr, word_len, player.num_guesses, player.lives);
            break;
        }

        // printf("Final Array: ");
        // for (int i = 0; i < word_len; i++)
        // {
        //     printf("%d,", final_arr[i]);
        // }
        // printf("\n");

        usleep(SLEEP_TIME);
    }

    printf("Waiting for all players to finish...\n");

    memset(&buffer, 0, MAX_BUFFER_SIZE);
    n = recv(client_sockfd, buffer, sizeof(buffer), 0);
    if (n < 0)
    {
        perror("❌ Receive failed");
        exit(1);
    }

    printf("Server response: %s\n", buffer);

    short int final_score = calc_score((short)word_len, (short)player.lives, game_won);

    n = send(client_sockfd, &final_score, sizeof(final_score), 0);
    if (n < 0)
    {
        perror("❌ Send Score failed");
        exit(1);
    }

    memset(&buffer, 0, MAX_BUFFER_SIZE);
    n = recv(client_sockfd, buffer, sizeof(buffer), 0);
    if (n < 0)
    {
        perror("❌ Receiving Leaderboard failed");
        exit(1);
    }

    // printf("Buffer: %s\n", buffer);
    display_leaderboard(buffer, player.username);

    close(client_sockfd);
    return 0;
}
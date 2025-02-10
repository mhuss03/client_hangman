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

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MAX_BUFFER_SIZE 1024
#define MAX_USERNAME_LEN 21

typedef struct
{
    char username[MAX_USERNAME_LEN];
    int num_guesses;
    int lives;
} User;

int is_valid_username(char *username)
{
    int len = strnlen(username, sizeof(username));
    if (len < 3 || len > 20) //
    {
        return -1; //
    }

    for (int i = 0; i < len; i++)
    {
        if (!isalpha(username[i]) || username[i] == ' ')
        {
            return -1; //
        }
    }

    return 1;
}

char *get_user_username(char *buffer)
{
    do
    {
        // while (getchar() != '\n')
        //     ;
        printf("Enter a username (3-20 characters only): ");
        fgets(buffer, sizeof(buffer), stdin);
        printf("\n");
    } while (is_valid_username(buffer) < 0);

    return buffer;
}

int main(void)
{
    User player;
    player.num_guesses = 0;
    player.lives = 6;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    socklen_t server_len = sizeof(server_addr);

    if (connect(sockfd, (struct sockaddr *)&server_addr, server_len) < 0)
    {
        perror("Connection failed");
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
    get_user_username(player.username);

    n = send(sockfd, player.username, strlen(player.username) + 1, 0);
    if (n < 0)
    {
        perror("Username Send failed");
        exit(1);
    }

    printf("Waiting for other players to ready up ...\n");

    // 2. receive all players are ready
    n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n < 0)
    {
        perror("Receive failed");
        exit(1);
    }

    printf("Server response: %s\n", buffer);

    // 3. send r char
    char ready;

    do
    {
        fgets(&ready, 3, stdin);
        printf("\n");
    } while (ready != 'r');

    n = send(sockfd, &ready, 10, 0);
    if (n < 0)
    {
        perror("r Send failed");
        exit(1);
    }

    // 4. receive length of word
    int word_len;

    n = recv(sockfd, &word_len, sizeof(word_len), 0);
    if (n < 0)
    {
        perror("Receive failed");
        exit(1);
    }
    printf("Word Length response: %d\n", word_len);

    // 5. send guess
    // 6. recieve results of guess
    // Guess Loop
    char guess;

    int temp_arr[word_len];
    memset(temp_arr, 0, sizeof(temp_arr));

    int final_arr[word_len];
    memset(final_arr, 0, sizeof(final_arr));

    int game_won;

    while (1)
    {
        // 5 on first iteration but then 10 for subsequent
        printf("word len %d\n", word_len);

        game_won = 1;

        printf("You have %d lives remaining.\n", player.lives);
        printf("Number of guesses made: %d\n", player.num_guesses);

        do
        {
            printf("Enter a letter: ");
            fgets(&guess, 3, stdin);
        } while (!isalpha(guess));

        n = send(sockfd, &guess, 1, 0);
        if (n < 0)
        {
            perror("Guess Send failed");
            exit(1);
        }

        n = recv(sockfd, temp_arr, sizeof(temp_arr), 0);
        if (n < 0)
        {
            perror("Receive failed");
            exit(1);
        }

        int correct_guess = 0;
        for (int i = 0; i < 5; i++)
        {
            if (temp_arr[i] == 1)
            {
                correct_guess = 1;
                final_arr[i] = 1;
            }
        }

        if (!correct_guess)
        {
            player.lives--;
            printf("Incorrect guess! You lost a life.\n");
        }
        else
        {
            printf("Correct guess!\n");
        }

        player.num_guesses++;

        if (player.lives <= 0)
        {
            printf("Game Over! You have no more lives.\n");
            break;
        }

        for (int i = 0; i < 5; i++)
        {
            if (final_arr[i] == 0)
            {
                game_won = 0;
                break;
            }
        }

        if (game_won == 1)
        {
            printf("Congratulations, you have won the game!\n");
            break;
        }

        // printf("Final Array: ");
        // for (int i = 0; i < word_len; i++)
        // {
        //     printf("%d,", final_arr[i]);
        // }
        // printf("\n");
    }

    close(sockfd);
    return 0;
}
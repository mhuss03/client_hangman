#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "../include/hangman_utils.h"

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int connect_to_server()
{
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

    return sockfd;
}

void handle_gameplay(int sockfd, User *player)
{
    char buffer[MAX_BUFFER_SIZE];
    int joining_check = 0;
    int n;

    n = recv(sockfd, &joining_check, sizeof(buffer), 0);
    if (n < 0)
    {
        perror("❌ Receive failed");
        exit(1);
    }

    if (joining_check == -1)
    {
        printf("Server is full\n");
        exit(1);
    }

    printf("Welcome to Hangman!: \n\n");

    memset(&buffer, 0, sizeof(buffer));

    // 1. Send username
    get_user_username(player->username, MAX_USERNAME_LEN);
    send(sockfd, player->username, strlen(player->username) + 1, 0);

    printf("⏳ Waiting for other players to Register...\n");

    // 2. Receive all players ready
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server response: %s\n", buffer);

    // 3. Ready signal
    char ready;
    do
    {
        printf("Ready up by entering 'r': ");
        ready = fgetc(stdin);
        while (getchar() != '\n')
            ;
    } while (ready != 'r');

    send(sockfd, &ready, 1, 0);
}
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

int is_valid_username(char *username)
{
    int len = strnlen(username, sizeof(username));
    if (len < 3 || len > 20)
    {
        return -1;
    }

    for (int i = 0; i < len; i++)
    {
        if (!isalpha(username[i]) || username[i] == ' ')
        {
            return -1;
        }
    }

    return 1;
}

char *get_user_username(char *buffer)
{
    do
    {
        printf("Enter a username (3-20 characters only): ");
        fgets(buffer, sizeof(buffer), stdin);
        printf("\n");
    } while (is_valid_username(buffer) < 0);

    return buffer;
}

int main(void)
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
        perror("Connection failed");
        exit(EXIT_FAILURE);
    };

    char buffer[MAX_BUFFER_SIZE];

    int n;

    n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n < 0)
    {
        perror("Receive failed");
        exit(1);
    }

    printf("Server response: %s\n", buffer);

    memset(&buffer, 0, sizeof(buffer));
    char *username = get_user_username(buffer);

    n = send(sockfd, username, strlen(username), 0);
    if (n < 0)
    {
        perror("Send failed");
        exit(1);
    }

    close(sockfd);

    return 0;
}
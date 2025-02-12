#ifndef CLIENT_H
#define CLIENT_H

int connect_to_server();
void handle_gameplay(int sockfd, User *player);

#endif // CLIENT_H
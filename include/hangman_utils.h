#ifndef HANGMAN_UTILS_H
#define HANGMAN_UTILS_H

#define MAX_USERNAME_LEN 21
#define MIN_USERNAME_LEN 3

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
} User_Leaderboard;

void Hungman(int lives);
int is_valid_username(char *username);
char *get_user_username(char *buffer, int size);
int is_valid_guess(char letter, char already_guessed[], int size);
short calc_score(short word_len, short lives, int correct);
int calc_player_count(char input_str[]);

#endif // HANGMAN_UTILS_H

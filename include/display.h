#ifndef DISPLAY_H
#define DISPLAY_H

void display_word_progress(char letter_final_arr[], int word_len);
void display_guessed_letters(char guessed[], int len);
void display_stats(char *username, char *letter_final_arr, int word_len, int num_guesses, int lives);
void display_leaderboard(char str_leaderboard[], char username[]);

#endif // DISPLAY_H

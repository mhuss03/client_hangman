#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *input_str = "a:10,b:50,";

    char *temp = malloc(strlen(input_str) + 1);
    strcpy(temp, input_str);
    int player_count = 0;

    printf("%s\n", temp);

    temp = strtok(temp, ",");
    while (temp != NULL)
    {
        printf("%s\n", temp);
        player_count++;
        temp = strtok(NULL, ",");
    }

    printf("%d\n", player_count);

    free(temp);
    return 0;
}
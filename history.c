#include "headers.h"

// history command helper function: adds new command to the history file
void addtohistory(char *line, char* home)
{
    if (strlen(line) == 0)
        return;
    
    FILE *f = fopen("/tmp/.shell_history", "a");
    fclose(f);

    f = fopen("/tmp/.shell_history", "r");  
    if (f == NULL) {
        perror("file");
        return;
    }

    char *history[200];
    char temp[200];
    sprintf(temp, "%s\n", line);
    int index = 0;
    size_t size = 200;
    if (f != NULL)
    {
        history[index] = (char *)malloc(sizeof(char) * 200);
        while (getline(&history[index], &size, f) > 0)
        {
            if (strncmp(history[index], "\n", sizeof(history[index])) == 0) {
                continue;
            }
            else
            {
                index++;
                history[index] = (char *)malloc(sizeof(char) * 200);
            }
        }
    }
    fclose(f);

    f = fopen("/tmp/.shell_history", "w");
    int i = 0;
    if (index - 20 > 0)
        i = index - 20;
    while(i < index)
    {
        fprintf(f, "%s", history[i]);
        i++;
    }

    if (strcmp(temp, history[index - 1]) != 0 || i == 0)
    {
        fprintf(f, "%s\n", line);
    }
    for (int i = 0; i <= index; i++)
    {
        free(history[i]);
    }
    fclose(f);
}

// history command
void historyfunc(char* home)
{
    FILE *f = fopen("/tmp/.shell_history", "r");

    char **history = (char **)malloc(sizeof(char *) * 200);
    int index = 0;
    if (f != NULL)
    {
        history[index] = (char *)malloc(sizeof(char) * 200);
        size_t size = 200;
        while (getline(&history[index], &size, f) > 0)
        {
            if (strncmp(history[index], "\n", sizeof(history[index])) != 0)
            {
                index++;
                history[index] = (char *)malloc(sizeof(char) * 200);
            }
        }
    }
    fclose(f);
    int i = 0;
    if (index - 10 > 0)
        i = index - 10;
    for (; i < index; i++)
    {
        printf("%d %s", i, history[i]);
        free(history[i]);
    }
    return;
}
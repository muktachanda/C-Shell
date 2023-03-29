#include "headers.h"

void piped(int pipec, char* line, char command[], char *params[], char *path, char *home, char *and)
{
    char **commands;
    commands = (char**) malloc(sizeof(char*) * 10);
    char *partpointer;
    int i = 0, pid;

    // parse the line into various commands
    char temp[100];
    strcpy(temp, line);
    partpointer = strtok(line, "|");
    while (partpointer != NULL)
    {
        commands[i] = (char*)malloc(sizeof(char) * 1000);
        commands[i] = strdup(partpointer);
        i++;
        partpointer = strtok(NULL, "|");
    }
    strcpy(line, temp);
    commands[i] = NULL;
    int numcommands = i;

    int p[pipec][2];
    for(int i = 0; i < pipec; i++)
    {
        f = pipe(p[i]);
        if(f != 0)
        {
            perror("pipe");
            return;
        }
    }

    for(int i = 0; i < numcommands; i++)
    {
        if(commands[i][0] = ' ')
        {
            commands[i]++;
        }
        if(commands[i][strlen(commands[i])-1] == ' ')
        {
            commands[i][strlen(commands[i])-1] == '\0';
        }
    }

    for(int i = 0; i < numcommands; i++)
    {
        char* temp = commands[i];
        command = strtok(temp, " ");
        strcpy(commands[i], temp);

        pid = fork();
        
        if(pid == 0)
        {
            if(i < pipec)
            {
                dup2(p[i][1], 1);
            }
            if(i != 0)
            {
                close(p[i-1][1]);
                dup2(p[i-1][0], 0);
            }
            checkcommand(path, home, command, params, commands[i]);
            exit(1);
        }
    }

    for(int i = 0; i < pipec; i++)
    {
        close(p[i][0]);
        close(p[i][1]);
    }

    for(int i = 0; i < pipec; i++)
    {
        if(i == pipec - 1)
        {
            int code;
            wait(&code);
            code = WEXITSTATUS(code);
        }
        else
        {
            wait(NULL);
        }
    }

}
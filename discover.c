#include "headers.h"

// discover function helper function- finds a path to file
void findpath(char *abspath, int flag, char *file)
{
    int temp = 0;
    struct dirent **list;
    int n = scandir(abspath, &list, 0, alphasort);

    for(; temp < n; temp++)
    {
        char *name = list[temp]->d_name;
        char currpath[1000];
        strcpy(currpath, "");
        strcpy(currpath, abspath);
        strcat(currpath, "/");
        strcat(currpath, name);
        if (isfile(currpath) == 0 && name[0] != '.')
        {
            if(strcmp(file, name) == 0 && flag != 1)
            {
                printf("%s\n", currpath);
            }
            findpath(currpath, flag, file);
        }
        else if (isfile(currpath) == 1)
        {
            if (strcmp(currpath, ".") != 0 && name[0] != '.' && strcmp(file, name) == 0 && flag != 2)
            {
                printf("%s\n", currpath);
            }
        }
    }
}

// discover function helper function- shows all files
// flag: 0- no flag, 1- just files, 2- just directories, 3- both
void recursiveprint(int flag, char *abspath)
{
    int temp = 0;
    struct dirent **list;
    int n = scandir(abspath, &list, 0, alphasort);

    for(; temp < n; temp++)
    {
        char *name = list[temp]->d_name;
        char currpath[1000];
        strcpy(currpath, abspath);
        strcat(currpath, "/");
        strcat(currpath, name);

        if (isfile(currpath) == 0 && name[0] != '.')
        {
            if (flag != 1)
            {
                printf("%s\n", currpath);
            }
            recursiveprint(flag, currpath);
        }
        else if (isfile(name) == 1 && flag != 2 && name[0] != '.')
            printf("%s\n", currpath);
    }
}

// discover command
void discover(char *params[], char *home)
{
    char abspath[100];
    strcpy(abspath, ".");
    int flag = 0;

    // discover
    if (params[1] == NULL)
    {
        printf("%s\n", abspath);
        recursiveprint(0, abspath);
        return;
    }

    char dir[30];
    strcpy(dir, "");
    char file[30];
    strcpy(file, "");

    for (int i = 1; params[i] != NULL; i++)
    {
        if (strstr(params[i], "-d"))
        {
            flag += 2;
        }
        else if (strstr(params[i], "-f"))
        {
            flag += 1;
        }
        else if (strstr(params[i], "\""))
        {
            char temp[strlen(params[i]) + 1];
            int p = 0;
            for (int j = 1; j < strlen(params[i]) - 1; j++)
            {
                temp[p++] = params[i][j];
            }
            temp[p] = '\0';
            strcpy(file, temp);
        }
        else
        {
            if (strstr(params[i], "./"))
            {
                strcpy(abspath, params[i]);
            }
            else
            {
                strcpy(dir, params[i]);
                strcpy(abspath, params[i]);
            }
        }
    }
    if (strcmp(dir, "") != 0)
    {
        if (strcmp(dir, "~") == 0)
        {
            strcpy(abspath, home);
            printf("%s\n", home);
        }
    }

    if (strcmp(file, "") == 0)
    {
        if (flag == 0 || flag == 2 || flag == 3)
            printf("%s\n", abspath);
        recursiveprint(flag, abspath);
    }
    else
    {
        findpath(abspath, flag, file);
    }
    return;
}
#include "headers.h"

// cd command
void cdfunc(char *params[], char *prevpath, char *home)
{
    char cwd[100];
    g = getcwd(cwd, sizeof(cwd));
    if (g == NULL) {
        perror("cwd");
        return;
    }

    // if no path is given, go to home directory
    if (params[1] == NULL)
    {
        char cwd[100];
        g = getcwd(cwd, sizeof(cwd));
        if (g == NULL) {
            perror("cwd");
            return;
        }
        strcpy(prevpath, cwd);
        char username[50];
        f = getlogin_r(username, sizeof(username));
        if (f == -1) {
            perror("username");
            return;
        }

        f = chdir(home);
        if (f == -1) {
            perror("chdir");
            return;
        }
        return;
    }
    char *param = params[1];
    char dir[100];

    // if the path is ~/something
    if (strstr(param, "~") != NULL && strlen(param) > 1)
    {
        char temp[100];
        int index = 0;
        int i;
        for (i = 0; i < sizeof(param); i++)
        {
            if (param[i] == '~')
            {
                break;
            }
        }
        strcpy(temp, home);
        index = strlen(temp);
        for (int j = 1; j < sizeof(params[1]); j++)
        {
            temp[index] = param[j];
            index++;
        }
        temp[index] = '\0';
        f = chdir(temp);
        if (f == -1) {
            perror("chdir");
            return;
        }
        return;
    }

    // cd . => stay in the same directory
    if (strcmp(param, ".") == 0)
    {
        char cwd[100];
        g = getcwd(cwd, sizeof(cwd));
        if (g == NULL) {
            perror("cwd");
            return;
        }

        f = chdir(cwd);
        if (f == -1) {
            perror("chdir");
            return;
        }
        strcpy(prevpath, cwd);
        return;
    }

    // cd .. => go to the directory before it
    if (strcmp(param, "..") == 0)
    {
        int i = strlen(cwd);
        while (i >= 0 && cwd[i] != '/')
        {
            i--;
        }

        // if the path is more than just /home
        if (i > 0)
        {
            strncpy(dir, cwd, i);
            dir[i] = '\0';
            f = chdir(dir);
            if (f == -1) {
                perror("chdir");
                return;
            }
            strcpy(prevpath, cwd);
            return;
        }

        // if the path is just /home go to / (root)
        else if (i == 0 && cwd[i + 1] != '\0')
        {
            strcpy(dir, "/");
            f = chdir(dir);
            if (f == -1) {
                perror("chdir");
                return;
            }
            strcpy(prevpath, cwd);
            return;
        }
    }

    // cd - => print the previous working directory and switch to it
    else if (strcmp(param, "-") == 0)
    {
        printf("%s\n", prevpath);
        f = chdir(prevpath);
        if (f == -1) {
            perror("chdir");
            return;
        }
        strcpy(prevpath, cwd);
        return;
    }

    // cd ~ => switch to home directory
    else if (strcmp(param, "~") == 0)
    {
        char cwd[100];
        g = getcwd(cwd, sizeof(cwd));
        if (g == NULL)
        {
            perror("cwd");
            return;
        }
        f = chdir(home);
        if (f == -1) {
            perror("chdir");
            return;
        }
        strcpy(prevpath, cwd);
        return;
    }

    DIR *directory = opendir(param);
    if (directory == NULL)
    {
        perror("read");
        return;
    }
    else
    {
        f = chdir(param);
        if (f == -1) {
            perror("chdir");
            return;
        }
        strcpy(prevpath, cwd);
    }
}
#include "headers.h"

// ls command helper function- returns 0 if directory, 1 if file
int isfile(const char *name)
{
    DIR *dr = opendir(name);
    if (dr != NULL)
    {
        closedir(dr);
        return 0;
    }
    // if(errno == ENOTDIR)
    //     return 1;
    return 1;
}

// ls command helper function 2 - qsort compare
static int myCompare(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

// ls command helper function- returns a list of files and dir
// hidden 1 means show hidden files info also
void lsshort(char *dir, int hidden)
{
    if (isfile(dir) == 1)
    {
        printf("%s\n", dir);
        return;
    }
    struct dirent **list;
    int index = 0, n;

    n = scandir(dir, &list, 0, alphasort);
    int temp = 0;
    struct stat b;
    while (temp < n)
    {
        char *name = list[temp]->d_name;
        stat(name, &b);
        if ((hidden == 0 && strcmp(name, "..") != 0 && strcmp(name, ".") != 0 && name[0] != '.') || hidden == 1)
        {
            if ((b.st_mode & S_IXOTH) && isfile(name) != 0)
                printf("\033[0;32m"); // executable is green
            else
            {
                if (isfile(name) == 1)
                    printf("\033[0;37m"); // file is white
                else
                    printf("\033[0;34m"); // dr is blue
            }
            printf("%s ", name);
        }
        free(list[temp]);
        temp++;
    }
    free(list);
    printf("\033[0;37m");
    printf("\n");
}

// ls command
void lsfunc(char *params[], char *home)
{
    struct dirent **list;
    int index = 0, n;
    char cwd[100];
    g = getcwd(cwd, sizeof(cwd));
    if (g == NULL) {
        perror("cwd");
        return;
    }
    n = scandir(cwd, &list, 0, alphasort);
    int temp = 0;

    // ls => shows info of cwd
    if (params[1] == NULL)
    {
        lsshort(cwd, 0);
    }

    for (int i = 1; params[i] != NULL; i++)
    {
        // ls .. => prints the info of the parent working directory
        char t[100];
        if (strcmp(params[i], "..") == 0 && params[i + 1] == NULL)
        {
            int i = strlen(cwd);
            while (i >= 0 && cwd[i] != '/')
            {
                i--;
            }
            char dir[100];
            if (i > 0)
            {
                strncpy(dir, cwd, i);
                dir[i] = '\0';
            }
            lsshort(dir, 0);
            return;
        }

        // ls . => shows info of cwd
        if (strcmp(params[i], ".") == 0 && params[i + 1] == NULL)
        {
            lsshort(cwd, 0);
            return;
        }

        // ls -a => shows info of cwd with hidden files included
        else if (strcmp(params[i], "-a") == 0 && params[i + 1] == NULL)
        {
            lsshort(cwd, 1);
            return;
        }

        // ls -l => shows extra info of cwd
        // perms|hard links|user|group|bytes|month|date|time|filename
        else if (strcmp(params[i], "-l") == 0 && params[i + 1] == NULL)
        {
            struct dirent **list;
            int temp = 0;
            int n = scandir(cwd, &list, 0, alphasort);

            // total file system blocks the current directory takes
            struct stat b;
            printf("total ");
            int f;
            stat(cwd, &b);
            long int blocks = 0;
            while (temp < n)
            {
                char *name = list[temp]->d_name;
                if ((strcmp(name, "..") != 0 && strcmp(name, ".") != 0 && name[0] != '.'))
                {
                    struct stat b;
                    stat(name, &b);
                    blocks += b.st_blocks;
                }
                temp++;
            }
            blocks /= 2;
            printf("%ld\n", blocks);
            temp = 0;

            // for each file in the directory
            while (temp < n)
            {
                char *name = list[temp]->d_name;
                if ((strcmp(name, "..") != 0 && strcmp(name, ".") != 0 && name[0] != '.'))
                {
                    struct stat b;
                    stat(name, &b);

                    // perms
                    printf((isfile(name) == 1) ? "-" : "d");
                    printf((b.st_mode & S_IRUSR) ? "r" : "-");
                    printf((b.st_mode & S_IWUSR) ? "w" : "-");
                    printf((b.st_mode & S_IXUSR) ? "x" : "-");
                    printf((b.st_mode & S_IRGRP) ? "r" : "-");
                    printf((b.st_mode & S_IWGRP) ? "w" : "-");
                    printf((b.st_mode & S_IXGRP) ? "x" : "-");
                    printf((b.st_mode & S_IROTH) ? "r" : "-");
                    printf((b.st_mode & S_IWOTH) ? "w" : "-");
                    printf((b.st_mode & S_IXOTH) ? "x " : "- ");

                    // number of hard links inside
                    printf("%ld ", b.st_nlink);

                    // user name
                    struct passwd *g;
                    g = getpwuid(b.st_uid);
                    if (g == NULL) {
                        perror("pwuid user");
                        return;
                    }
                    printf("%s ", g->pw_name);

                    // group name
                    g = getpwuid(b.st_gid);
                    if (g == NULL) {
                        perror("pwuid group");
                        return;
                    }
                    printf("%s ", g->pw_name);

                    // byte size
                    printf("%5ld ", b.st_size);

                    // date and time
                    char date[12];
                    strftime(date, 20, "%b %2d %2d:%2d ", localtime(&(b.st_mtime)));
                    printf("%s ", date);

                    // file name
                    if ((b.st_mode & S_IXOTH) && isfile(name) != 0)
                        printf("\033[0;32m"); // executable is green
                    else
                    {
                        if (isfile(name) == 1)
                            printf("\033[0;37m"); // file is white
                        else
                            printf("\033[0;34m"); // dr is blue
                    }
                    printf("%s\n", name);
                    printf("\033[0;37m");
                }
                free(list[temp]);
                temp++;
            }
            free(list);
            return;
        }

        // ls ~ => prints the info of the home directory
        if (strcmp(params[i], "~") == 0 && params[i + 1] == NULL)
        {
            f = chdir(home);
            if (f == -1)
            {
                perror("chdir");
                return;
            }
            lsshort(home, 0);
            f = chdir(cwd);
            if (f == -1)
            {
                perror("chdir");
                return;
            }
            return;
        }

        // ls -a -l / ls -la / ls -al => prints info of all including hidden
        if ((strcmp(params[i], "-a") == 0 && params[i + 1] != NULL && strcmp(params[i + 1], "-l") == 0 && params[i + 2] == NULL) || (params[i + 1] == NULL && strcmp(params[i], "-al") == 0) || (strcmp(params[i], "-la") == 0 && params[i + 1] == NULL) || (strcmp(params[i], "-l") == 0 && params[i + 2] == NULL && strcmp(params[i + 1], "-a") == 0))
        {
            struct dirent **list;
            int temp = 0;
            int n = scandir(cwd, &list, 0, alphasort);

            // total file system blocks the current directory takes
            struct stat b;
            printf("total ");
            stat(cwd, &b);
            long int blocks = 0;
            while (temp < n)
            {
                char *name = list[temp]->d_name;
                struct stat b;
                stat(name, &b);
                blocks += b.st_blocks;
                temp++;
            }
            blocks /= 2;
            printf("%ld\n", blocks);
            temp = 0;

            // for each file in the directory
            while (temp < n)
            {
                char *name = list[temp]->d_name;
                struct stat b;
                stat(name, &b);

                // perms
                printf((isfile(name) == 1) ? "-" : "d");
                printf((b.st_mode & S_IRUSR) ? "r" : "-");
                printf((b.st_mode & S_IWUSR) ? "w" : "-");
                printf((b.st_mode & S_IXUSR) ? "x" : "-");
                printf((b.st_mode & S_IRGRP) ? "r" : "-");
                printf((b.st_mode & S_IWGRP) ? "w" : "-");
                printf((b.st_mode & S_IXGRP) ? "x" : "-");
                printf((b.st_mode & S_IROTH) ? "r" : "-");
                printf((b.st_mode & S_IWOTH) ? "w" : "-");
                printf((b.st_mode & S_IXOTH) ? "x " : "- ");

                // number of hard links inside
                printf("%ld ", b.st_nlink);

                // user name
                struct passwd *g;
                g = getpwuid(b.st_uid);
                if (g == NULL)
                {
                    perror("pwuid user");
                    return;
                }
                printf("%s ", g->pw_name);

                // group name
                g = getpwuid(b.st_gid);
                if (g == NULL)
                {
                    perror("pwuid group");
                    return;
                }
                printf("%s ", g->pw_name);

                // byte size
                printf("%5ld ", b.st_size);

                // date and time
                char date[12];
                strftime(date, 20, "%b %2d %2d:%2d ", localtime(&(b.st_mtime)));
                printf("%s ", date);

                // file name
                if ((b.st_mode & S_IXOTH) && isfile(name) != 0)
                    printf("\033[0;32m"); // executable is green
                else
                {
                    if (isfile(name) == 1)
                        printf("\033[0;37m"); // file is white
                    else
                        printf("\033[0;34m"); // dr is blue
                }
                printf("%s\n", name);
                printf("\033[0;37m");
                free(list[temp]);
                temp++;
            }
            free(list);
            return;
        }

        // ls <directory_name> / ls <directory_path>
        if (isfile(params[i]) != 1 && params[i + 1] == NULL)
        {
            f = chdir(params[i]);
            if (f == -1)
            {
                perror("chdir");
                return;
            }
            char temp[100];
            getcwd(temp, 100);
            lsshort(temp, 0);
            f = chdir(cwd);
            if (f == -1)
            {
                perror("chdir");
                return;
            }
        }

        // ls <file_name>
        else if (isfile(params[i]) == 1 && params[i + 1] == NULL)
        {
            if (strstr(params[i], "/") == NULL)
                printf("%s\n", params[i]);
            else
            {
                int j;
                for (j = strlen(params[i]) - 1; j >= 0; j--)
                {
                    if (params[i][j] == '/')
                    {
                        break;
                    }
                }
                j++;
                char file[40];
                int index = 0;
                for (int k = j; k < strlen(params[i]); k++)
                {
                    file[index++] = params[i][k];
                }
                file[index] = '\0';
                printf("%s\n", file);
            }
        }

        // ls <flags> <directory/file_name>
        // ls <dir_1> -l <dir_2> -a <file>
        if (params[i + 1] != NULL)
        {
            char** flag = (char **)malloc(sizeof(char *) * 5);
            char** arg = (char **)malloc(sizeof(char *) * 10);
            
            int flagi = 0;
            int argi = 0;
            int p = 1;
            while (params[p] != NULL)
            {
                if (strstr(params[p], "-") != NULL) 
                {
                    flag[flagi] = (char *)malloc(sizeof(char) * 10);
                    strcpy(flag[flagi++], params[p]);
                }
                else 
                {
                    arg[argi] = (char *)malloc(sizeof(char) * 60);
                    strcpy(arg[argi++], params[p]);
                }
                p++;
            }
            flag[flagi] = NULL;
            arg[argi] = NULL;

            // sort the arguments
            qsort(arg, argi, sizeof(char *), myCompare);

            int flags = 0;
            // just -a : 1, just -l: 2; both -a and -l: 3
            for (int j = 0; j < flagi; j++)
            {
                if (strstr(flag[j], "-la") || strstr(flag[j], "-al"))
                {
                    flags = 3;
                }
                else if (strstr(flag[j], "-a"))
                {
                    flags++;
                }
                else if (strstr(flag[j], "-l"))
                    flags += 2;
            }

            int hidden = 0;
            if (flags == 1 || flags == 3)
                hidden = 1;

            argi--;
            int k;
            for (k = 0; k <= argi; k++)
            {
                struct dirent **list;
                int temp = 0;

                // no flags
                if (flags == 0)
                {
                    if (isfile(arg[k]) == 1)
                    {
                        if(k != 0) 
                        {
                            printf("\n%s\n", arg[k]);
                        }
                        else
                        {
                            printf("%s\n", arg[k]);
                        }
                    }
                    else
                    {
                        if (k > 0)
                            printf("\n");
                        if (argi >= 1)
                            printf("%s: \n", arg[k]);
                        char temp[100];
                        getcwd(temp, 100);
                        strcat(temp, "/");
                        strcat(temp, arg[k]);
                        f = chdir(temp);
                        if (f == -1)
                        {
                            perror("chdir");
                            return;
                        }
                        lsshort(temp, 0);
                        f = chdir(cwd);
                        if (f == -1)
                        {
                            perror("chdir");
                            return;
                        }
                    }
                }

                // both -a and -l on a file
                if (isfile(arg[k]) == 1 && flags >= 2)
                {
                    if (k > 0)
                        printf("\n");
                    struct stat b;
                    stat(arg[k], &b);

                    // perms
                    printf("-");
                    printf((b.st_mode & S_IRUSR) ? "r" : "-");
                    printf((b.st_mode & S_IWUSR) ? "w" : "-");
                    printf((b.st_mode & S_IXUSR) ? "x" : "-");
                    printf((b.st_mode & S_IRGRP) ? "r" : "-");
                    printf((b.st_mode & S_IWGRP) ? "w" : "-");
                    printf((b.st_mode & S_IXGRP) ? "x" : "-");
                    printf((b.st_mode & S_IROTH) ? "r" : "-");
                    printf((b.st_mode & S_IWOTH) ? "w" : "-");
                    printf((b.st_mode & S_IXOTH) ? "x " : "- ");

                    // number of hard links inside
                    printf("%ld ", b.st_nlink);

                    // user name
                    struct passwd *g;
                    g = getpwuid(b.st_uid);
                    if (g == NULL)
                    {
                        perror("pwuid user");
                        return;
                    }
                    printf("%s ", g->pw_name);

                    // group name
                    g = getpwuid(b.st_gid);
                    if (g == NULL)
                    {
                        perror("pwuid group");
                        return;
                    }
                    printf("%s ", g->pw_name);

                    // byte size
                    printf("%5ld ", b.st_size);

                    // date and time
                    char date[12];
                    strftime(date, 20, "%b %2d %2d:%2d ", localtime(&(b.st_mtime)));
                    printf("%s ", date);

                    // file name
                    if ((b.st_mode & S_IXOTH) && isfile(arg[k]) != 0)
                        printf("\033[0;32m"); // executable is green
                    else
                    {
                        if (isfile(arg[k]) == 1)
                            printf("\033[0;37m"); // file is white
                        else
                            printf("\033[0;34m"); // dr is blue
                    }
                    printf("%s", arg[k]);
                    printf("\033[0;37m");
                    printf("\033[0;37m");
                    printf("\n");
                    continue;
                }
                // only -a
                else if (flags == 1)
                {
                    // on a file
                    if (k > 0)
                        printf("\n");
                    if (isfile(arg[k]) == 1)
                    {
                        printf("%s", arg[k]);
                        printf("\033[0;37m");
                        continue;
                    }
                    // on a dir
                    else
                    {
                        if (k > 0)
                            printf("\n");
                        if (argi >= 1)
                        {
                            printf("%s: \n", arg[k]);
                        }
                        f = chdir(arg[k]);
                        if (f == -1)
                        {
                            perror("chdir");
                            return;
                        }
                        char temp[100];
                        getcwd(temp, 100);
                        lsshort(temp, 1);
                        f = chdir(cwd);
                        if (f == -1)
                        {
                            perror("chdir");
                            return;
                        }
                        continue;
                    }
                }
                // both -l and -a on a dir and just -l on the dir
                else if (isfile(arg[k]) == 0 && flags >= 2)
                {
                    if (k > 0)
                        printf("\n");
                    if (arg[1] != NULL)
                    {
                        printf("\033[0;37m");
                        printf("%s:\n", arg[k]);
                    }
                    int n = scandir(arg[k], &list, 0, alphasort);

                    if (flags == 1)
                    {
                        f = chdir(arg[k]);
                        if (f == -1)
                        {
                            perror("chdir");
                            return;
                        }
                        lsshort(arg[k], 1);
                        f = chdir(cwd);
                        if (f == -1)
                        {
                            perror("chdir");
                            return;
                        }
                        continue;
                    }

                    // total file system blocks the current directory takes
                    f = chdir(arg[k]);
                    if (f == -1)
                    {
                        perror("chdir");
                        return;
                    }

                    struct stat b;
                    printf("\033[0;37m");
                    printf("total ");
                    int f;
                    stat(arg[k], &b);

                    long int blocks = 0;
                    while (temp < n)
                    {
                        char *name = list[temp]->d_name;
                        if ((hidden == 0 && strcmp(name, "..") != 0 && strcmp(name, ".") != 0 && name[0] != '.') || hidden == 1)
                        {
                            struct stat b;
                            stat(name, &b);
                            blocks += b.st_blocks;
                        }
                        temp++;
                    }
                    blocks /= 2;
                    printf("%ld\n", blocks);
                    temp = 0;

                    // for each file in the directory
                    while (temp < n)
                    {
                        char *name = list[temp]->d_name;
                        if ((hidden == 0 && strcmp(name, "..") != 0 && strcmp(name, ".") != 0 && name[0] != '.') || hidden == 1)
                        {
                            struct stat b;
                            stat(name, &b);

                            // perms
                            printf((isfile(name) == 1) ? "-" : "d");
                            printf((b.st_mode & S_IRUSR) ? "r" : "-");
                            printf((b.st_mode & S_IWUSR) ? "w" : "-");
                            printf((b.st_mode & S_IXUSR) ? "x" : "-");
                            printf((b.st_mode & S_IRGRP) ? "r" : "-");
                            printf((b.st_mode & S_IWGRP) ? "w" : "-");
                            printf((b.st_mode & S_IXGRP) ? "x" : "-");
                            printf((b.st_mode & S_IROTH) ? "r" : "-");
                            printf((b.st_mode & S_IWOTH) ? "w" : "-");
                            printf((b.st_mode & S_IXOTH) ? "x " : "- ");

                            // number of hard links inside
                            printf("%ld ", b.st_nlink);

                            // user name
                            struct passwd *g;
                            g = getpwuid(b.st_uid);
                            if (g == NULL) {
                                perror("pwuid user");
                                return;
                            }
                            printf("%s ", g->pw_name);

                            // group name
                            g = getpwuid(b.st_gid);
                            if (g == NULL) {
                                perror("pwuid group");
                                return;
                            }
                            printf("%s ", g->pw_name);

                            // byte size
                            printf("%5ld ", b.st_size);

                            // date and time
                            char date[12];
                            strftime(date, 20, "%b %2d %2d:%2d ", localtime(&(b.st_mtime)));
                            printf("%s ", date);

                            // file name
                            if ((b.st_mode & S_IXOTH) && isfile(name) != 0)
                                printf("\033[0;32m"); // executable is green
                            else
                            {
                                if (isfile(name) == 1)
                                    printf("\033[0;37m"); // file is white
                                else
                                    printf("\033[0;34m"); // dr is blue
                            }
                            printf("%s\n", name);
                            printf("\033[0;37m");
                        }
                        free(list[temp]);
                        temp++;
                    }
                    free(list);
                    f = chdir(cwd);
                    if (f == -1)
                    {
                        perror("chdir");
                        return;
                    }
                }
            }

            return;
        }
    }
}
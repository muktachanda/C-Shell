#include "headers.h"

// check what command needs to be run now
void checkcommand(char *path, char *home, char *command, char **params, char* line)
{

    if(strstr(inbuiltcommands, command) && (strstr(line, "<") || strstr(line, ">")))
    {
        int j = 0;
        while(params[j] != NULL)
        {
            if(strcmp(params[j], ">") == 0 || strcmp(params[j], ">>") == 0 || strcmp(params[j], "<") == 0)
            {
                params[j] = NULL;
                break;
            }
            j++;
        }
    }

    if (strcmp(command, "-1") == 0)
    {
        exit(0);
    }

    else if (strcmp(command, "cd") == 0)
    {
        cdfunc(params, path, home);
    }

    else if (strcmp(command, "echo") == 0)
    {
        echofunc(params);
    }

    else if (strcmp(command, "pwd") == 0)
    {
        pwdfunc(params);
    }

    else if (strcmp(command, "ls") == 0)
    {
        lsfunc(params, home);
    }

    else if (strcmp(command, "pinfo") == 0)
    {
        pinfofunc(params, home);
    }

    else if (strcmp(command, "history") == 0)
    {
        historyfunc(home);
    }

    else if (strcmp(command, "discover") == 0)
    {
        discover(params, home);
    }
    else if (strcmp(command, "jobs") == 0)
    {
        int fl = 0;
        if(params[1] != NULL && !strcmp(params[1], "-r"))
        {
            fl = 1;
        }
        else if(params[1] != NULL && !strcmp(params[1], "-s"))
        {
            fl = 2;
        }
        jobs(fl);
    }
    else if(strcmp(command, "sig") == 0)
    {
        if(params[1] != NULL)
        {
            int pid;
            if(atoi(params[1]) > count)
            {
                perror("job doesn't exist");
                return;
            }
            for(int i = 0; i < count; i++) {
                if(jobsarray[i] == atoi(params[1])-1)
                {
                    pid = pidarray[i];
                    break;
                }
            }
            int signal = atoi(params[2]);
            kill(pid, signal);
        }
        else
        {
            perror("no job entered");
            return;
        }
    }
    else if(strcmp(command, "fg") == 0)
    {
        int job = atoi(params[1]);
        if(job > count) {
            perror("job doesn't exist");
            return;
        }
        fg(job);
    }
    else if(strcmp(command, "bg") == 0)
    {
        int job = atoi(params[1]);
        if(job > count) {
            perror("job doesn't exist");
            return;
        }
        bg(job);
    }
    else 
    {
        if(strstr(line, "<"))
        {
            char temp[1000];
            int index = 0;
            for(int i = 0; i < strlen(line); i++)
            {
                if(line[i] != '<' && line[i] != '>')
                {
                    temp[index++] = line[i];
                }
                if(line[i] == '>')
                {
                    temp[index] = '\0';
                    break;
                }
            }
            temp[index] = '\0';
            strcpy(line, temp);
        }
        setpgid(0, 0);
        foreground(line);
    }

    // assignment 3 spec 1: redirect < and >
    if(inputf)
    {
        close(inputf);
        dup2(stdintemp, 0);
        close(stdintemp);
    }
    if(outputf)
    {
        close(outputf);
        dup2(stdouttemp, 1);
        close(stdouttemp);
    }
}

// display the prompt of the shell for every line
void displayprompt(char *path, char *cwd)
{

    // clear the screen when the file is run first
    static int first = 1;
    if (first == 1)
    {
        const char *CLEAR_SCREEN = " \e[1;1H\e[2J";
        write(STDOUT_FILENO, CLEAR_SCREEN, 12);
        first = 0;
    }

    char username[60];
    f = getlogin_r(username, sizeof(username));
    if (f == -1)
    {
        perror("username");
        return;
    }

    char systemname[40];
    f = gethostname(systemname, sizeof(systemname));
    if (f == -1) {
        perror("systemname");
        return;
    }

    // replace cwd with ~
    char currdir1[40];
    int in = 1;

    if (strstr(path, cwd) != NULL && strcmp(path, "/") != 0)
    {
        currdir1[0] = '~';
        for (int i = strlen(cwd); i < strlen(path); i++)
        {
            currdir1[in] = path[i];
            in++;
        }
        currdir1[in] = '\0';
    }
    else
    {
        strcpy(currdir1, path);
    }

    // display the prompt with appropriate coloring
    char sys[50];
    printf("\033[0;37m"); // white
    strcpy(sys, "<");
    printf("%s", sys);
    strcpy(sys, username);
    strcat(sys, "@");
    strcat(sys, systemname);
    printf("\033[0;32m"); // green
    printf("%s", sys);
    printf("\033[0;37m"); // white
    strcpy(sys, ":");
    printf("%s", sys);
    strcpy(sys, currdir1);
    printf("\033[0;34m"); // blue
    printf("%s", sys);
    strcpy(sys, "> ");
    printf("\033[0;37m"); // white
    printf("%s", sys);

    if (tflag > 1)
    {
        printf("\033[0;34m"); // blue
        printf("took %ds", tflag);
        printf("\033[0;37m"); // white
        printf(">");
        tflag = 0;
    }
}

// read the command the user enters
int read_command(char line[], char command[], char *params[], char *path, char *home, char *and)
{
    int index = 0, i = 0;
    int num = 0;
    char *partpointer;
    char *part[500];
    and[0] = '!';
    and[1] = '\0';
    int aindex = 1;
    int redirect = 0;
    inputf = 0;
    outputf = 0;
    int pipec = 0;

    // read line
    while (1)
    {
        int c = fgetc(stdin);
        if (c == '\n')
        {
            and[aindex] = index;
            aindex++;
            and[aindex] = '\0';
            break;
        }
        if (c == ';')
        {
            num++;
            and[aindex] = index;
            aindex++;
            and[aindex] = '\0';
            break;
        }
        if (c == '&')
        {
            // num++;
            and[aindex] = index;
            aindex++;
            bflag = 1;
        }
        if(c == '|')
        {
            pipec++;
        }
        line[index] = c;
        index++;
    }

    line[index] = '\0';
    addtohistory(line, home);
    if (index == 0)
    {
        return -1;
    }

    if(pipec > 0)
    {
        piped(pipec, line, command, params, path, home, and);
        return 0;
    }

    // parse the line into words
    char temp[100];
    strcpy(temp, line);
    partpointer = strtok(line, " \t \n");
    while (partpointer != NULL)
    {
        part[i] = strdup(partpointer);
        i++;
        partpointer = strtok(NULL, " \t \n");
    }
    strcpy(line, temp);

    // first word of the line is the shell command
    strcpy(command, part[0]);

    // other words of the line are the parameters
    int j;
    for (j = 0; j < i; j++)
    {
        params[j] = part[j];
    }
    params[j] = NULL;

    if (bflag == 1)
    {
        andfunc(line, and);
    }

    j = 0;
    while(params[j] != NULL)
    {
        if(strcmp(part[j], "<") == 0)
        {
            inputf = open(part[j+1], O_RDONLY);
            if(inputf == -1)
            {
                perror("opening input file");
                return 0;
            }
        }
        else if(strcmp(part[j], ">") == 0)
        {
            outputf = open(part[j+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(outputf == -1)
            {
                perror("opening output file");
                return 0;
            }
        }
        else if(strcmp(part[j], ">>") == 0)
        {
            outputf = open(part[j+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if(outputf == -1)
            {
                perror("opening output file");
                return 0;
            }
        }
        j++;
    }

    if(inputf)
    {
        stdintemp = dup(0);
        dup2(inputf, 0);
    }
    if(outputf)
    {
        stdouttemp = dup(1);
        dup2(outputf, 1);
    }
    return num;
}

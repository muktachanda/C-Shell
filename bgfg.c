#include "headers.h"

// background processes function
void background(char *commands)
{
    int j = 0;
    char *partpointer;
    char *part[100];
    // parse the line into words
    partpointer = strtok(commands, " \t \n");
    while (partpointer != NULL)
    {
        part[j] = strdup(partpointer);
        j++;
        partpointer = strtok(NULL, " \t \n");
    }
    part[j] = NULL;

    pid_t pid = fork();
    int status;

    if (pid == 0)
    {
        setpgid(0, 0);
        if (execvp(part[0], part) < 0)
            perror("execvp");
    }
    else
    {
        waitpid(pid, &status, WNOHANG);
        printf("[%d] %d\n", count + 1, pid);
        bgcommands[count] = (char *)malloc(sizeof(char) * 200);
        strcpy(bgcommands[count], part[0]);
        pidarray[count] = (int)pid;
        count++;
    }
}

// background process helper function
void backgroundcheck()
{
    int status;
    int pid = waitpid(-1, &status, WNOHANG);
    int i;
    if (pid > 0)
    {
        for (i = 0; pidarray[i] != (int)pid; i++)
        {
            continue;
        }

        if (WIFSTOPPED(status))
            return;

        char exit[20];
        if (WIFEXITED(status))
            strcpy(exit, "normally");
        else
            strcpy(exit, "abnormally");

        sprintf(temp[tindex++], "%s with pid %d exited %s\n", bgcommands[i], pidarray[i], exit);
    }
}

// foreground processes function
void foreground(char *commands)
{
    int j = 0;
    time_t timer1;
    timer1 = time(NULL);
    char *partpointer;
    char *part[100];
    // parse the line into words
    partpointer = strtok(commands, " \t \n");
    while (partpointer != NULL)
    {
        part[j] = strdup(partpointer);
        j++;
        partpointer = strtok(NULL, " \t \n");
    }
    part[j] = NULL;

    pid_t pid = fork();
    int status;

    if (pid == 0)
    {
        if (execvp(part[0], part) < 0) {
            perror("execvp");
            return;
        }
    }
    else
    {
        waitpid(pid, &status, 0);
        time_t timer2 = time(NULL);
        tflag = timer2 - timer1;
    }
}

// and command
void andfunc(char *line, char *and)
{
    char **c = (char **)malloc(sizeof(char *));
    int cindex = 0;

    if (strlen(and) > 1)
    {
        int j = 0;
        while (and[j + 1] != '\0')
        {
            char temp[50];
            int tempindex = 0;
            int start = and[j] + 1;
            c[cindex] = (char *)malloc(sizeof(char) * 100);
            if (and[j] == '!')
                start = 0;
            int end = and[j + 1] - 1;
            for (int i = start; i <= end; i++)
            {
                temp[tempindex++] = line[i];
            }
            temp[tempindex] = '\0';
            strcpy(c[cindex], temp);
            cindex++;
            strcpy(temp, "");
            j++;
        }
        c[cindex] = NULL;
    }

    int j = 0;
    while (c[j] != NULL)
    {
        if (c[j + 1] == NULL)
            foreground(c[j]);
        else
            background(c[j]);
        j++;
    }
}

// sort jobs alphabetically
void alphasortbg()
{
    int t1;
    char temp[500];
    for(int i = 0; i < count; i++) {
        for(int j = 0; j < count - i - 1; j++) 
        {
            if(strcmp(bgcommands[j], bgcommands[j+1]) > 0)
            {
                strcpy(temp, bgcommands[j]);
                strcpy(bgcommands[j], bgcommands[j+1]);
                strcpy(bgcommands[j+1], temp);
                t1 = jobsarray[j];
                jobsarray[j] = jobsarray[j+1];
                jobsarray[j+1] = t1;
                t1 = pidarray[j];
                pidarray[j] = pidarray[j+1];
                pidarray[j+1] = t1;
            }
        }
    }
}

// bg command
void bg(int job)
{
    alphasortbg();
    int pid;
    for(int i = 0; i < count; i++)
    {
        if(jobsarray[i] == job-1)
        {
            pid = pidarray[i];
            break;
        }
    }
    kill(pid, SIGCONT);
}

// fg command
void fg(int job)
{
    alphasortbg();
    int pid;
    for(int i = 0; i < count; i++)
    {
        if(jobsarray[i] == job-1)
        {
            pid = pidarray[i];
            break;
        }
    }

    setpgid(pid, getpgid(0));
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    tcsetpgrp(0, pid);
    f = kill(pid, SIGCONT);
    if(f < 0)
    {
        perror("can't run bg process in fg");
        return;
    }

    int status;
    waitpid(pid, &status, WUNTRACED);
    
    tcsetpgrp(0, getpgid(0));
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
}

#include "headers.h"


// echo command
void echofunc(char *params[])
{
    for (int i = 1; params[i] != NULL; i++)
    {
        if (strcmp(params[i], "&") == 0 || strcmp(params[i], ";") == 0)
        {
            break;
        }
        printf("%s ", params[i]);
    }
    printf("\n");
}

// pwd command
void pwdfunc(char *params[])
{
    char currdir[40];
    g = getcwd(currdir, sizeof(currdir));
    if (g == NULL) {
        perror("cwd");
        return;
    }
    printf("%s\n", currdir);
}

// pinfo command
// process id | status | memory | executable path
void pinfofunc(char *params[], char *home)
{
    int check = 0;
    // process id
    int pid = 0;
    if (params[1] == NULL)
    {
        pid = getpid();
    }
    else
    {
        pid = (int)atoi(params[1]);
    }

    // status + memory
    char file[50];
    sprintf(file, "/proc/%d/stat", pid);
    char filestat[1000] = {'\0'};
    FILE *process = fopen(file, "r");
    if (process == NULL) {
        perror("process status");
        return;
    }
    fread(filestat, 1000, 1, process);
    fclose(process);

    char *info[1000] = {NULL};
    int index = 0;
    info[index] = strtok(filestat, " ");
    index++;
    while (info[index - 1] != NULL)
    {
        info[index++] = strtok(NULL, " ");
    }
    info[index] = NULL;

    int m = 0;
    while(m <= count) {
        if(pidarray[m] == pid) {
            check = 1;
            break;
        }
        m++;
    }

    if (strcmp(info[4], info[7]) == 0 && check == 0)
        strcat(info[2], "+");
    printf("pid : %d\nprocess Status : %s\nmemory : %s\n", pid, info[2], info[22]);

    // executable path
    sprintf(file, "/proc/%d/exe", pid);
    char exec[4097] = {'\0'};
    int readstatus = readlink(file, exec, 4097);
    if (readstatus == -1) {
        perror("readstatus");
        return;
    }

    if (strstr(exec, home))
    {
        char cwd[1024];
        int index = 1;
        cwd[0] = '~';
        int i = strlen(home);
        while (i < strlen(exec))
        {
            cwd[index++] = exec[i];
            i++;
        }
        cwd[index] = '\0';
        printf("executable path: %s\n", cwd);
    }
    else
        printf("executable path: %s\n", exec);
    return;
}

// jobs command
// prints list of all jobs
void jobs(int flag)
{
    char f[50];
    char stat[200];
    char status[8];

    for(int i = 0; i < count; i++)
    {
        alphasortbg();
        if(!pidarray[i])
        {
            continue;
        }
        sprintf(f, "/proc/%d/stat", pidarray[i]);
        FILE* procstatus = fopen(f, "r");
        if(procstatus == NULL)
        {
            strcpy(status, "Stopped");
            if(flag == 1)
            {
                continue;
            }
            printf("[%d] %s %s [%d]\n", jobsarray[i]+1, status, bgcommands[i], pidarray[i]);
            continue;
        }
        fread(stat, 100, 1, procstatus);
        fclose(procstatus);

        char* temp = strtok(stat, " ");
        temp = strtok(NULL, " ");
        temp = strtok(NULL, " ");

        if(temp[0] == 'S')
            strcpy(status, "Running");
        else
            strcpy(status, "Stopped");

        if(flag == 1)
        {
            if((!strcmp(status, "Stopped")))
                continue;
        }
        else if (flag == 2)
        {
            if((!strcmp(status, "Running")))
                continue;
        }
        printf("[%d] %s %s [%d]\n", jobsarray[i]+1, status, bgcommands[i], pidarray[i]);
    }
}

// sig command
void sig(int pid)
{
    for(int i = 0; i < count; i++){
        if(pidarray[i] == pid)
        {
            kill(pid, SIGINT);
            return;
        }
    }
}
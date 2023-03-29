#include "headers.h"

int f;
char *g;
int bflag = 0;
int pidarray[10000];
char *bgcommands[10000];
int jobsarray[10000];
char temp[200][200];
char inbuiltcommands[1000];
int tindex = 0;
int count = 0;
int tflag = 0;
int traverser = 0;
int inputf, outputf, stdintemp, stdouttemp;

// main running function
int main(int argc, char **argv)
{
    char prevpath[100], home[100];
    char path[100];
    getcwd(path, sizeof(prevpath));
    getcwd(prevpath, sizeof(prevpath));
    strcpy(home, prevpath);
    int num = -2;
    char *history[21];
    int hindex = 0;

    strcpy(inbuiltcommands, "-1cdechopwdlspinfohistorydiscoverjobssigfgbg");

    for(int i = 0; i < 10000; i++) {
        jobsarray[i] = i;
    }

    signal(SIGCHLD, backgroundcheck);

    // infinite loop so the shell can run
    while (1)
    {
        char line[1000];
        char command[100];
        strcpy(command, "");
        char *params[50];
        char currdir[40];
        g = getcwd(currdir, sizeof(currdir));
        if (g == NULL) {
            perror("cwd");
            return 0;
        }

        if (num <= 0)
        {
            displayprompt(currdir, path);
        }
        else
        {
            num--;
        }

        char and[10];
        and[0] = '\0';

        // exit if Ctrl-D
        /*
        if(fgets(line, 999, stdin) == 0)
        {
            printf("\n");
            break;
        }
        */

        // check the command only if it is not empty
        num = read_command(line, command, params, prevpath, home, and);

        if (num != -1 && bflag == 0)
        {
            checkcommand(prevpath, home, command, params, line);
        }
        else
        {
            bflag = 0;
        }
        for (int i = traverser; temp[i][0] != '\0'; i++)
        {
            printf("%s", temp[traverser++]);
        }

        
    }
    return 0;
}
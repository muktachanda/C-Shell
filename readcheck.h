#ifndef _READCHECK_H_
#define _READCHECK_H_

extern int f;
extern char *g;
extern int bflag;
extern int pidarray[10000];
extern char *bgcommands[10000];
extern char temp[200][200];
extern int tindex;

extern int count;
extern int tflag;
extern int traverser;
extern int inputf, outputf, stdintemp, stdouttemp;

void checkcommand(char *path, char *home, char *command, char **params, char* line);

void displayprompt(char *path, char *cwd);

int read_command(char line[], char command[], char *params[], char *path, char *home, char *and);

#endif
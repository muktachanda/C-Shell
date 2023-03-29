#ifndef __FUNC_H_
#define __FUNC_H_

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

void echofunc(char *params[]);

void pwdfunc(char *params[]);

void pinfofunc(char *params[], char *home);

void redirect(char* params[]);

void jobs(int flag);

void sig(int pid);

#endif

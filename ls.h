#ifndef __LS_H_
#define __LS_H_

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

int isfile(const char *name);

static int myCompare(const void *a, const void *b);

void lsshort(char *dir, int hidden);

void lsfunc(char *params[], char *home);

#endif
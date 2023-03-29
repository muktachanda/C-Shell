#ifndef __DISCOVER_H_
#define __DISCOVER_H_

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

void findpath(char *abspath, int flag, char *file);

void recursiveprint(int flag, char *abspath);

void discover(char *params[], char *home);

#endif
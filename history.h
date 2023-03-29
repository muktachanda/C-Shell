#ifndef __HISTORY_H_
#define __HISTORY_H_

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

void addtohistory(char *line, char* home);

void historyfunc(char* home);

#endif
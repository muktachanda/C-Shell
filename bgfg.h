#ifndef _HELPERS_H
#define _HELPERS_H

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

void background(char *commands);

void backgroundcheck();

void andfunc(char *line, char *and);

void foreground(char *commands);

void alphasortbg();

void fg(int job);

void bg(int job);

#endif
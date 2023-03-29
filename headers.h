#ifndef __HEADERS_H_
#define __HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <time.h>
#include <assert.h>
#include <signal.h>
#include <termios.h>
#include "cd.h"
#include "discover.h"
#include "func.h"
#include "history.h"
#include "ls.h"
#include "readcheck.h"
#include "bgfg.h"
#include "pipe.h"


extern int f;
extern char *g;
extern int bflag;
extern int pidarray[10000];
extern char *bgcommands[10000];
extern int jobsarray[10000];
extern char temp[200][200];
extern int tindex;
extern char inbuiltcommands[1000];

extern int count;
extern int tflag;
extern int traverser;
extern int inputf, outputfd, stdintemp, stdouttemp;

int alphasort(const struct dirent **a, const struct dirent **b);

#endif
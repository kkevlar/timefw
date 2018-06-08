#ifndef A_H
#define A_H

#define _BSD_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>

#ifdef DEBUG

#include "assert.h"
#include <stdio.h>
#define a(args) assert(args)
#define p(...) (fprintf(stderr, __VA_ARGS__))

#else

#define a(args)
#define p(...)

#endif

#endif 

#pragma once

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR(x)              #x
#define XSTR(x)             STR(x)
#define ARRLEN(a)           (sizeof(a)/sizeof(a[0]))
#define STREQ(s1, s2)       (strncmp(s1, s2, ARRLEN(s1)) == 0)
#define FAIL_(m, ...)       {fprintf(stderr, __FILE__ ":" XSTR(__LINE__) ": " m ": %s (%d)\n",  __VA_ARGS__);}
#define FAIL(r, ...)        {FAIL_(__VA_ARGS__, strerror(r), r); return r < 0 ? -r : r;}
#define WRITES(fd,s)        write(fd, s, sizeof(s))

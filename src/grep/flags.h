#ifndef FLAGS_H
#define FLAGS_H
#define _GNU_SOURCE

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags {
  int e, i, v, c, l, n, h, s, f, o;
  int pattern_length;
  int pattern_size;
  char *pattern;
} flag;

int pat_add(flag *flags, char *pattern);
void get_pat(flag *flags, char *filepath);
int parcer(flag *flags, int argc, char **argv);

#endif
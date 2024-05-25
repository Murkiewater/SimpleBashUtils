#ifndef CAT_H
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct opt {
  int b;
  int e;
  int n;
  int s;
  int E;
  int t;
  int T;
  int v;
} opt_choice;

int parser(int argc, char **argv, opt_choice *opt);

int printfile(char *filename, opt_choice *opt, int *str_count,
              int *empty_str_count, int *flag, int file_count);

static void print_invalid_opt();

#endif
#ifndef OUT_H
#define OUT_H
#define _GNU_SOURCE

#include <regex.h>

#include "flags.h"

void print_line_by_char(char *line);
void print_match(regex_t *reg, char *line, int line_count, flag flags,
                 char *path);
int file_est(char *filename);
void print_line(flag flags, regex_t *reg, char *line, int lines_count,
                char *path);
void print_file(flag flags, char *path, regex_t *reg);
void out(flag flags, int argc, char **argv);

#endif
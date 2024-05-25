#include "flags.h"

int pat_add(flag *flags, char *pattern) {
  if (!pattern) return 1;
  int n = strlen(pattern);
  if (flags->pattern_length == 0) {
    flags->pattern = NULL;
    flags->pattern = malloc(1024 * sizeof(char));
    flags->pattern_size = 1024;
  }
  if (flags->pattern_size < flags->pattern_length + n) {
    flags->pattern = realloc(flags->pattern, flags->pattern_size * 2);
    flags->pattern_size *= 2;
  }
  if (flags->pattern_length != 0) {
    strcat(flags->pattern + flags->pattern_length, "|");
    flags->pattern_length++;
  }
  flags->pattern_length +=
      sprintf(flags->pattern + flags->pattern_length, "%s", pattern);
  return 0;
}

void get_pat(flag *flags, char *filepath) {
  FILE *f = fopen(filepath, "r");
  if (f == NULL) {
    if (!flags->s) perror(filepath);
    return;
  }
  char *line = NULL;
  size_t memline = 0;
  int read = getline(&line, &memline, f);
  while (read != -1) {
    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }
    pat_add(flags, line);
    read = getline(&line, &memline, f);
  }
  free(line);
  fclose(f);
}

int parcer(flag *flags, int argc, char **argv) {
  flags->pattern_length = 0;
  flags->pattern = NULL;
  int opt = 0, is_error = 0;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1 && !is_error) {
    switch (opt) {
      case 'e':
        flags->e = 1;
        if (pat_add(flags, optarg) != 0) is_error = 1;
        break;
      case 'i':
        flags->i = REG_ICASE;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        get_pat(flags, optarg);
        break;
      case 'o':
        flags->o = 1;
        break;
      case '?':
      default:
        is_error = 1;
        break;
    }
  }
  if (flags->pattern_length == 0 && !is_error) {
    pat_add(flags, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) flags->h = 1;
  return is_error;
}

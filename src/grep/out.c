#include "out.h"

void print_line_by_char(char *line) {
  int n = (int)strlen(line);
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') putchar('\n');
}

void print_match(regex_t *reg, char *line, int lines_count, flag flags,
                 char *path) {
  regmatch_t math;
  int offset = 0;
  int result = 0;
  while ((result = regexec(reg, line + offset, 1, &math, 0)) == 0) {
    if (math.rm_eo && !flags.h) printf("%s:", path);
    if (math.rm_eo > 0 && flags.n) {
      printf("%d:", lines_count);
    }
    for (int i = math.rm_so + offset; i < math.rm_eo + offset; i++) {
      putchar(line[i]);
    }
    putchar('\n');
    offset += math.rm_eo;
  }
}

int file_est(char *filename) {
  int is_est = 1;
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    is_est = 0;
  } else {
    fclose(file);
  }
  return is_est;
}

void print_line(flag flag, regex_t *reg, char *line, int lines_count,
                char *path) {
  if (!flag.c && !flag.l) {
    if (!flag.h && !flag.o) printf("%s:", path);
    if (flag.n && !flag.o) printf("%d:", lines_count);
    if (flag.o)
      print_match(reg, line, lines_count, flag, path);
    else {
      print_line_by_char(line);
    }
  }
}

void print_file(flag flag, char *path, regex_t *reg) {
  FILE *file = fopen(path, "r");
  size_t line_size = 0;
  char *line = NULL;
  int lines_count = 1;
  int matching_lines_count = 0;
  int read = getline(&line, &line_size, file);
  while (read != -1) {
    int result = regexec(reg, line, 0, NULL, 0);

    if ((result == 0 && !flag.v) || (flag.v && result != 0)) {
      print_line(flag, reg, line, lines_count, path);
      matching_lines_count++;
    }
    read = getline(&line, &line_size, file);
    lines_count++;
  }
  free(line);

  if (flag.c && !flag.l) {
    if (!flag.h) printf("%s:", path);
    printf("%d\n", matching_lines_count);
  }
  if (flag.l && matching_lines_count > 0) printf("%s\n", path);
  fclose(file);
}

void out(flag flags, int argc, char **argv) {
  regex_t reg;
  int is_error =
      regcomp(&reg, flags.pattern, REG_EXTENDED | REG_NEWLINE | flags.i);
  if (is_error) {
    printf("%s\n", flags.pattern);
    perror("Error during regex compiling");
  } else {
    for (int i = optind; i < argc; i++) {
      if (file_est(argv[i])) {
        print_file(flags, argv[i], &reg);
      } else if (!flags.s) {
        perror(argv[i]);
      } else {
        continue;
      }
    }
  }
  regfree(&reg);
}

#include "cat.h"
#define OPTIONS_END -1

static void print_invalid_opt() {
  fprintf(stderr, "Try cat --help for more info.");
  exit(EXIT_FAILURE);
}

int printfile(char *filename, opt_choice *opt, int *str_count,
              int *empty_str_count, int *flag, int file_count) {
  int ret = 0;
  int is_first = 1;
  FILE *f = fopen(filename, "r");
  if (f != NULL) {
    int cur_c;
    char last_c = '\n';
    while ((cur_c = fgetc(f)) != EOF) {
      if (opt->s) {  // squeeze-blank
        if (last_c == '\n' && cur_c == '\n') {
          (*flag)++;
          if (*flag >= 2) {
            continue;
          }
        } else if (last_c != '\n' && cur_c == '\n') {
          *flag = 0;
        }
        if (is_first && file_count > 1 && cur_c == '\n') (*flag)--;
      }
      if (opt->b && (last_c == '\n' && cur_c != '\n')) {
        printf("%6d\t", (*empty_str_count)++);
      } else if (opt->n && last_c == '\n') {
        printf("%6d\t", (*str_count)++);
      }
      if (opt->E && cur_c == '\n') {  // show-ends
        putchar('$');
      }
      if (opt->T && (cur_c == '\t')) {  // show-tabs
        putchar('^');
        cur_c = 'I';
      }
      if (opt->v) {  // show-nonprinting
        if (cur_c < 32 && cur_c != '\t' && cur_c != '\n') {
          printf("^%c", cur_c + 64);
        } else if (cur_c == 127) {
          printf("^?");
        } else if (cur_c >= 128 && cur_c <= 159) {
          printf("M-^%c", (cur_c - 128 + 64));
        } else if (cur_c >= 160) {
          printf("M-%c", cur_c - 128);
        } else if ((cur_c >= 32 && cur_c < 127) || cur_c == 9 || cur_c == 10) {
          putchar(cur_c);
        }
      } else {
        putchar(cur_c);
      }
      last_c = cur_c;
    }
    fclose(f);
  } else {
    ret = -1;
  }
  return ret;
}

int parser(int argc, char **argv, opt_choice *opt) {
  int key, ret = 0, index = 0;
  static struct option options[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };
  while ((key = getopt_long(argc, argv, "bEnsTvet", options, &index)) != -1) {
    switch (key) {
      case 'b':
        opt->b = 1;
        break;
      case 'e':
        opt->v = 1;
        opt->E = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 't':
        opt->T = 1;
        opt->v = 1;
        break;
      case 'E':
        opt->E = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 'T':
        opt->T = 1;
        break;
      default:
        print_invalid_opt();
        break;
    }
  }
  return ret;
}

int main(int argc, char **argv) {
  int rez = 1;
  int flag = 0;
  int str_count = 1;
  int empty_str_count = 1;
  opt_choice opt = {
      .b = 0,
      .e = 0,
      .E = 0,
      .n = 0,
      .s = 0,
      .t = 0,
      .v = 0,
      .T = 0,
  };

  rez = parser(argc, argv, &opt);
  if (opt.b) {
    opt.n = 0;
  }
  for (int i = 1; i < argc; i++) {
    printfile(argv[i], &opt, &str_count, &empty_str_count, &flag, i - optind);
  }
  return rez;
}
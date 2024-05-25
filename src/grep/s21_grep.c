#include "flags.h"
#include "out.h"

int main(int argc, char **argv) {
  flag flags = {0};
  if (parcer(&flags, argc, argv) == 0 && flags.pattern != NULL) {
    out(flags, argc, argv);
    free(flags.pattern);
  }
  return 0;
}
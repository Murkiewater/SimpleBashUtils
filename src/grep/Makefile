CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11
EXECUTABLE=s21_grep

TEST_DIR=test_files/

WORK: all clean tests test_leaks

all: $(EXECUTABLE)

$(EXECUTABLE): flags.o s21_grep.o out.o
	@$(CC) $(CFLAGS) $^ -o $@

s21_grep.o: s21_grep.c
	@$(CC) $(CFLAGS) -c $^ -o $@

out.o: out.c
	@$(CC) $(CFLAGS) -c $^ -o $@

flags.o: flags.c
	@$(CC) $(CFLAGS) -c $^ -o $@

tests: $(EXECUTABLE)
	@bash tests.sh

test_leaks: $(EXECUTABLE)
	@bash test_leaks.sh

clean:
	@rm -f $(EXECUTABLE) *.o *.log
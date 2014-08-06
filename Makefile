#
# Makefile
#
# Computer Science 50
# Problem Set 3
#

fifteen: fifteen.c fifteen.h random_board.c random_board.h
	clang -ggdb3 -O0 -std=c99 -Wall -Werror -o fifteen fifteen.c random_board.c -lcs50 -lm

clean:
	rm -f *.o a.out core fifteen

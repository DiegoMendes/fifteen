#
# Makefile
#
# Computer Science 50
# Problem Set 3
#

fifteen: fifteen.c fifteen.h random_board.c random_board.h solver.c solver.h a_star.c a_star.h
	clang -ggdb3 -O0 -std=c99 -Wall -Werror -o fifteen fifteen.c random_board.c solver.c a_star.c -lcs50 -lm

fifteen_debug: fifteen.c fifteen.h random_board.c random_board.h solver.c solver.h a_star.c a_star.h
	clang -ggdb3 -O0 -std=c99 -Wall -Werror -D DEBUG -o fifteen fifteen.c random_board.c solver.c a_star.c -lcs50 -lm

test: test.c fifteen.h a_star.c a_star.h solver.c solver.h random_board.c random_board.h
	clang -ggdb3 -O0 -std=c99 -Wall -Werror -o test test.c a_star.c solver.c random_board.c -lcs50 -lm

test_debug: test.c fifteen.h a_star.c a_star.h solver.c solver.h random_board.c random_board.h
	clang -ggdb3 -O0 -std=c99 -Wall -Werror -D DEBUG -o test test.c a_star.c solver.c random_board.c -lcs50 -lm

clean:
	rm -f *.o a.out core fifteen test

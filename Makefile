##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile for setting up the project
##

NAME = workguard
CFLAGS = -Wall -Wextra -Werror -g3

all: $(NAME)

$(NAME):
	gcc src/*.c -o $(NAME)

clean:
	rm -f a.out
	rm -f $(NAME)

fclean: clean
	-rm -f *.gcov
	-rm -f *.gcda
	-rm -f *.gcno
	-rm -f *.out
	-rm -f unit_tests
	-rm -f coverage.json

re : fclean all

unit_tests: fclean all
	gcc src/lib/* src/modules/*.c \
	src/modules/algo/*.c tests/*.c \
	-lcriterion -o unit_tests --coverage

tests_run: unit_tests
	./unit_tests
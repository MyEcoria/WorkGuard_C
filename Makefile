##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile for setting up the project
##

NAME = workguard
DEBUG = -Wall -Wextra -Werror -g3
OPTIMISATION = -O3 -Ofast -march=native -flto \
	-funroll-loops -fomit-frame-pointer

all: $(NAME)

$(NAME):
	gcc src/main.c \
	src/modules/epollo.c \
	src/modules/http.c \
	src/modules/socket.c \
	src/modules/utils.c \
	-o $(NAME) $(OPTIMISATION)

debug: fclean
	gcc src/*.c src/modules/*.c -o $(NAME) $(DEBUG)

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
	-rm -rf lmdb
	-rm -f lib/liblmdb.a
	-rm -f include/lmdb.h
	-rm -f test

re : fclean all

test:
	-git clone https://github.com/LMDB/lmdb.git
	cd lmdb/libraries/liblmdb && make && cd ../../..
	cp lmdb/libraries/liblmdb/liblmdb.a lib/liblmdb.a
	cp lmdb/libraries/liblmdb/lmdb.h include/lmdb.h
	gcc test.c lib/* -o test
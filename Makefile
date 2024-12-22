##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile for setting up the project
##

NAME = workguard
DEBUG = -Wall -Wextra -Werror -g3
OPTIMISATION = -O3 -Ofast -march=native -flto \
	-funroll-loops -fomit-frame-pointer -pthread

all: $(NAME)

$(NAME): lmdb h2o
	gcc src/main.c \
	src/modules/http/h2o.c \
	src/modules/db/db.c \
	src/modules/db/utils_db.c \
	lib/liblmdb.a \
	lib/libh2o.a \
	-luv -lssl -lcrypto -lz -lm -lpthread \
	-o $(NAME) $(OPTIMISATION)

debug: fclean
	gcc src/*.c src/modules/http/*.c -o $(NAME) $(DEBUG)

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
	-rm -f test
	-rm -rf data

re : fclean all

lmdb:
	-git clone https://github.com/LMDB/lmdb.git
	cd lmdb/libraries/liblmdb && make && cd ../../..
	cp lmdb/libraries/liblmdb/liblmdb.a lib/liblmdb.a
	cp lmdb/libraries/liblmdb/lmdb.h include/lmdb.h

lib_clean:
	-rm -rf lmdb
	-rm -f lib/liblmdb.a
	-rm -f include/lmdb.h
	-rm -rf data
	-rm -rf h2o
	-rm -f lib/libh2o.a
	-rm -f include/h2o.h
	-rm -rf include/h2o

h2o:
	-git clone --recursive https://github.com/MyEcoria/h2o.git
	cd h2o && cmake . && make && cd ..
	cp h2o/libh2o_static.a lib/libh2o.a
	cp -r h2o/include/* include/

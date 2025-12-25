#!/usr/bin/env sh

cc -Wall -Wextra -Wshadow -pedantic -std=c89 -fsanitize=address -g -DDEBUG -O0 ./libqp.c -o ./prog

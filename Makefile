# This file uses the gnu make syntax

CFLAGS += -Wall -Wextra -pedantic -std=c89 -g
LDFLAGS += 
NAME = test

.PHONY: all
all: $(NAME)

$(NAME): test.o
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o $@

test.o: test.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(NAME) test.o

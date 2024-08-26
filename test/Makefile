CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lfly -lpng -L../ -I../ -Ifly

test: test.c ../libfly.a
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f test

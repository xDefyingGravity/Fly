CC = clang
CFLAGS = -Wall -Wextra -std=c99 -Wunused-variable -I.
LDFLAGS = -lpng
SRC = fly.c main.c
OBJ = $(SRC:.c=.o)

all: fly

fly: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

lib: $(OBJ)
	ar rcs libfly.a $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f fly libfly.a $(OBJ)

test:
	$(CC) $(CFLAGS) -o fly_test test/test.c fly.c -I. -lpng

.PHONY: clean all fly lib test

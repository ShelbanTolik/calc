CC = gcc
CFLAGS = -Wall -Wextra -O2 -Ilib -Isrs -Iconstants -D_GNU_SOURCE -lm
SRC = srs/main.c srs/input/input.c lib/mongoose.c
BIN = webcalc

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(BIN)

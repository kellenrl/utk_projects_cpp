
CC = g++
CFLAGS = -Wall
BIN = friendgraph1 friendgraph2 friendgraph3

all: $(BIN)

.cpp: 
	$(CC) $(CFLAGS) -o $* $<

clean:
	@rm -f $(BIN)
	@rm -f doknow?.txt mightknow?.txt

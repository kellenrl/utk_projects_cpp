
CC = g++
CFLAGS = -Wall
BIN = FruitInventory1 FruitInventory2 
OBJ = $(BIN:%=%.o)

all: $(BIN)

.cpp: 
	$(CC) $(CFLAGS) -o $* $<

clean:
	@rm -f $(BIN) $(OBJ)

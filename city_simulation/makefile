
CC = g++
CFLAGS = -g -Wall
BIN = Citysim2

all: $(BIN)

.cpp: 
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BIN) 
	rm -f city_info.txt 
	rm -f city_distancetable.txt 
	rm -f city_timetable.txt 
	rm -f city_graph.txt 

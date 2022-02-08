CC = g++
CFLAGS = -Wall

all: Stega

Crypto: Stega.cpp PPM.h PPM.o Rnumgen.h Rnumgen.o
	$(CC) $(CFLAGS) -o $@ Stega.cpp PPM.o Rnumgen.o

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f Stega PPM.o Rnumgen.o
	rm -f *_wmsg.ppm

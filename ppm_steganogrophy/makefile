CC = g++
CFLAGS = -Wall

all: Crypto 

Crypto: Crypto.cpp PPM.h PPM.o Rnumgen.h Rnumgen.o
	$(CC) $(CFLAGS) -o $@ Crypto.cpp PPM.o Rnumgen.o

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f Crypto PPM.o Rnumgen.o
	rm -f *_wmsg.ppm

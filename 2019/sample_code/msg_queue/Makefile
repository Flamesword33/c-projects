
all: etest stest

etest: etest.c msgq.o
	$(CC) -o etest etest.c msgq.o

stest: stest.c msgq.o
	$(CC) -o stest stest.c msgq.o

msgq.o: msgq.c msgq.h

clean:
	rm etest stest msgq.o

CC=gcc
CFLAGS=-Wall

forkd: forkd.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm forkd

PTH_CONFIG=./pth-2.0.7-preempt/pth-config

CC=gcc
CFLAGS=-Wall -Wextra -g -std=gnu99 $(shell $(PTH_CONFIG) --cflags)
LDFLAGS=$(shell $(PTH_CONFIG) --ldflags) -Wl,-rpath,$(shell $(PTH_CONFIG) --libdir) -static
LIBS=$(shell $(PTH_CONFIG) --libs)

all: pth clean test

clean:
	rm -f test *.o core

test: test.o
#	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(CC) -o $@ $^ pth-2.0.7-preempt/lib/libpth.a

pth:
	cd pth-2.0.7-preempt; \
		make; \
		make install

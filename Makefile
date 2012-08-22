PTH_CONFIG=./pth-2.0.7-preempt/pth-config

CC=gcc
CFLAGS=-Wall -Wextra -g -std=gnu99 $(shell $(PTH_CONFIG) --cflags)
LDFLAGS=$(shell $(PTH_CONFIG) --ldflags) -Wl,-rpath,$(shell $(PTH_CONFIG) --libdir) -static
LIBS=$(shell $(PTH_CONFIG) --libs)

all: test

clean:
	rm -f test *.o core

test: test.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

pth:
	./build-pth.sh --debug

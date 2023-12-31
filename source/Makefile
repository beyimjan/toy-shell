CC = gcc

ifneq (build, $(MAKECMDGOALS))
CFLAGS = -g -Wall -Wextra -DDEBUG_LEVEL=0
endif

SRCS = $(filter-out toy-shell.c, $(wildcard *.c))
OBJS = $(SRCS:.c=.o)

PROG = toy-shell

all: $(PROG)

build: clean $(PROG)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCS)
	$(CC) -MM $^ > $@

$(PROG): toy-shell.c $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -rf *.o $(PROG) deps.mk

.PHONY = all build clean

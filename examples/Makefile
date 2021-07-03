CC = gcc
INCL = -I../include
DBG = -g
LIB = ../build/cnetwork.a
OPT = -O3
WARN = -Wall -Wextra

ARCHIVER = ar

SRCS := $(shell find src -name *.c)
SRCSOUT := $(patsubst src/%.c, build/%.out, $(SRCS))


.PHONY: examples
.DEFAULT_GOAL := examples

$(SRCSOUT): build/%.out : src/%.c $(LIB)
	${CC} $(OPT) ${DBG} ${INCL} $(patsubst build/%.out, src/%.c, $@) -o $@ $(LIB) 

examples: $(SRCSOUT) 

purge:
	rm -r build
	mkdir build
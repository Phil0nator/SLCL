CC = gcc
INCL = -Iinclude
DBG = -g
OUT = build/slcl.a
LIB = 
ARCHIVER = ar
OPT = -Os

SRCS := $(shell find src -name *.c)
HEADERS := $(shell find src -name *.h)
SRCSOUT := $(patsubst src/%.c, build/%.o, $(SRCS))

EXAMPLES := $(shell find examples -name *.c)
EXAMPLESOUT := $(patsubst examples/%.c, examples/build/%.out, $(EXAMPLES))


.PHONY: all
.PHONY: examples
.DEFAULT_GOAL := all

$(HEADERS):
	$(echo "updated header: "$@)

$(SRCSOUT): build/%.o : src/%.c include/SLCL/%.h
	${CC} $(OPT) ${DBG} ${INCL} -c $(patsubst build/%.o, src/%.c, $@) -o $@

examples: library
	cd examples && $(MAKE) && cd ..

library : $(SRCSOUT)
	${ARCHIVER} rcs ${OUT} ${SRCSOUT} 

all: examples library

purge:
	rm -r build
	mkdir build
	cd examples && $(MAKE) purge && cd ..

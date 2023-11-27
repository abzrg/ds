EXE = bst

UNAME := $(shell uname -s)

CC = gcc
ifeq ($(UNAME), Darwin)
	CC = clang
endif

CFLAGS := -g -std=c11 -pedantic -Wall -Wextra
CFLAGS += -fdiagnostics-color=always # for scan-build colorful output

ifeq ($(UNAME), Darwin)
	ifeq ($(CC), clang)
		ASANFLAGS := -fsanitize=address -fsanitize=undefined
		CFLAGS += $(ASANFLAGS)
		LDFLAGS += -fsanitize=address
	endif
else
	ASANFLAGS := -fsanitize=address -fsanitize=undefined
	CFLAGS += $(ASANFLAGS)
	LDFLAGS += -fsanitize=address
endif


all: $(EXE)


# Weird output when used with clang and sanitizers?
valg:
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXE)


clean:
	find . -type f -executable -print | xargs rm


.PHONY: all, valg, clean
# vim: ts=4

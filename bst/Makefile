EXE = bst

CC = gcc
CFLAGS := -g -std=c11 -pedantic -Wall -Wextra
CFLAGS += -fdiagnostics-color=always # for scan-build colorful output

# # Clang sanitizers
# # address, undefined, memory,...
# ASANFLAGS := -fsanitize=address -fsanitize=undefined
# CFLAGS += $(ASANFLAGS)
# LDFLAGS := -fsanitize=address


all: $(EXE)


# Weird output when used with clang and sanitizers?
valg:
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXE)


clean:
	find . -type f -executable -print | xargs rm


.PHONY: all, valg, clean
# vim: ts=4

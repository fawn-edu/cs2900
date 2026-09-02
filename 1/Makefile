.POSIX:
.SUFFIXES:
.PHONY: clean test doc all

CC = cc
CFLAGS = -std=c23 -O2 -Wall -Wextra -pedantic

dese: dese.out
	./dese.out 0123456789ABCDEF 133457799BBCDFF1
dese.out: des_encrypt.c
	$(CC) $(CFLAGS) -o $@ des_encrypt.c
doc: 1.typ
	typst compile 1.typ
all: dese.out doc
clean:
	rm -f *.out* ?.pdf

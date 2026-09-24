.POSIX:
.SUFFIXES:
.PHONY: clean test doc all

CC = cc
CFLAGS = -std=c23 -O2 -Wall -Wextra -pedantic

desd: desd.out
	./desd.out 85E813540F0AB405 133457799BBCDFF1
desd.out: des_decrypt.c
	$(CC) $(CFLAGS) -o $@ des_decrypt.c
rsa.out: rsa.c
	$(CC) $(CFLAGS) -o $@ rsa.c
doc: 2.typ
	typst compile 2.typ
all: desd.out rsa.out doc
clean:
	rm -f *.out* ?.pdf

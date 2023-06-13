all: 1cc test

1cc: main.c
	gcc $? -o $@ -O3

test: 1cc
	./test.sh

clean:
	rm -f *.o *~ tmp* 1cc

.PHONY: test clean

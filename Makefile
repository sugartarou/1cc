all: 1cc

1cc: main.c
	gcc $? -o $@ -O3

test: 1cc
	./test.sh

clean:
	rm -f $(all) *.o *~ tmp*

.PHONY: test clean

all: compile

compile:test.c utility.c
	gcc -std=c99 -D_XOPEN_SOURCE=500 test.c utility.c
	./a.out 0 1000000 100 1000
clean:
	rm -f *~ ./a.out
time:test.c utility.c
	gcc -std=c99 -D_XOPEN_SOURCE=500 test.c utility.c
	time ./a.out 0 1000000 100 1000


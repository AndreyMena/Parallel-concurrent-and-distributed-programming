hello: hello.c
	cc -g -Wall -Wextra hello.c -o hello -pthread

all: hello lint memcheck helgrind

.PHONY: lint 
lint:
	cpplint --filter=-readability/casting hello.c

.PHONY: memcheck
memcheck:
	valgrind --quiet --tool=memcheck ./hello

.PHONY: helgrind
helgrind:
	valgrind --quiet --tool=helgrind ./hello

.PHONY: clean
clean:
	rm hello


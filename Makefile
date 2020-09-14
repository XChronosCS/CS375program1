M = main

CFLAGS= -g -Wall -Werror -std=c++14

all: $(M)

$(M): $(M).o
	g++ $(CFLAGS) $(M).o -o $(M)

$(M).o: $(M).cpp
	g++ $(CFLAGS) -c $(M).cpp -o $(M).o

.PHONY: clean run all memcheck

run: all
	./$(M)

clean:
	rm -rf *.o $(M).o $(M)

memcheck: all
	valgrind --leak-check=full ./$(M)

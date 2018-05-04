CC=g++
CFLAGS=-g -std=c++11
SRC= main.cpp FA.cpp
EXEC=NFA2DFA

default:
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)
clean:
	rm -f $(EXEC)
.PHONY: clean

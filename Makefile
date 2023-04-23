CC = g++
CFLAGS = -Wall -Wextra -std=c++17

TARGETS = main game-state

all: $(TARGETS)

main: main.o game-state.o
	$(CC) $(CFLAGS) -o main main.o game-state.o

%.o: %.cc
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGETS) *.o

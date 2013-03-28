.PHONY: all clean dep

all: dep race

dep:
	-gtags -iv

race: race.cpp main.cpp race.h
	g++ race.cpp main.cpp -g -o race

clean:
	rm *.o race -rf
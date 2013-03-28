.PHONY: all clean dep

all: dep race

dep:
	gtags -iv

race: race.cpp main.cpp

clean:
	rm *.o race
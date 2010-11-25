CC=g++

all:MyBot3 MyBot2

clean:
	rm -rf *.o MyBot2

MyBot2: MyBot2.o PlanetWars.o
MyBot3: MyBot3.o PlanetWars.o

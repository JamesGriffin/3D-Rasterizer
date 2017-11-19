LIBS = -lSDL2 -lSDL2main -lSDL2_ttf
CFLAGS = -Wall -std=c++11 -g -fopenmp -O3


SRC=$(wildcard src/*.cpp)

rasterizer: $(SRC)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

run:
	./rasterizer

build_and_run:  $(SRC)
		g++ -o rasterizer $^ $(CFLAGS) $(LIBS)
		./rasterizer

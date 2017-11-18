LIBS = -lSDL2 -lSDL2main -lSDL2_ttf
CFLAGS = -Wall -std=c++11 -O3 -g -fopenmp


SRC=$(wildcard src/*.cpp)

rasterizer: $(SRC)
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

run:
	./rasterizer

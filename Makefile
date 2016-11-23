UNAME = $(shell uname -o)
ifeq ($(OS),Windows_NT)
	EXEEXT = .exe
	COMPILE_OPT = -lfreeglut -lglu32 -lopengl32
else
	COMPILE_OPT = -lglut -lGLU -lGL
endif

OBJ=\
build/PNGTex.o\
build/lodepng.o\
build/ui.o\
build/map.o\
build/car.o\
build/trsys.o\
build/temp.o\
build/pos.o\
build/croad.o\
build/road.o\
build/main.o

all: trsys

trsys: $(OBJ)
	g++ $(OBJ) $(COMPILE_OPT) -o build/trsys

build/PNGTex.o: ui/PNGTex.cpp
	g++ -c ui/PNGTex.cpp -o build/PNGTex.o
build/lodepng.o: ui/lodepng.cpp
	g++ -c ui/lodepng.cpp -o build/lodepng.o
build/ui.o: ui/ui.cpp
	g++ -c ui/ui.cpp -o build/ui.o
build/map.o: ui/map.cpp
	g++ -c ui/map.cpp -o build/map.o
build/car.o: engine/car.cpp
	g++ -c engine/car.cpp -o build/car.o
build/trsys.o: engine/trsys.cpp
	g++ -c engine/trsys.cpp -o build/trsys.o
build/temp.o: engine/temp.cpp
	g++ -c engine/temp.cpp -o build/temp.o
build/pos.o: engine/pos.cpp
	g++ -c engine/pos.cpp -o build/pos.o
build/croad.o: engine/croad.cpp
	g++ -c engine/croad.cpp -o build/croad.o
build/road.o: engine/road.cpp
	g++ -c engine/road.cpp -o build/road.o
build/main.o: main.cpp
	g++ -c main.cpp -o build/main.o

clean:
	rm -rf build/*

run:
	./build/trsys$(EXEEXT)

test:
	valgrind --leak-check=full ./build/trsys$(EXEEXT)

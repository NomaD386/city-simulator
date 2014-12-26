UNAME = $(shell uname -o)
ifeq ($(OS),Windows_NT)
	EXEEXT = .exe
	COMPILE_OPT = -lfreeglut -lglu32 -lopengl32
else
	COMPILE_OPT = -lglut -lGLU -lGL
endif

all: trsys

trsys: *.o
	g++ *.o $(COMPILE_OPT) -o trsys

*.o: *.cpp ui/*.cpp engine/*.cpp
	g++ -c *.cpp ui/*.cpp engine/*.cpp


clean:
	rm -rf *.o trsys$(EXEEXT)

run:
	./trsys$(EXEEXT)
	
test:
	valgrind --leak-check=full ./trsys$(EXEEXT)

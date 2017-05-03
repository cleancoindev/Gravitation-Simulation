OBJECTS=Planet.o GravityPlane.o Interpret.o
OUT=test

all: main.cpp $(OBJECTS)
	g++ -Wl,-rpath,$(PREFIX)/lib -o $(OUT) --std=c++11 -O2 main.cpp $(OBJECTS) -lreindeergl -lGLEW -lGLU -lglut -lGL -lSOIL

clean:
	rm *.o $(OUT)

Planet.o: Planet.cpp
	g++ -c -o Planet.o --std=c++11 -O2 Planet.cpp

GravityPlane.o: GravityPlane.cpp
	g++ -c -o GravityPlane.o --std=c++11 -O2 GravityPlane.cpp

Interpret.o: Interpret.cpp
	g++ -c -o Interpret.o --std=c++11 -O2 Interpret.cpp

INCLUDES = -I ./src
FLAGS = -O1 -g -D_GNU_SOURCE=1 -D_THREAD_SAFE
SDL = $(shell sdl2-config --cflags)
LDLFLAGS = $(shell sdl2-config --libs)
OBJECTS = ./build/main.o ./build/entities.o ./build/vector.o

all: $(OBJECTS)
	gcc ${FLAGS} ${INCLUDES} ${SDL} ${OBJECTS} -o ./bin/asteroids $(LDLFLAGS) -lm

./build/main.o: src/main.c
	gcc ${FLAGS} ${INCLUDES} ${SDL} ./src/main.c -c -o ./build/main.o

./build/entities.o: src/entities.c
	gcc ${FLAGS} ${INCLUDES} ${SDL} ./src/entities.c -c -o ./build/entities.o

./build/vector.o: src/vector.c
	gcc ${FLAGS} ${INCLUDES} ${SDL} ./src/vector.c -c -o ./build/vector.o

clean:
	rm -rf ./build/*.o ./bin/asteroids
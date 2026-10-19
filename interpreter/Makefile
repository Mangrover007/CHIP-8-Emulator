SRCS=src/emulator.cpp src/interpreter.cpp src/main.cpp
OBJS=build/emulator.o build/interpreter.o build/main.o

main.out: $(OBJS)
	g++ \
	-lSDL2 \
	$(OBJS) \
	-o main.out

build/main.o: src/main.cpp
	g++ -c -I./include src/main.cpp -o build/main.o

build/emulator.o: src/emulator.cpp
	g++ -c -I./include src/emulator.cpp -o build/emulator.o

build/interpreter.o: src/interpreter.cpp
	g++ -c -I./include src/interpreter.cpp -o build/interpreter.o



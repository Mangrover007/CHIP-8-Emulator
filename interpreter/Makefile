OBJS=build/emulator.o build/interpreter.o

build/mkryuga.out: $(OBJS)
	g++ \
	-lSDL2 \
	$(OBJS) \
	src/main.cpp \
	-o build/mkryuga.out

build/emulator.o: include/emulator.hpp src/emulator.cpp
	g++ -c -I./include src/emulator.cpp -o build/emulator.o

build/interpreter.o: include/interpreter.hpp src/interpreter.cpp
	g++ -c -I./include src/interpreter.cpp -o build/interpreter.o



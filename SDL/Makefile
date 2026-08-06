# OBJS specifies which files to compile as part of the project
OBJS = src/SDL-input.cpp

# CXX specifies which compiler we're using
CXX = g++

# COMPILER_FLAGS: Includes your local headers and suppresses warnings
COMPILER_FLAGS = -w -I./include -std=c++23

# LOCAL_LIBS: Points directly to your local static SDL2 library
LOCAL_LIBS = ./lib/libSDL2.a

# SYSTEM_LIBS: The OS-level dependencies SDL2 needs to talk to your hardware. 
# You may need to add or remove flags here depending on exactly how your local libSDL2.a was compiled.
SYSTEM_LIBS = -pthread -ldl -lm -lrt -lX11 -lXext

# OBJ_NAME specifies the name of our executable
OBJ_NAME = ./build/input.out

# This is the target that compiles our executable
all: $(OBJS)
	mkdir -p build
	$(CXX) $(COMPILER_FLAGS) $(OBJS) -o $(OBJ_NAME) $(LOCAL_LIBS) $(SYSTEM_LIBS)

# g++ -w -I./include src/SDL-input.cpp -o input.out $(pkg-config --cflags --libs --static sdl2)


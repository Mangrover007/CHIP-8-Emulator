# OBJS specifies which files to compile as part of the project
OBJS = src/main.cpp

# CXX specifies which compiler we're using
CXX = g++

# COMPILER_FLAGS: Includes your local headers and suppresses warnings
COMPILER_FLAGS = -w -lSDL2 -lSDL2_image -std=c++23

# OBJ_NAME specifies the name of our executable
OBJ_NAME = ./build/scaled-image.out

# This is the target that compiles our executable
all: $(OBJS)
	mkdir -p build
	$(CXX) $(COMPILER_FLAGS) $(OBJS) -o $(OBJ_NAME)


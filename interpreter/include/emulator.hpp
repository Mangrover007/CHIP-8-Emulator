#pragma once

#include <SDL2/SDL.h>
#include "interpreter.hpp"


/* !! Experimental !!
 * Emulator class currently contains the chip8 class as a member
 * variable. This makes a tight coupling of the two classes.
 *
 * Try to separate both the classes, like the Austin Morlan's
 * tutorial does.
 */

class Emulator
{
public:
    Emulator();

    // main game loop
    void StartGame();

    void LoadROM(const char* filename);

    bool ProcessInput();

private:
    // initialize SDL2 video subsystem
    bool init();

    void close();

private:
    char* mTitle;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    Chip8 chip8;
};


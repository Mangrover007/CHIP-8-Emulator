#include <SDL2/SDL.h>
#include <stdio.h>

const uint SCREEN_WIDTH = 800;
const uint SCREEN_HEIGHT = 640;

// When declaring pointers, it is good practice to initialize
// them immediately.
SDL_Window* gWindow = nullptr;

// These are pointers. Images should be pointers to save memory.
SDL_Surface* gWindowSurface = nullptr;
SDL_Surface* gImageSurface = nullptr;

// In game development, it is better to write code in as modular
// of a way as you can. Make functions that are reusable.

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow(
	    "image :D",
	    SDL_WINDOWPOS_UNDEFINED,
	    SDL_WINDOWPOS_UNDEFINED,
	    SCREEN_WIDTH,
	    SCREEN_HEIGHT,
	    SDL_WINDOW_SHOWN
	);

    if (gWindow == nullptr)
    {
	printf("Error creating window: %s\n", SDL_GetError());
	return false;
    }

    gWindowSurface = SDL_GetWindowSurface(gWindow);

    return true;
}

bool loadMedia()
{
    gImageSurface = SDL_LoadBMP("/home/mango/personal/emulator/SDL/assets/mgsvtpp-ddog.bmp");
    
    if (gImageSurface == nullptr)
    {
	printf("Error loading image: %s\n", SDL_GetError());
	return false;
    }

    return true;
}

void close()
{
    // It is good practice to set the pointers to nullptr
    // when they are not pointing to anything
    //
    // Otherwise, they may cause undefined bahvior in
    // different Operating Systems (C++ is weird like
    // that)
    SDL_FreeSurface(gImageSurface);
    gImageSurface = nullptr;

    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    SDL_Quit();
}

void keepOpen()
{
    // SDL_Event is the data type given to any input from
    // the user. Like mouse input, keyboard input, etc.
    //
    // All events go to a Event Queue. To pop the Event
    // Queue, you call SDL_PollEvent(&e) which pops the
    // top of the queue.
    // 
    // Each event in the Event Queue has a meaning. The
    // type of event that is pushed on the queue when
    // a user quits the application is SDL_QUIT.
    SDL_Event e;
    bool quit = false;
    while (quit == false)
    {
	while (SDL_PollEvent(&e))
	{ if (e.type == SDL_QUIT)
	    quit = true;
	}
    }
}

int main(int argc, char** argv)
{
    if (!init())
    {
	printf("Failed to initialize!\n");
	return -1;
    }

    if (!loadMedia())
    {
	printf("Failed to load media.\n");
	return -1;
    }

    SDL_BlitSurface(gImageSurface, nullptr, gWindowSurface, nullptr);
    SDL_UpdateWindowSurface(gWindow);

    keepOpen();

    close();

    return 0;
}


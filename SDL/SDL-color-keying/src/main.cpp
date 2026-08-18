#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include <chrono>

#include "../include/LTexture.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define FPS_MS 4 * 16.6667f

SDL_Window* gWindow = nullptr;
SDL_Texture* gTexture = nullptr;
SDL_Renderer* gRenderer = nullptr;

LTexture gFooTexture;
LTexture gBackgroundTexture;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	printf("Error initializing SDL: %s\n", SDL_GetError());
	return false;
    }

    const int IMG_INIT_FLAGS = IMG_INIT_PNG;
    if (IMG_Init(IMG_INIT_FLAGS) != IMG_INIT_FLAGS)
    {
	printf("Error initializing SDL_image: %s\n", IMG_GetError());
	return false;
    }

    gWindow = SDL_CreateWindow(
	        "HELLO COLOR KEYING :D",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	    );
    
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr)
    {
	printf("Error creating a renderer: %s\n", SDL_GetError());
	return false;
    }

    return true;
}

bool loadMedia()
{
    bool success = true;

    if (!gFooTexture.loadFromFile("./assets/foo.png"))
    {
	printf("Failed to load foo.png: %s\n", SDL_GetError());
	success = false;
    }

    if (!gBackgroundTexture.loadFromFile("./assets/background.png"))
    {
	printf("Failed to load background.png: %s\n", SDL_GetError());
	success = false;
    }

    return success;
}

void close()
{
    gFooTexture.free();
    gBackgroundTexture.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = nullptr;
    gWindow = nullptr;

    IMG_Quit();
    SDL_Quit();
}


inline std::chrono::duration<double, std::milli> frameTime()
{
    return std::chrono::duration<double, std::milli>(FPS_MS);
}

int main(int argc, char** argv)
{
    if (!init())
    {
	printf("Failed to initialize the program!\n");
	return -1;
    }

    if (!loadMedia())
    {
	printf("Failed to load all media!\n");
	return -1;
    }

    using sclock = std::chrono::system_clock;
    sclock::time_point tp = sclock::now();

    int frame = 0;

    SDL_Event e;
    bool quit = false;
    while (quit == false)
    {
	while (SDL_PollEvent(&e) != 0)
	{
	    if (e.type == SDL_QUIT)
	    {
		quit = true;
	    }
	}

	// every FPS_MS seconds, advance one frame
	// frame rate is 60 FPS which is 1/60 = 0.16667 seconds per frame
	
	sclock::time_point now = sclock::now();
	if (now - tp >= frameTime())
	{
	    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	    SDL_RenderClear(gRenderer);

	    gBackgroundTexture.render(0, 0);
	    gFooTexture.render(SCREEN_WIDTH / 2 + (frame + 1), SCREEN_HEIGHT / 2 - (frame + 1));

	    SDL_RenderPresent(gRenderer);

	    tp = now;
	    frame++;
	    frame %= 40;
	}
    }

    return 0;
}


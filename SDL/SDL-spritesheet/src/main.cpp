#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/LTexture.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

SDL_Rect gSpriteClip[4]{};
LTexture gSpriteSheet;

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
	        "KONNICHIWA SPRITESHEET :D",
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

    if (!gSpriteSheet.loadFromFile("./assets/sprites.png"))
    {
	printf("Failed to load sprite sheet! Error: %s\n", IMG_GetError());
	success = false;
    }
    else
    {
	// define clips
	gSpriteClip[0].x = 0;
	gSpriteClip[0].y = 0;
	gSpriteClip[0].w = 100;
	gSpriteClip[0].h = 100;

	gSpriteClip[1].x = 100;
	gSpriteClip[1].y = 0;
	gSpriteClip[1].w = 100;
	gSpriteClip[1].h = 100;

	gSpriteClip[2].x = 0;
	gSpriteClip[2].y = 100;
	gSpriteClip[2].w = 100;
	gSpriteClip[2].h = 100;

	gSpriteClip[3].x = 100;
	gSpriteClip[3].y = 100;
	gSpriteClip[3].w = 100;
	gSpriteClip[3].h = 100;
    }

    return success;
}

void close()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = nullptr;
    gWindow = nullptr;

    IMG_Quit();
    SDL_Quit();
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
	printf("Failed to load media for the program!\n");
	return -1;
    }

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

	SDL_SetRenderDrawColor(gRenderer, 0xAA, 0xAA, 0xAA, 0xFF);
	SDL_RenderClear(gRenderer);

	gSpriteSheet.render(0, 0, &gSpriteClip[0]);
	gSpriteSheet.render(SCREEN_WIDTH - 100, 0, &gSpriteClip[1]);
	gSpriteSheet.render(0, SCREEN_HEIGHT - 100, &gSpriteClip[2]);
	gSpriteSheet.render(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100, &gSpriteClip[3]);

	SDL_RenderPresent(gRenderer);
    }

    return 0;
}


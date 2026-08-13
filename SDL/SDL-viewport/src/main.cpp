/*
 * Viewports!
 *
 * In order to render to only a part of the screen, you use a viewport.
 *
 * A viewport is a portion of the screen that SDL's renderer will store in its
 * context and use for all rendering later.
 *
 * The viewport context seems to reset after any call to SDL_RenderClear()
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include <string>

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture *gTexture = nullptr;

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	printf("Error initializing SDL: %s\n", SDL_GetError());
	return false;
    }

    gWindow = SDL_CreateWindow(
	    "Hello GPU RENDERING :D",
	    SDL_WINDOWPOS_UNDEFINED,
	    SDL_WINDOWPOS_UNDEFINED,
	    SCREEN_WIDTH,
	    SCREEN_HEIGHT,
	    SDL_WINDOW_SHOWN
	    );

    if (gWindow == nullptr)
    {
	printf("Error creating SDL Window: %s\n", SDL_GetError());
	return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    if (gRenderer == nullptr)
    {
	printf("Error creating SDL Renderer: %s\n", SDL_GetError());
	return false;
    }

    const int IMG_INIT_FLAGS = IMG_INIT_PNG | IMG_INIT_JPG;
    if (IMG_Init(IMG_INIT_FLAGS) != IMG_INIT_FLAGS)
    {
        printf("Error initializing SDL_image: %s\n", IMG_GetError());
	return false;
    }

    return true;
}

SDL_Texture* loadTexture(std::string path)
{
    SDL_Texture* texture = nullptr;

    // load image into RAM
    SDL_Surface* image = IMG_Load(path.c_str());
    if (image == nullptr)
    {
	printf("Failed to load image %s\nError: %s\n", path.c_str(), IMG_GetError());
	return nullptr;
    }

    // copy the image into the GPU
    texture = SDL_CreateTextureFromSurface(gRenderer, image);
    SDL_FreeSurface(image);

    if (texture == nullptr)
    {
	printf("Failed to convert SDL_Surface to SDL_Texture: %s\n", SDL_GetError());
	return nullptr;
    }

    return texture;
}


SDL_Texture* gMedia[5]{};

bool loadMedia()
{
    std::string base = "/home/mango/personal/emulator/SDL/SDL-scale/assets/";

    std::string images[]{
        "halloween.jpg",
	"konachan_14.jpg",
	"konachan_15.jpg",
	"konachan_3.png",
	"konachan_17.jpg"
    };

    for (int imageIndex = 0; imageIndex < 5; imageIndex++)
    {
	gMedia[imageIndex] = loadTexture(base + images[imageIndex]);
	if (gMedia[imageIndex] == nullptr)
	{
	    printf("Error loading media %s\nError: %s\n", std::string(base + images[imageIndex]).c_str(), SDL_GetError());
	    return false;
	}
    }

    return true;
}

void close()
{
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyTexture(gTexture);

    gWindow = nullptr;
    gRenderer = nullptr;
    gTexture = nullptr;

    for (int mediaIndex = 0; mediaIndex < 5; mediaIndex++)
    {
	SDL_DestroyTexture(gMedia[mediaIndex]);
	gMedia[mediaIndex] = nullptr;
    }

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char** argv)
{
    if (!init())
    {
	printf("Could not initialize the program.\n");
	return -1;
    }

    if (!loadMedia())
    {
	printf("Could not load program media.\n");
	return -1;
    }

    SDL_Rect viewports[]
    {
	{0, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2},
	{SCREEN_WIDTH / 3, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2},
	{SCREEN_WIDTH * 2 / 3, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2},
	{0, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
	{SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
    };

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

	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(gRenderer);

	for (int i = 0; i < 5; i++)
	{
	    SDL_RenderSetViewport(gRenderer, &viewports[i]);
	    SDL_RenderCopy(gRenderer, gMedia[i], nullptr, nullptr);
	}

	SDL_RenderPresent(gRenderer);
    }

    close();

    return 0;
}


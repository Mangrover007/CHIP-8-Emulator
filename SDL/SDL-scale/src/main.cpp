#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

SDL_Surface* gWindowSurface = nullptr;
SDL_Window* gWindow = nullptr;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;

// SDL_image shenanigans
const int IMG_INIT_FLAGS = IMG_INIT_PNG | IMG_INIT_JPG;

// asset shenanigans
#include <filesystem>
const uint ASSET_SIZE = 54;
const std::string ASSET_DIR = "/home/mango/personal/emulator/SDL/SDL-scale/assets/";
SDL_Surface* gImages[ASSET_SIZE]{};

// random shenanigans
#include <random>
std::default_random_engine randEngine;
std::uniform_int_distribution<uint> rng(0, ASSET_SIZE - 1);

// chrono shenanigans (framerate)
#include <chrono>
const double FPS_MS = 16.667 * 60.0;

// ------------------------------------------------------------------ //
//               Program Setup               //
// ------------------------------------------------------------------ //

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	printf("Error initializing SDL: %s\n", SDL_GetError());
	return false;
    }

    if (IMG_Init(IMG_INIT_FLAGS) != IMG_INIT_FLAGS)
    {
	printf("Error initializing SDL_image: %s\n", IMG_GetError());
	return false;
    }

    gWindow = SDL_CreateWindow(
		"HELLO IMAGE LOADING AND OPTIMIZATION",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	    );

    gWindowSurface = SDL_GetWindowSurface(gWindow);

    return true;
}

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    for (int i = 0; i < ASSET_SIZE; ++i)
    {
	SDL_FreeSurface(gImages[i]);
	gImages[i] = nullptr;
    }
    
    IMG_Quit();
    SDL_Quit();
}

SDL_Surface* loadSurface(const std::string& path)
{
    SDL_Surface* optimizedSurface = nullptr;

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr)
    {
	printf("Could not load image: %s\nError: %s\n", path.c_str(), IMG_GetError());
	return nullptr;
    }

    // Blitting images, aka copying it from one surface to another surface
    // is faster if the format of both surface is same.
    //
    // SDL_ConvertSurface is used to perform that conversion beforehand.
    optimizedSurface = SDL_ConvertSurface(surface, gWindowSurface->format, 0);
    if (optimizedSurface == nullptr)
    {
	printf("Could not optimize image: %s\nError: %s\n", path.c_str(), IMG_GetError());
	return nullptr;
    }

    SDL_FreeSurface(surface);

    return optimizedSurface;
}

bool loadMedia(const std::string& path)
{
    int i = 0;

    for (auto& entry : std::filesystem::directory_iterator(path))
    {
	gImages[i] = loadSurface(entry.path().string());

	if (gImages[i] == nullptr)
	{
	    return false;
	}

	++i;
    }

    return true;
}

// ------------------------------------------------------------------ //
//               Main program               //
// ------------------------------------------------------------------ //

int main(int argc, char** argv)
{
    if (!init())
    {
	printf("Failed to initialize the program.\n");
	return -1;
    }

    if (!loadMedia(ASSET_DIR))
    {
	printf("Error starting the program: Failed to load media.\n");
	return -1;
    }

    // render something to the window
    SDL_FillRect(gWindowSurface, nullptr, SDL_MapRGB(gWindowSurface->format, 0x00, 0x00, 0x00));
    SDL_UpdateWindowSurface(gWindow);

    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();

    SDL_Event e;
    bool quit = false;
    while (quit == false)
    {
	if (SDL_PollEvent(&e) != 0 && e.type == SDL_QUIT)
	{
	    quit = true;
	}
    
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	std::chrono::duration<double, std::milli> timeDiff = now - tp;
	if (timeDiff >= std::chrono::duration<double, std::milli>(FPS_MS))
	{
	    printf("Time difference: %ld\n", std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff).count());
	    int imageIndex = rng(randEngine);

	    SDL_Rect imgRect = { .x = 0, .y = 0, .w = SCREEN_WIDTH, .h = SCREEN_HEIGHT };
	    SDL_BlitScaled(gImages[imageIndex], nullptr, gWindowSurface, &imgRect);

	    SDL_UpdateWindowSurface(gWindow);

	    tp = now;
	}
    }

    close();

    return 0;
}


/* 
 * The idea is to load all the images first and store them in an array.
 * Then, simply update the screen surface on a button press.
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

const uint SCREEN_WIDTH = 1920;
const uint SCREEN_HEIGHT = 1080;

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

SDL_Window* gWindow = nullptr;
SDL_Surface* gWindowSurface = nullptr;
SDL_Surface* gCurrentImage = nullptr;
SDL_Surface* gImageList[KEY_PRESS_SURFACE_TOTAL];

int gImageListIndex[] = {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

bool init()
{
    gWindow = SDL_CreateWindow(
	        "Input :D",
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

SDL_Surface* loadSurface(const char* path)
{
    SDL_Surface* image = SDL_LoadBMP(path);

    if (image == nullptr)
    {
	printf("Error loading bitmap image: %s\n", SDL_GetError());
    }

    return image;
}

bool loadMedia()
{
    const std::string path = "/home/mango/personal/emulator/SDL/SDL-input/assets/img-";
    bool success = true;

    for (int imageIndex = 0; imageIndex < 6; imageIndex++)
    {
        gImageList[gImageListIndex[imageIndex]] = loadSurface(std::string(path + std::to_string(imageIndex) + ".bmp").c_str());
        if (gImageList[gImageListIndex[imageIndex]] == nullptr)
	{
	    printf("Error loading bitmap image: %s\n", SDL_GetError());
	    success = false;
	}
    }

    return success;
}

void close()
{
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    for (int imageIndex = 0; imageIndex < 6; imageIndex++)
    {
	SDL_FreeSurface(gImageList[gImageListIndex[imageIndex]]);
	gImageList[gImageListIndex[imageIndex]] = nullptr;
    }

    SDL_Quit();
}
int main(int argc, char** argv)
{
    if (!init())
    {
	printf("Failed to initialize.\n");
	close();
	return -1;
    }

    // load all images into the array, exit if can't
    if (!loadMedia())
    {
	printf("Failed to load all media.\n");
	close();
	return -1;
    }

    SDL_Event e;
    bool quit = false;

    while (!quit)
    {
	while (SDL_PollEvent(&e) != 0)
	{
	    if (e.type == SDL_QUIT)
	    {
		quit = true;
	    }
	    else if (e.type == SDL_KEYDOWN)
	    {
		SDL_FillRect(gWindowSurface, nullptr, SDL_MapRGB(gWindowSurface->format, 0x00, 0x00, 0x00));
		switch (e.key.keysym.sym)
		{
		    case SDLK_UP:
			gCurrentImage = gImageList[KEY_PRESS_SURFACE_UP];
			break;
		    case SDLK_DOWN:
			gCurrentImage = gImageList[KEY_PRESS_SURFACE_DOWN];
			break;
		    case SDLK_RIGHT:
			gCurrentImage = gImageList[KEY_PRESS_SURFACE_RIGHT];
			break;
		    case SDLK_LEFT:
			gCurrentImage = gImageList[KEY_PRESS_SURFACE_LEFT];
			break;
		    default:
			gCurrentImage = gImageList[KEY_PRESS_SURFACE_DEFAULT];
			break;
		}
	    }

	    SDL_BlitSurface(gCurrentImage, nullptr, gWindowSurface, nullptr);

	    SDL_UpdateWindowSurface(gWindow);
	}
    }

    return 0;
}


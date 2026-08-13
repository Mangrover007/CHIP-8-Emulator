#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>


SDL_Renderer* gRenderer = nullptr;
SDL_Window* gWindow = nullptr;
SDL_Texture* gWindowTexture = nullptr;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 640;

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

bool loadMedia()
{
    std::string path = "/home/mango/personal/emulator/SDL/SDL-scale/assets/genshin.jpg";

    gWindowTexture = loadTexture(path);
    if (gWindowTexture == nullptr)
    {
        printf("Error loading texture %s\n", path.c_str());
	return false;
    }

    return true;
}

void close()
{
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyTexture(gWindowTexture);

    gWindow = nullptr;
    gRenderer = nullptr;
    gWindowTexture = nullptr;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char** argv)
{
    if (!init())
    {
	printf("Could not initialize program.\n");
	return -1;
    }

    if (!loadMedia())
    {
	printf("Could not load media.\n");
	return -1;
    }

    SDL_RenderClear(gRenderer);

    SDL_RenderCopy(gRenderer, gWindowTexture, nullptr, nullptr);

    SDL_RenderPresent(gRenderer);

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
    }

    close();

    return 0;
}


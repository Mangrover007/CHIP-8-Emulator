#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../include/globals.h"
#include "../include/Player.h"

#include <chrono>

SDL_Window* gWindow;
SDL_Renderer* gRenderer;
Player gPlayer;
LTexture gTextTexture;
TTF_Font* gFont;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	printf("Error initializing SDL video subsystem!\n%s\n", SDL_GetError());
	return false;
    }

    const int IMG_INIT_FLAGS = IMG_INIT_PNG;
    if (IMG_Init(IMG_INIT_FLAGS) != IMG_INIT_FLAGS)
    {
	printf("Error initializing SDL video subsystem!\n%s\n", SDL_GetError());
	return false;
    }

    if (TTF_Init() == -1)
    {
	printf("Error initializing SDL video subsystem!\n%s\n", SDL_GetError());
	return false;
    }

    gFont = TTF_OpenFont("/usr/share/fonts/OTF/ipag.ttf", 56);

    gWindow = SDL_CreateWindow(
		"HELLO ANIMATION!!! :D",
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

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    
    if (gRenderer == nullptr)
    {
	printf("Error initializing the renderer: %s\n", SDL_GetError());
	return false;
    }

    return true;
}

// !NOTE: not sure if SDL_GetError() calls are right for error messages
bool loadAssets()
{
    bool success = true;
    
    if (!gPlayer.loadFromFile("./assets/foo-walking.png"))
    {
	printf("Error loading foo animation sprite!\n%s\n", SDL_GetError());
	success = false;
    }
    else
    {
	SDL_Rect spriteFrames[4];

	spriteFrames[0].x = 0;
	spriteFrames[0].y = 0;
	spriteFrames[0].w = 64;
	spriteFrames[0].h = 205;

	spriteFrames[1].x = 64;
	spriteFrames[1].y = 0;
	spriteFrames[1].w = 64;
	spriteFrames[1].h = 205;

	spriteFrames[2].x = 128;
	spriteFrames[2].y = 0;
	spriteFrames[2].w = 64;
	spriteFrames[2].h = 205;

	spriteFrames[3].x = 192;
	spriteFrames[3].y = 0;
	spriteFrames[3].w = 64;
	spriteFrames[3].h = 205;

	gPlayer.setSpriteFrames(spriteFrames);
	gPlayer.setIdleFrame(spriteFrames[0]);
    }

    if (!gTextTexture.loadTextTexture("ようこそ！僕の世界へ！", SDL_Color{0x00, 0x00, 0x00, 0xFF}))
    {
	printf("Error loading text!\n%s\n", SDL_GetError());
	success = false;
    }

    return success;
}

void close()
{
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);

    gPlayer.~Player();

    TTF_CloseFont(gFont);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void update()
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    gTextTexture.render(0, 0);

    gPlayer.render();

    SDL_RenderPresent(gRenderer);
}

void gameLoop()
{
    using sclock = std::chrono::system_clock;
    sclock::time_point tp = sclock::now();

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
	    else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
	    {
		gPlayer.move(e.key);
	    }
	}

	sclock::time_point now = sclock::now();
	if (now - tp >= std::chrono::duration<double, std::milli>(FPS_MS))
	{
	    tp = now;
	    update();
	}
    }

    close();
}

int main(int argc, char** argv)
{

    if (!init())
    {
	printf("Failed to initialize the program!\n");
	return -1;
    }

    printf("Program initialization successful! Loading assets...\n");

    if (!loadAssets())
    {
	printf("Failed to laod assets!\n");
	return -1;
    }

    gameLoop();

    return 0;
}


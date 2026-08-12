/* 
 * In order to interact with input devices and device hardware, we need
 * an API that takes all those and turns it into something C++ can
 * interact with.
 *
 * SDL is what does that.
 *
 * */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <chrono>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 800

const double FPS_MS = 8;

int main( int argc, char** argv )
{
    SDL_Window* window = nullptr;
    SDL_Surface* windowSurface = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	printf("Error initializing SDL: %s\n", SDL_GetError());
	return -1;
    }

    printf("SDL initialization complete!\n");

    window = SDL_CreateWindow(
	    "HELLO WORLD :D",
	    SDL_WINDOWPOS_UNDEFINED,
	    SDL_WINDOWPOS_UNDEFINED,
	    SCREEN_HEIGHT,
	    SCREEN_WIDTH,
	    SDL_WINDOW_SHOWN
	    );

    if (window == nullptr)
    {
	printf("An error occurred when creating the window.\n %s\n", SDL_GetError());
	return -1;
    }

    windowSurface = SDL_GetWindowSurface(window);

    using clock = std::chrono::high_resolution_clock;
    clock::time_point sys_clock = clock::now();

    int frame = 0;

    // at first, its FF0000 --> FF00FF --> 0000FF --> 00FFFF --> 00FF00 --> FFFF00 --> FF0000
    // so i see a line forming here
    // for red, it goes   FF, FF, 00, 00, 00, FF
    // for green, it goes 00, 00, 00, FF, FF, FF
    // for blue, it goes  00, FF, FF, FF, 00, 00
    //
    // OH
    //
    // Represent this in bits
    // 110001
    // 000111
    // 011100
    //
    // This is wrong though. If I represent who needs to increase:
    // 00 -1 00 1  well fuck
    // 00  0 10 0
    // 01 00 -10
    //
    // I can represent this as an array

    uint8_t red{ 0xFF }, green{ 0x00 }, blue{ 0x00 };
    int8_t r[]{ 0, -1, 0, 0, 1, 0 };
    int8_t g[]{ 0, 0, 1, 0, 0, -1 };
    int8_t b[]{ 1, 0, 0, -1, 0, 0 };
    uint8_t wheel{ 0x00 };
    uint8_t i = 0;

    while (true)
    {
	if (wheel == 0xFF)
	{
	    wheel = 0x00;
	    i = (i + 1) % 6;
	    printf("RED: %d, GREEN: %d, BLUE: %d\n", r[i], g[i], b[i]);
	    printf("RED: %d, GREEN: %d, BLUE: %d\n", red, green, blue);
	}

	clock::time_point now = clock::now();
	std::chrono::duration<double, std::milli> timeDiff = now - sys_clock;
	if (timeDiff >= std::chrono::duration<double, std::milli>(FPS_MS))
	{
	    wheel += 0x01;

	    red   += (0x01 * (r[i]));
	    green += (0x01 * (g[i]));
	    blue  += (0x01 * (b[i]));

	    SDL_FillRect(windowSurface, nullptr, SDL_MapRGB(windowSurface->format, red, green, blue));

	    SDL_UpdateWindowSurface(window);

	    sys_clock = now;
	}
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


#include "../include/emulator.hpp"


Emulator::Emulator() : chip8{}
{
    mWindow = nullptr;
    mRenderer = nullptr;

    init();
}


bool Emulator::init()
{
    bool success = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	printf("Failed to initialize SDL video subsystem.\n");
	printf("%s\n", SDL_GetError());
	success = false;
    }
    else
    {
	mWindow = SDL_CreateWindow(
		mTitle,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		chip8.DISPLAY_WIDTH, chip8.DISPLAY_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (mWindow == nullptr)
	{
	    printf("Failed to create SDL window.\n");
	    printf("%s\n", SDL_GetError());
	    success = false;
	}
	else
	{
	    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	    
	    if (mRenderer == nullptr)
	    {
		printf("Failed to create SDL renderer.\n");
		printf("%s\n", SDL_GetError());
		success = false;
	    }
	}
    }

    return success;
}


void Emulator::LoadROM(const char* filename)
{
    chip8.LoadROM(filename);
}


/* !! Experimental !!
 * This function currently creates a new surface and a new texture every
 * single frame. I still need to experiment if texture streaming
 * SDL_UpdateTexture() is faster or slower than this.
 */
void Emulator::StartGame()
{
    bool quit = false;

    // !! this is only for testing purposes !!
    int i = 0;

    // information for running the game loop on a fixed framerate
    const float framerate = (1.0 / 60) * 1000;
    uint64_t startTick = SDL_GetTicks64();
    uint64_t lastTick = SDL_GetTicks64();

    // game loop
    while (quit == false)
    {
	startTick = SDL_GetTicks64();

	quit = ProcessInput();

	SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(mRenderer);

	// we will render everything here
	chip8.Cycle();

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*) chip8.display, chip8.DISPLAY_WIDTH, chip8.DISPLAY_HEIGHT, 32, 4 * chip8.DISPLAY_WIDTH, 0, 0, 0, 0);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);

	SDL_RenderCopy(mRenderer, texture, nullptr, nullptr);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);

	SDL_RenderPresent(mRenderer);

	float dt = startTick - lastTick;
	if (dt <= framerate)
	{
	    SDL_Delay(framerate - dt);
	}

	lastTick = startTick;
    }
    
    close();
}


bool Emulator::ProcessInput()
{
    bool quit = false;

    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
	if (e.type == SDL_QUIT)
	{
	    quit = true;
	    break;
	}
	else if (e.type == SDLK_DOWN)
	{
	    switch (e.key.keysym.sym)
	    {
		case SDLK_x:
		    chip8.keypad[0x0] = 1;
		    break;

		case SDLK_1:
		    chip8.keypad[0x1] = 1;
		    break;

		case SDLK_2:
		    chip8.keypad[0x2] = 1;
		    break;

		case SDLK_3:
		    chip8.keypad[0x3] = 1;
		    break;

		case SDLK_q:
		    chip8.keypad[0x4] = 1;
		    break;

		case SDLK_w:
		    chip8.keypad[0x5] = 1;
		    break;

		case SDLK_e:
		    chip8.keypad[0x6] = 1;
		    break;

		case SDLK_a:
		    chip8.keypad[0x7] = 1;
		    break;

		case SDLK_s:
		    chip8.keypad[0x8] = 1;
		    break;

		case SDLK_d:
		    chip8.keypad[0x9] = 1;
		    break;

		case SDLK_z:
		    chip8.keypad[0xA] = 1;
		    break;

		case SDLK_c:
		    chip8.keypad[0xB] = 1;
		    break;

		case SDLK_4:
		    chip8.keypad[0xC] = 1;
		    break;

		case SDLK_r:
		    chip8.keypad[0xD] = 1;
		    break;

		case SDLK_f:
		    chip8.keypad[0xE] = 1;
		    break;

		case SDLK_v:
		    chip8.keypad[0xF] = 1;
		    break;
	    }
	}
	else if (e.type == SDLK_UP)
	{
	    switch (e.key.keysym.sym)
	    {
		case SDLK_x:
		    chip8.keypad[0x0] = 0;
		    break;

		case SDLK_1:
		    chip8.keypad[0x1] = 0;
		    break;

		case SDLK_2:
		    chip8.keypad[0x2] = 0;
		    break;

		case SDLK_3:
		    chip8.keypad[0x3] = 0;
		    break;

		case SDLK_q:
		    chip8.keypad[0x4] = 0;
		    break;

		case SDLK_w:
		    chip8.keypad[0x5] = 0;
		    break;

		case SDLK_e:
		    chip8.keypad[0x6] = 0;
		    break;

		case SDLK_a:
		    chip8.keypad[0x7] = 0;
		    break;

		case SDLK_s:
		    chip8.keypad[0x8] = 0;
		    break;

		case SDLK_d:
		    chip8.keypad[0x9] = 0;
		    break;

		case SDLK_z:
		    chip8.keypad[0xA] = 0;
		    break;

		case SDLK_c:
		    chip8.keypad[0xB] = 0;
		    break;

		case SDLK_4:
		    chip8.keypad[0xC] = 0;
		    break;

		case SDLK_r:
		    chip8.keypad[0xD] = 0;
		    break;

		case SDLK_f:
		    chip8.keypad[0xE] = 0;
		    break;

		case SDLK_v:
		    chip8.keypad[0xF] = 0;
		    break;
	    }
	}
	else
	{
	    // printf("Emulator is taking input.\n");
	}
    }

    return quit;
}


void Emulator::close()
{
    SDL_DestroyRenderer(mRenderer);

    mWindow = nullptr;
    mRenderer = nullptr;

    SDL_Quit();
}


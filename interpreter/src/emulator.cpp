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
		SDL_WINDOW_SHOWN
		| SDL_WINDOW_RESIZABLE
		// | SDL_WINDOW_FULLSCREEN
		);

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


/*
std::chrono::duration<uint, std::milli> to_milli(std::chrono::steady_clock::duration T)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(T);
}
*/


/* !! Experimental !!
 * This function currently creates a new surface and a new texture every single frame. I still need to experiment if texture streaming
 * SDL_UpdateTexture() is faster or slower than this.
 */
void Emulator::StartGame()
{
    bool stop = false;

    // typedefs are scoped
    typedef std::chrono::steady_clock clock;

    // emulator running at a CPU clock speed of 1000Hz (1e6 nanoseconds = 1 millisecond)
    const clock::duration time_step = clock::duration((int) 1e6);
    clock::time_point startTick = clock::now();
    clock::time_point lastTick = clock::now();
    
    clock::duration dt = clock::duration(0);

    // main ideas:
    // 1. only update lastTick when a full time_step has elapsed
    // 2. only call chip8.Cycle() every time_step
    //
    // case 1: dt < time_step (don't update lastTick), wait for startTick
    // to get past the current time_step
    //
    // case 2: dt >= time_step
    //
    // in case 2, startTick may start from some time after time_step
    //
    // dt in this case will encapsulate some exact multiple of time_step
    // + a small amount of time since the start of the current time_step
    //
    // this is why you only subtract time_steps from dt
    //
    // this is also why lastTick is also updated as startTick - dt instead
    // of just startTick
    // because startTick may have recorded the time slightly after the start
    // of the current time_step
    // so you need to account for that small dt
    
    // you could imagine a series of time_steps as blocks in time arranged
    // in a sequence. the first startTick starts at the first block at the
    // very beginning. this is also the first lastTick
    //
    // startTick advances variably within that one time_step, and may advance
    // to a bit farther than a single time_step, or multiple time_steps.
    //
    // dt accounts for all of that. lastTick is always the start of the
    // previous time_step. with all this in mind,
    // define dt = startTick - lastTick
    //
    // both cases have already been discussed above

    // uint instructions_executed = 0;
    // clock::time_point programStartTime = startTick;

    // emulation loop
    while (stop == false)
    {
	startTick = clock::now();
	dt = startTick - lastTick;

	bool exec = false;

	stop = ProcessInput();

	while (dt >= time_step)
	{
	    exec = true;
	    chip8.Cycle();
	    dt -= time_step;
	    // instructions_executed++;
	}

	if (exec)
	{
	    lastTick = startTick - dt;

	    SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
	    SDL_RenderClear(mRenderer);

	    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*) chip8.display, chip8.DISPLAY_WIDTH, chip8.DISPLAY_HEIGHT, 32, 4 * chip8.DISPLAY_WIDTH, 0, 0, 0, 0);
	    SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);

	    SDL_RenderCopy(mRenderer, texture, nullptr, nullptr);

	    SDL_FreeSurface(surface);
	    SDL_DestroyTexture(texture);

	    SDL_RenderPresent(mRenderer);
	}
    }

    // the formula for calculating number of instructions executed per second
    // uncomment all the related variables and functions
    // ((float) (instructions_executed * 1000) / to_milli(clock::now() - programStartTime).count())

    close();
}


bool Emulator::ProcessInput()
{
    bool exit = false;

    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
	if (e.type == SDL_QUIT)
	{
	    exit = true;
	    break;
	}
	else if (e.type == SDL_KEYDOWN)
	{
	    switch (e.key.keysym.sym)
	    {
		case SDLK_LEFT:
		    chip8.keypad[0x0] = 1;
		    break;

		case SDLK_RIGHT:
		    chip8.keypad[0x1] = 1;
		    break;

		case SDLK_UP:
		    chip8.keypad[0x2] = 1;
		    break;

		case SDLK_DOWN:
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
	else if (e.type == SDL_KEYUP)
	{
	    switch (e.key.keysym.sym)
	    {
		case SDLK_LEFT:
		    chip8.keypad[0x0] = 0;
		    break;

		case SDLK_RIGHT:
		    chip8.keypad[0x1] = 0;
		    break;

		case SDLK_UP:
		    chip8.keypad[0x2] = 0;
		    break;

		case SDLK_DOWN:
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
    }

    return exit;
}


void Emulator::close()
{
    SDL_DestroyRenderer(mRenderer);

    mWindow = nullptr;
    mRenderer = nullptr;

    SDL_Quit();
}


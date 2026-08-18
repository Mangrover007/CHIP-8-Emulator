#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>

#include "../include/LTexture.h"

// extern will tell this file that this variable is defined
// some where else
extern SDL_Renderer* gRenderer;

LTexture::LTexture()
{
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

void LTexture::free()
{
    if (mTexture != nullptr)
    {
	SDL_DestroyTexture(mTexture);
	mWidth = 0;
	mHeight = 0;
    }
}

LTexture::~LTexture()
{
    free();
}

bool LTexture::loadFromFile(const std::string& path)
{
    free();

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr)
    {
	printf("Failed to load image: %s\nError: %s\n", path.c_str(), IMG_GetError());
	return false;
    }
    
    // set color key of the surface so that all pixels having the
    // color key value will automatically not render, effectively
    // making it "transparent"
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x00, 0xFF, 0xFF));

    // the texture is then the only that part of the texture that
    // did not get deleted by the color key
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    if (texture == nullptr)
    {
	printf("Failed to convert surface into texture: %s\nError: %s\n", path.c_str(), SDL_GetError());
	return false;
    }

    mWidth = surface->w;
    mHeight = surface->h;
    mTexture = texture;

    SDL_FreeSurface(surface);

    return true;
}

void LTexture::render(const int& x, const int& y)
{
    SDL_Rect image = {x, y, mWidth, mHeight};

    // this is the same function used for rendering any image
    SDL_RenderCopy(gRenderer, mTexture, nullptr, &image);
}


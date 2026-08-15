#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <stdio.h>

#include "../include/LTexture.h"

// What we are doing is using the global renderer to render to
// the window, but controlling how it is rendered through this
// class.
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

    // the texture is then the only that part of the surface that
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

void LTexture::render(const int& x, const int& y, const SDL_Rect* clip)
{
    SDL_Rect image = {x, y, mWidth, mHeight};

    // clip is a SDL_Rect {x, y, w, h} which defines the starting (x, y)
    // coordinates of a sprite in a spritesheet, and the rectangle's
    // width and height from that (x, y) of that sprite

    if (clip != nullptr)
    {
	// here, the destination will show the real size of the sprite
	// if we were to mess around with image.w and image.h, then we
	// can adjust the area on to which the entire sprite clip will
	// be stretched
	image.w = clip->w;
	image.h = clip->h;
    }

    // this function stretches the texture to the dest_rect's dimensions
    SDL_RenderCopy(gRenderer, mTexture, clip, &image);
}


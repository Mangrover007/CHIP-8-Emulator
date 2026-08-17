#include "../include/Player.h"

Player::Player()
{
    posX = SCREEN_WIDTH / 2;
    posY = SCREEN_HEIGHT / 2;
    frame = 0;
}

Player::~Player()
{
    this->LTexture::~LTexture();
}

void Player::move(const SDL_KeyboardEvent& e)
{
    switch (e.keysym.sym)
    {
    case SDLK_UP:
	posY = std::max(posY - P_SPEED, 0);
	break;
    case SDLK_DOWN:
	posY = std::min(posY + P_SPEED, SCREEN_HEIGHT - 205);
	break;
    case SDLK_LEFT:
	posX = std::max(posX - P_SPEED, 0);
	break;
    case SDLK_RIGHT:
	posX = std::min(posX + P_SPEED, SCREEN_WIDTH - 64);
	break;
    }
}

void Player::render()
{
    frame = (frame + 1) % 4;
    this->LTexture::render(posX, posY, &spriteFrames[frame]);
}

void Player::setSpriteFrames(const SDL_Rect* spriteClips)
{
    for (int i = 0; i < 4; i++)
    {
	SDL_Rect clip = spriteClips[i];
	spriteFrames[i] = clip;
    }
}


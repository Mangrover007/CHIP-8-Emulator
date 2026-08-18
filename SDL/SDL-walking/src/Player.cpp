#include "../include/Player.h"

Player::Player()
{
    posX = SCREEN_WIDTH / 2;
    posY = SCREEN_HEIGHT / 2;
    frame = 0;

    isMoving = false;

    angle = 0;
    centre = nullptr;

    direction = 0;
}

Player::~Player()
{
    this->LTexture::~LTexture();
}

void Player::move(const SDL_KeyboardEvent& e)
{
    if (e.type == SDL_KEYDOWN)
    {
	switch (e.keysym.sym)
	{
	    case SDLK_UP:
		posY = std::max(posY - P_SPEED, 0);
		isMoving = true;
		break;
	    case SDLK_DOWN:
		posY = std::min(posY + P_SPEED, SCREEN_HEIGHT - 205);
		isMoving = true;
		break;
	    case SDLK_LEFT:
		posX = std::max(posX - P_SPEED, 0);
		isMoving = true;
		break;
	    case SDLK_RIGHT:
		posX = std::min(posX + P_SPEED, SCREEN_WIDTH - 64);
		isMoving = true;
		break;
	    case SDLK_q:
		angle = angle + 90;
		isMoving = false;
		break;
	    case SDLK_e:
		angle = angle - 90;
		isMoving = false;
		break;
	}
    }
    else if (e.type == SDL_KEYUP)
    {
	isMoving = false;
    }
}

void Player::render()
{
    if (isMoving)
    {
	frame = (frame + 1) % 4;
	this->LTexture::render(posX, posY, &spriteFrames[frame], angle, centre, SDL_FLIP_NONE);
    }
    else
    {
	this->LTexture::render(posX, posY, &idleFrame, angle, centre, SDL_FLIP_NONE);
    }
}

void Player::setSpriteFrames(const SDL_Rect* spriteClips)
{
    for (int i = 0; i < 4; i++)
    {
	SDL_Rect clip = spriteClips[i];
	spriteFrames[i] = clip;
    }
}

void Player::setIdleFrame(const SDL_Rect& spriteFrame)
{
    idleFrame = spriteFrame;
}


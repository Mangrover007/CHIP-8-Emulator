#include <SDL2/SDL.h>
#include "./LTexture.h"
#include "./globals.h"

class Player : public LTexture
{
private:
    const int P_SPEED = 10;
    int posX;
    int posY;

    int frame;

    SDL_Rect spriteFrames[4];

public:
    Player();
    ~Player();

    void move(const SDL_KeyboardEvent&);
    void render();

    void setSpriteFrames(const SDL_Rect*);
};


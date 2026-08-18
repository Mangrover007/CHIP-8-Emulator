#include <SDL2/SDL.h>
#include "./LTexture.h"
#include "./globals.h"

class Player : public LTexture
{
private:
    const int P_SPEED = 10;
    int posX;
    int posY;
    bool isMoving;

    // sprite data and animation date
    SDL_Rect spriteFrames[4];
    SDL_Rect idleFrame;
    int frame;

    // 0 = facing left, 1 = facing right
    int direction;

    // rotation
    double angle;
    SDL_Point* centre;

public:
    Player();
    ~Player();

    void move(const SDL_KeyboardEvent&);
    void render();

    void setSpriteFrames(const SDL_Rect*);
    void setIdleFrame(const SDL_Rect&);
};


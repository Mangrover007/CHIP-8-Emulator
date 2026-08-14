#include <SDL2/SDL.h>
#include <string>

class LTexture
{
public:
    LTexture();
    ~LTexture();

    void free();
    bool loadFromFile(const std::string& path);

    void render(const int& x, const int& y);

private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};


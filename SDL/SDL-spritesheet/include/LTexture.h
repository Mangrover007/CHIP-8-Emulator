#include <SDL2/SDL.h>
#include <string>

class LTexture
{
public:
    LTexture();
    ~LTexture();

    void free();
    bool loadFromFile(const std::string& path);

    void setColor(Uint8 r, Uint8 g, Uint8 b);
    void render(const int& x, const int& y, const SDL_Rect* clip = nullptr);

private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};


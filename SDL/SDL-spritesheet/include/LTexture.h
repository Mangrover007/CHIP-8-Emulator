#include <SDL2/SDL.h>
#include <string>

class LTexture
{
public:
    LTexture();
    ~LTexture();

    void free();
    bool loadFromFile(const std::string& path);

    void setColor(const Uint8& r, const Uint8& g, const Uint8& b);
    void render(const int& x, const int& y, const SDL_Rect* clip = nullptr);

    void setBlendMode(const SDL_BlendMode& blendmode);
    void setAlpha(const Uint8& alpha);
    
private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};


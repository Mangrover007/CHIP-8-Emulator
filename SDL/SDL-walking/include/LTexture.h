#include <SDL2/SDL.h>
#include <string>

class LTexture
{
public:
    LTexture();
    ~LTexture();

    void free();
    bool loadTextTexture(const std::string& text, SDL_Color color);
    bool loadFromFile(const std::string& path);

    void setColor(Uint8 r, Uint8 g, Uint8 b);
    void render(int x, int y, const SDL_Rect* clip = nullptr, double angle = 0.0, const SDL_Point* centre = nullptr, const SDL_RendererFlip& flip = SDL_FLIP_NONE);

    void setBlendMode(const SDL_BlendMode& blendmode);
    void setAlpha(const Uint8& alpha);

    int getHeight();
    int getWidth();
    
private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};


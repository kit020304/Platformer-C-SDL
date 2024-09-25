#ifndef KURSACH2_ANIMATION_H
#define KURSACH2_ANIMATION_H
#include <string>
#include <SDL.h>

class Animation {
public:
    Animation(){};
    void Update();
    void Draw(float x, float y, int spritWidth, int spriteHeight, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void SetProps(std::string textureID, int spriteRow, int frameCount, int animSpeed);
    int m_SpriteFrame;
private:
    int m_SpriteRow;
    int m_AnimSpeed, m_FrameCount;
    std::string m_TextureID;
};


#endif //KURSACH2_ANIMATION_H

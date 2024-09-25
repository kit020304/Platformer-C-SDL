#ifndef KURSACH2_GAMEOBJECT_H
#define KURSACH2_GAMEOBJECT_H
#include "Object.h"
#include "Transform.h"
#include <SDL.h>
#include "Point.h"
struct Proporties{
public:
    Proporties(std::string textureID = "", int x = 0, int y = 0, int width = 0, int height = 0, SDL_RendererFlip flip = SDL_FLIP_NONE){
        X = x;
        Y = y;
        Flip = flip;
        Width = width;
        Height = height;
        TextureID = textureID;
    }
    std::string TextureID;
    int Width, Height;
    int X,Y;
    SDL_RendererFlip Flip;
};
class GameObject: public Object{
public:
    GameObject(Proporties* props): m_TextureID(props->TextureID), m_Width(props->Width), m_Height(props->Height), m_Flip(props->Flip){
        m_Transform = new Transform(props->X, props->Y);
        float px = props->X + props->Width/2;
        float py = props->Y + props->Height/2;
        m_Origin = new Point(px, py);
    }
    inline Point* GetOrigin() {return m_Origin;}
    virtual void Draw() = 0;
    virtual int Update(float dt) = 0;
    virtual void Clean() = 0;
protected:
    Point* m_Origin;
    Transform* m_Transform;
    int m_Width, m_Height;
    double m_Compression;
    std::string m_TextureID;
    SDL_RendererFlip m_Flip;
};


#endif //KURSACH2_GAMEOBJECT_H

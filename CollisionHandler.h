#ifndef KURSACH2_COLLISIONHANDLER_H
#define KURSACH2_COLLISIONHANDLER_H
#include <SDL.h>
#include <vector>
#include "TileLayer.h"
#include "GameMap.h"

class CollisionHandler {
public:
    bool MapCollision(SDL_Rect a, int Lay);
    bool CheckCollision(SDL_Rect a, SDL_Rect b);
    inline static CollisionHandler* GetInstance(){
        return s_Instance = (s_Instance != nullptr)? s_Instance : new CollisionHandler();
    }
private:
    CollisionHandler();
    TileMap m_CollisionTilemap;
    TileLayer* m_CollisionLayer;
    static CollisionHandler* s_Instance;
};


#endif //KURSACH2_COLLISIONHANDLER_H

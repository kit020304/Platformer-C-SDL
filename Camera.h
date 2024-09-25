#ifndef KURSACH2_CAMERA_H
#define KURSACH2_CAMERA_H
#include "Point.h"
#include "Vector2D.h"
#include <SDL.h>
#include "Engine.h"

class Camera {
public:
    static Camera* GetInstance(){
        return s_Instance = (s_Instance != nullptr) ? s_Instance : new Camera();
    }
    void Update(float dt);
    inline SDL_Rect GetViewBox() {return m_ViewBox;}
    inline Vector2D GetPosition(){return m_Position;}
    inline void SetTarget(Point* target){m_Target = target;}

private:
    Camera(){m_ViewBox = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};}
    Point* m_Target;
    Vector2D m_Position;
    SDL_Rect m_ViewBox;
    static Camera* s_Instance;
};


#endif //KURSACH2_CAMERA_H

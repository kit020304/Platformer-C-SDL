#ifndef KURSACH2_INPUT_H
#define KURSACH2_INPUT_H
#include <SDL.h>
#include "Vector2D.h"
enum Axis {HORIZONTAL, VERTICAL};
class Input {
public:
    static Input* GetInstance(){
        return s_Instance = (s_Instance != nullptr) ? s_Instance : new Input();
    }

    void Listen();
    bool GetKeyDown(SDL_Scancode key);
    bool GetLKM(){return LKM;}
    bool GetPKM(){return PKM;}
    int GetAxisKey(Axis axis);
    void SetLKM(){LKM = false;}
    void SetPKM(){PKM = false;}
    Vector2D* GetMousePosition() { return new Vector2D(m_MousePosX, m_MousePosY); }
    void MouseUpdatePos();

private:
    Input();
    void KeyUp();
    void KeyDown();
    bool LKM = false, PKM = false;
    const Uint8 * m_KeyStates;
    static Input* s_Instance;
    int m_MousePosX, m_MousePosY;
};


#endif //KURSACH2_INPUT_H

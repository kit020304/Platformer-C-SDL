#include "Input.h"
#include "Engine.h"
#include "Menu.h"
Input* Input::s_Instance = nullptr;

Input::Input() {
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}
void Input::Listen() {
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch (event.type) {
            case SDL_QUIT: Engine::GetInstance()->Quit(); Menu::GetInstance()->Quit(); Pause::GetInstance()->Quit(); Death::GetInstance()->Quit(); Record::GetInstance()->Quit(); break;
            case SDL_KEYDOWN: KeyDown(); break;
            case SDL_KEYUP: KeyUp(); break;
            case SDL_MOUSEMOTION: MouseUpdatePos(); break;
            case SDL_MOUSEBUTTONDOWN:{
                switch (event.button.button){
                    case SDL_BUTTON_LEFT: LKM = true; break;
                    case SDL_BUTTON_RIGHT: PKM = true; break;
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:{
                switch(event.button.button){
                    case SDL_BUTTON_LEFT: LKM = false; break;
                    case SDL_BUTTON_RIGHT: PKM = false; break;
                }
                break;
            }
        }
    }
}

bool Input::GetKeyDown(SDL_Scancode key) {
    return (m_KeyStates[key] == 1);
}

void Input::KeyUp() {
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Input::KeyDown() {
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}

int Input::GetAxisKey(Axis axis) {
    switch(axis){
        case HORIZONTAL:
            if (GetKeyDown(SDL_SCANCODE_D) || GetKeyDown(SDL_SCANCODE_RIGHT))
                return 1;
            if(GetKeyDown(SDL_SCANCODE_A) || GetKeyDown(SDL_SCANCODE_LEFT))
                return -1;
            break;
        case VERTICAL:
            if (GetKeyDown(SDL_SCANCODE_W) || GetKeyDown(SDL_SCANCODE_UP))
                return 1;
            if(GetKeyDown(SDL_SCANCODE_S) || GetKeyDown(SDL_SCANCODE_DOWN))
                return -1;
            break;
        default:
            return 0;
    }
}
void Input::MouseUpdatePos()
{
    SDL_GetMouseState(&m_MousePosX,&m_MousePosY);
}

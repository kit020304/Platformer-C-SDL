#ifndef KURSACH2_ENGINE_H
#define KURSACH2_ENGINE_H
#include <SDL.h>
#include <SDL_image.h>
#include "GameMap.h"
#include "GameObject.h"
#include "ctime"


#define SCREEN_WIDTH 1680
#define SCREEN_HEIGHT 960

class Engine {
public:
    static Engine* GetInstance(){
        return s_Instance = (s_Instance != nullptr) ? s_Instance : new Engine();
    }
    bool Init();
    bool Clean();
    void Quit();

    void Update();
    void Render();
    void Events();

    inline void Run(){m_IsRunning = true;};
    inline void Stop(){m_IsRunning = false; UnsetLevel();}

    void UnsetLevel();
    void SetLevel1();
    void SetLevel2();
    void SetLevel3();
    void SetCurLevel(){m_CurLevel = 1;}

    GameMap* GetMap();
    inline SDL_Window* GetWindow(){return m_Window;}
    inline bool IsRunning(){return m_IsRunning;}
    inline SDL_Renderer* GetRenderer(){return m_Renderer;}
    static long int StartTime;
    static long int EndTime;
private:
    Engine(){};
    int m_CurLevel = 1;
    bool m_IsRunning;
    GameMap* m_LevelMap1;
    GameMap* m_LevelMap2;
    GameMap* m_LevelMap3;
    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    static Engine* s_Instance;
    std::vector<GameObject*> m_GameObjects;

};


#endif //KURSACH2_ENGINE_H
